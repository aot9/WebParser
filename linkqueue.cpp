#include "linkqueue.h"

LinkQueue::LinkQueue()
{}

bool LinkQueue::empty()
{
    m_mutex.lock();

    bool result = m_queue.empty();

    m_mutex.unlock();

    return result;
}

void LinkQueue::enqueue(const QString aUrl)
{
    m_mutex.lock();

    if (!m_history.contains(aUrl))
    {
        m_history.insert(aUrl);
        m_queue.enqueue(aUrl);
    }

    m_mutex.unlock();
}

QString LinkQueue::dequeue()
{
    m_mutex.lock();

    QString url = m_queue.dequeue();

    m_mutex.unlock();

    return url;
}

void LinkQueue::clear()
{
    m_mutex.lock();

    m_history.clear();
    m_queue.clear();

    m_mutex.unlock();

}

