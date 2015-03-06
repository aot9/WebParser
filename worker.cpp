#include <QThread>

#include "worker.h"
#include "pageparser.h"

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}

void Worker::runParsing(QString aUrl, QString aText, int aThreadNum, int aLinkNum)
{
    m_maxLinkNum = aLinkNum;

    for (int i = 0; i < aThreadNum; ++i)
    {
        m_vecThread.push_back(new QThread);
        m_vecPageParser.push_back(new PageParser(i, aText));

        QObject::connect(m_vecPageParser[i], SIGNAL(finishedParsing(QStringList, QString, QString, int)),
                         this,             SLOT(onPageParsed(QStringList, QString, QString, int)));

        QObject::connect(this,             SIGNAL(setTaskForThread(uint, QString)),
                         m_vecPageParser[i], SLOT(parseUrl(uint, QString)));

        m_vecPageParser[i]->moveToThread(m_vecThread[i]);

        m_vecThread[i]->start();
    }

    emit setTaskForThread(0, aUrl);
}

void Worker::stopParsing()
{
    m_queue.clear();
    m_history.clear();

    for (int i = 0; i < m_vecThread.size(); ++i)
    {
        m_vecThread[i]->quit();
        m_vecThread[i]->wait();

        m_vecPageParser[i]->deleteLater();
        m_vecThread[i]->deleteLater();
    }

    m_vecPageParser.clear();
    m_vecThread.clear();

    emit workerStopped();
}

void Worker::onPause()
{
    for (int i = 0; i < m_vecThread.size(); ++i)
    {
        m_vecPageParser[i]->m_isPause = true;
        m_vecPageParser[i]->m_isReady = false;
    }
}

void Worker::onResume()
{
    for (int i = 0; i < m_vecThread.size(); ++i)
    {
        m_vecPageParser[i]->m_isPause = false;
        m_vecPageParser[i]->m_isReady = true;
    }

    if (!m_queue.empty())
    {
        emit setTaskForThread(0, m_queue.front());
        m_queue.pop_front();
    }
}

void Worker::onPageParsed(QStringList aNewUrls, QString aCompletedUrl, QString aErrStr, int aMatchNum)
{
    if (aErrStr.size())
        emit listsChanged(aCompletedUrl + " " + aErrStr, -1);
    else
        emit listsChanged(aCompletedUrl, aMatchNum);

    m_history << aCompletedUrl;

    if (m_history.size() == m_maxLinkNum)
    {
        stopParsing();
        return;
    }

    for (int i = 0; i < aNewUrls.size(); ++i)
    {
        if (!m_history.contains(aNewUrls[i]))
            m_queue << aNewUrls[i];
    }

    for (int i = 0; i < m_vecThread.size(); ++i)
    {
        if (m_vecPageParser[i]->m_isReady)
        {
            if (!m_queue.empty())
            {
                emit setTaskForThread(i, m_queue.front());
                m_queue.pop_front();
            }
            else
                break;
        }
    }
}


