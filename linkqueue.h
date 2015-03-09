#ifndef SHAREDINFO_H
#define SHAREDINFO_H

#include <QSet>
#include <QQueue>
#include <QString>
#include <QMutex>

class LinkQueue
{
public:
    LinkQueue();
private:
    QSet<QString> m_history;
    QQueue<QString> m_queue;

    QMutex m_mutex;

public:
    void enqueue(const QString aUrl);
    QString dequeue();

    bool empty();
    void clear();
};

#endif // SHAREDINFO_H
