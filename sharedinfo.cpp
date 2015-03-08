#include "sharedinfo.h"


SharedInfo::SharedInfo()
{
}

// History
bool SharedInfo::isInHistory(const QString aUrl)
{
    bool result;
    m_completedMutex.lock();
    result  = m_completed.contains(aUrl);
    m_completedMutex.unlock();

    return result;
}

void SharedInfo::addToHistory(const QString aUrl)
{
    m_completedMutex.lock();
    m_completed.insert(aUrl);
    m_completedMutex.unlock();
}

int SharedInfo::getHistorySize()
{
    int result;
    m_completedMutex.lock();
    result  = m_completed.size();
    m_completedMutex.unlock();

    return result;
}

// Progress
bool SharedInfo::isInProgress(const QString aUrl)
{
    bool result;
    m_inProgressMutex.lock();
    result  = m_inProgress.contains(aUrl);
    m_inProgressMutex.unlock();

    return result;
}

void SharedInfo::addToProgress(const QString aUrl)
{
    m_inProgressMutex.lock();
    m_inProgress.insert(aUrl);
    m_inProgressMutex.unlock();
}

void SharedInfo::removeFromProgress(const QString aUrl)
{
    m_inProgressMutex.lock();
    m_inProgress.remove(aUrl);
    m_inProgressMutex.unlock();
}

int SharedInfo::getInProgressSize()
{
    int result;
    m_inProgressMutex.lock();
    result = m_inProgress.size();
    m_inProgressMutex.unlock();

    return result;
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

void SharedInfo::pushToQueue(const QString aUrl)
{
    m_queueMutex.lock();
    m_queue.enqueue(aUrl);
    m_queueMutex.unlock();
}

QString SharedInfo::getFromQueue()
{
    QString url;
    m_queueMutex.lock();
    url = m_queue.dequeue();
    m_queueMutex.unlock();

    return url;
}

void SharedInfo::clear()
{
    m_queue.clear();
    m_completed.clear();
    m_inProgress.clear();
}

