#include "worker.h"
#include "pageparser.h"
#include <iostream>

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}

void Worker::runParsing(QString url, QString text, int nt, int nl)
{
    maxLinks = nl;

    for (int i = 0; i < nt; ++i)
    {
        vecThread.push_back(new QThread);
        vecPageParser.push_back(new PageParser(i, text));

        QObject::connect(vecPageParser[i], SIGNAL(finishedParsing(QStringList, QString, QString, int)),
                         this,             SLOT(onPageParsed(QStringList, QString, QString, int)));

        QObject::connect(this,             SIGNAL(setTaskForThread(uint, QString)),
                         vecPageParser[i], SLOT(parseUrl(uint, QString)));

        vecPageParser[i]->moveToThread(vecThread[i]);

        vecThread[i]->start();
    }

    emit setTaskForThread(0, url);
}

void Worker::stopParsing()
{
    queue.clear();
    history.clear();

    for (int i = 0; i < vecThread.size(); ++i)
    {
        vecThread[i]->quit();
        vecThread[i]->wait();

        vecPageParser[i]->deleteLater();
        vecThread[i]->deleteLater();
    }

    vecPageParser.clear();
    vecThread.clear();

    emit workerStopped();
}

void Worker::onPause()
{
    for (int i = 0; i < vecThread.size(); ++i)
    {
        vecPageParser[i]->isPause = true;
        vecPageParser[i]->isReady = false;
    }
}

void Worker::onResume()
{
    for (int i = 0; i < vecThread.size(); ++i)
    {
        vecPageParser[i]->isPause = false;
        vecPageParser[i]->isReady = true;
    }

    if (!queue.empty())
    {
        emit setTaskForThread(0, queue.front());
        queue.pop_front();
    }
}

void Worker::onPageParsed(QStringList newUrls, QString completedUrl, QString errStr, int count)
{
    if (errStr.size())
        emit listsChanged(completedUrl + " " + errStr, -1);
    else
        emit listsChanged(completedUrl, count);

    history << completedUrl;

    if (history.size() == maxLinks)
    {
        stopParsing();
        return;
    }

    for (int i = 0; i < newUrls.size(); ++i)
    {
        if (!history.contains(newUrls[i]))
            queue << newUrls[i];
    }

    for (int i = 0; i < vecThread.size(); ++i)
    {
        if (vecPageParser[i]->isReady)
        {
            if (!queue.empty())
            {
                emit setTaskForThread(i, queue.front());
                queue.pop_front();
            }
            else
                break;
        }
    }
}


