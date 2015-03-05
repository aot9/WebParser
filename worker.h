#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QStringList>

class PageParser;

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    
signals:
    void listsChanged(QString, int);

    void setTaskForThread(unsigned int, QString);
    void workerFinished();
public slots:
    void runParsing(QString url, QString text, int nt, int nl);
    void onPageParsed(QStringList newUrls, QString completedUrl, int count);
private:
    QThread** arrThread;
    PageParser** arrPageParser;

    QStringList queue;
    QStringList history;

    int maxThreads;
    int maxLinks;
    bool needCleanup;

public:
    QAtomicInt pause, stop;
};

#endif // WORKER_H
