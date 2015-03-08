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
    QSet<QString> m_completed;
    QSet<QString> m_inProgress;
    QQueue<QString> m_queue;

    QMutex m_completedMutex;
    QMutex m_inProgressMutex;
    QMutex m_queueMutex;

public:
    bool isInHistory(const QString aUrl);
    void addToHistory(const QString aUrl);
    int getHistorySize();

    bool isInProgress(const QString aUrl);
    void addToProgress(const QString aUrl);
    void removeFromProgress(const QString aUrl);
    int getInProgressSize();

    bool isQueueEmpty();
    void pushToQueue(const QString aUrl);
    QString getFromQueue();

    void clear();
};

#endif // SHAREDINFO_H
