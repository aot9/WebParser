#include "worker.h"
#include "pageparser.h"
#include <iostream>
//#define NT 5

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}

void Worker::runParsing(QString url, QString text, int nt, int nl)
{
    pause = false;
    needCleanup = true;

    maxThreads = nt;
    maxLinks = nl;

    arrThread = new QThread*[maxThreads];
    arrPageParser = new PageParser*[maxThreads];

    for (int i = 0; i < maxThreads; ++i)
    {
        arrThread[i] = new QThread;
        arrPageParser[i] = new PageParser(i, text);

        QObject::connect(arrPageParser[i], SIGNAL(finishedParsing(QStringList, QString, int)), this, SLOT(onPageParsed(QStringList, QString, int)));
        QObject::connect(this, SIGNAL(setTaskForThread(uint, QString)), arrPageParser[i], SLOT(parseUrl(uint, QString)));

        arrPageParser[i]->moveToThread(arrThread[i]);

        arrThread[i]->start();
    }

    //arrPageParser[0]->parseUrl(url);
    emit setTaskForThread(0, url);
}

void Worker::onPageParsed(QStringList newUrls, QString completedUrl, int count)
{
    emit listsChanged(completedUrl, count);

    history << completedUrl;

    while(pause == 1)
        sleep(1);

    if (history.size() == maxLinks || stop == 1)
    {
        if(needCleanup)
        {
            queue.clear();
            history.clear();
            for (int i = 0; i < maxThreads; ++i)
            {
                arrThread[i]->quit();
                arrThread[i]->wait();

                arrPageParser[i]->deleteLater();
                arrThread[i]->deleteLater();
            }
            delete[] arrThread;
            delete[] arrPageParser;

            needCleanup = false;
            stop = 0;

            emit workerFinished();
        }

        return;
    }


    for (int i = 0; i < newUrls.size(); ++i)
    {
        if (!history.contains(newUrls[i]))
            queue << newUrls[i];
    }

    for (int i = 0; i < maxThreads; ++i)
    {
        if (arrPageParser[i]->isReady)
        {
            if (!queue.empty())
            {
                //arrPageParser[i]->parseUrl(queue.front());
                emit setTaskForThread(i, queue.front());
                queue.pop_front();
            }
            else
                break;
        }
    }
}


