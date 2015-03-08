#ifndef SHAREDINFO_H
#define SHAREDINFO_H

#include <QSet>
#include <QQueue>
#include <QString>
#include <QMutex>

class SharedInfo
{
public:
    SharedInfo();
private:
    QSet<QString> m_history;
    QQueue<QString> m_queue;

    QMutex m_historyMutex;
    QMutex m_queueMutex;

public:
    bool isInHistory(const QString aUrl);
    void addToHistory(const QString aUrl);

    bool isQueueEmpty();
    void enqueue(const QString aUrl);
    QString dequeue();

    void clear();
};

#endif // SHAREDINFO_H
