#include "sharedinfo.h"

SharedInfo::SharedInfo()
{
}

// History
bool SharedInfo::isInHistory(const QString aUrl)
{
    bool result;
    m_historyMutex.lock();
    result = m_history.contains(aUrl);
    m_historyMutex.unlock();

    return result;
}

void SharedInfo::addToHistory(const QString aUrl)
{
    m_historyMutex.lock();
    m_history.insert(aUrl);
    m_historyMutex.unlock();
}

// Queue
bool SharedInfo::isQueueEmpty()
{
    bool result;
    m_queueMutex.lock();
    result = m_queue.empty();
    m_queueMutex.unlock();

    return result;
}

void SharedInfo::enqueue(const QString aUrl)
{
    m_queueMutex.lock();
    m_queue.enqueue(aUrl);
    m_queueMutex.unlock();
}

QString SharedInfo::dequeue()
{
    QString url;
    m_queueMutex.lock();
    url = m_queue.dequeue();
    m_queueMutex.unlock();

    return url;
}

void SharedInfo::clear()
{
    m_queueMutex.lock();
    m_queue.clear();
    m_queueMutex.unlock();

    m_historyMutex.lock();
    m_history.clear();
    m_historyMutex.unlock();
}

