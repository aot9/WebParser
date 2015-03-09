#include <QThread>

#include "worker.h"
#include "pageparser.h"

Worker::Worker(QObject *parent) :
    QObject(parent), m_completed(0), m_needPause(false)
{
}

void Worker::runParsing(QString aUrl, QString aText, int aThreadNum, int aLinkNum)
{
    m_completed = 0;
    m_queue.clear();

    m_maxLinkNum = aLinkNum;

    for (int i = 0; i < aThreadNum; ++i)
    {
        m_vecThread.push_back(new QThread);
        m_vecPageParser.push_back(new PageParser(i, aText, m_queue));

        QObject::connect(m_vecPageParser[i], SIGNAL(finishedParsing(QString, QString, int)),
                         this, SLOT(onPageParsed(QString, QString, int)));

        QObject::connect(this, SIGNAL(setTaskForThread(uint, QString)),
                         m_vecPageParser[i], SLOT(parseUrl(uint, QString)));

        m_vecPageParser[i]->moveToThread(m_vecThread[i]);

        m_vecThread[i]->start();
    }

    m_queue.enqueue(aUrl);
    emit setTaskForThread(0, m_queue.dequeue());
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

    emit workerStopped();
}

void Worker::onPause()
{
    m_activeTaskCount = 0;
    for (int i = 0; i < m_vecThread.size(); ++i)
    {
        if (!m_vecPageParser[i]->m_isReady)
            ++m_activeTaskCount;

        m_vecPageParser[i]->m_isPause = true;
        m_vecPageParser[i]->m_isReady = false;
    }

    if (m_activeTaskCount == 0)
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

    if (!m_queue.empty())
        emit setTaskForThread(0, m_queue.dequeue());
}

void Worker::onPageParsed(QString aCompletedUrl, QString aErrStr, int aMatchNum)
{
    if (m_completed == m_maxLinkNum)
    {
        stopParsing();
        return;
    }
    else
        ++m_completed;

    if (aErrStr.size())
        emit listsChanged(aCompletedUrl + " " + aErrStr, -1);
    else
        emit listsChanged(aCompletedUrl, aMatchNum);

    if (m_needPause)
    {
        if (--m_activeTaskCount == 0)
        {
            m_needPause = false;
            emit workerPaused();
        }
        return;
    }

    for (int i = 0; i < m_vecThread.size(); ++i)
    {
        if (m_vecPageParser[i]->m_isReady)
        {
            if (m_queue.empty())
                break;
            else
                emit setTaskForThread(i, m_queue.dequeue());
        }
    }

}
