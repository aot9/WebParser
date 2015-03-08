#include <QThread>

#include "worker.h"
#include "pageparser.h"

#include <iostream>

Worker::Worker(QObject *parent) :
    QObject(parent), m_needPause(false), m_needStop(false), m_completed(0)
{
}

void Worker::runParsing(QString aUrl, QString aText, int aThreadNum, int aLinkNum)
{
    m_maxLinkNum = aLinkNum;
    m_completed = 0;
    m_info.clear();

    for (int i = 0; i < aThreadNum; ++i)
    {
        m_vecThread.push_back(new QThread);
        m_vecPageParser.push_back(new PageParser(i, aText, m_info));

        QObject::connect(m_vecPageParser[i], SIGNAL(finishedParsing(QString, QString, int)),
                         this,             SLOT(onPageParsed(QString, QString, int)));

        QObject::connect(this,             SIGNAL(setTaskForThread(uint, QString)),
                         m_vecPageParser[i], SLOT(parseUrl(uint, QString)));

        m_vecPageParser[i]->moveToThread(m_vecThread[i]);

        m_vecThread[i]->start();
    }

    m_info.addToProgress(aUrl);
    emit setTaskForThread(0, aUrl);
}

void Worker::stopParsing()
{
    for (int i = 0; i < m_vecThread.size(); ++i)
    {
        m_vecThread[i]->quit();
        m_vecThread[i]->wait();

        m_vecPageParser[i]->deleteLater();
        m_vecThread[i]->deleteLater();
    }

    m_vecPageParser.clear();
    m_vecThread.clear();

    m_needPause = false;
    m_needStop = false;

    emit workerStopped();
}

void Worker::onPause()
{
    for (int i = 0; i < m_vecThread.size(); ++i)
    {
        m_vecPageParser[i]->m_isPause = true;
        m_vecPageParser[i]->m_isReady = false;
    }

    if (m_info.getInProgressSize() == 0)
        emit workerPaused();
    else
        m_needPause = true;
}

void Worker::onResume()
{
    for (int i = 0; i < m_vecThread.size(); ++i)
    {
        m_vecPageParser[i]->m_isPause = false;
        m_vecPageParser[i]->m_isReady = true;
    }

    if (!m_info.isQueueEmpty())
        emit setTaskForThread(0, m_info.getFromQueue());
}

void Worker::onPageParsed(QString aCompletedUrl, QString aErrStr, int aMatchNum)
{
    ++m_completed;
    m_info.removeFromProgress(aCompletedUrl);

    if (aErrStr.size())
        emit listsChanged(aCompletedUrl + " " + aErrStr, -1);
    else
        emit listsChanged(aCompletedUrl, aMatchNum);

    //std::cout << m_info.getInProgressSize() << std::endl;
    if (m_needPause)
    {
        if (m_info.getInProgressSize() == 0)
        {
            m_needPause = false;
            emit workerPaused();
        }
        return;
    }

    if (m_needStop)
    {
        if (m_info.getInProgressSize() == 0)
            stopParsing();
        return;
    }

    for (int i = 0; i < m_vecThread.size(); ++i)
    {
        if (m_vecPageParser[i]->m_isReady)
        {
            if (m_info.isQueueEmpty())
                break;
            else
            {
                if (m_completed + m_info.getInProgressSize() == m_maxLinkNum)
                {
                    m_needStop = true;
                    break;
                }
                else
                {
                    QString url = m_info.getFromQueue();
                    emit setTaskForThread(i, url);
                    m_info.addToProgress(url);
                }
                std::cout << m_info.getInProgressSize() << std::endl;
            }
        }
    }

}


