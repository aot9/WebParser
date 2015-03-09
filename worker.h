#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include <QString>
#include <QSet>

#include "linkqueue.h"

class PageParser;
class QThread;

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    
signals:
    void listsChanged(QString, int);

    void setTaskForThread(unsigned int, QString);
    void workerStopped();
    void workerPaused();

public slots:
    void runParsing(QString aUrl, QString aText, int aThreadNum, int aLinkNum);
    void onPageParsed(QString aCompletedUrl, QString aErrStr, int aMatchNum);
    void stopParsing();
    void onPause();
    void onResume();

private:
    QVector<QThread*> m_vecThread;
    QVector<PageParser*> m_vecPageParser;

    LinkQueue m_queue;

    int m_maxLinkNum;
    int m_activeTaskCount;
    int m_completed;
    bool m_needPause;
};

#endif // WORKER_H
