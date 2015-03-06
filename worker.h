#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QVector>
#include <QString>
#include <QSet>

class PageParser;

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    

signals:
    void listsChanged(QString, int);

    void setTaskForThread(unsigned int, QString);
    void workerStopped();

public slots:
    void runParsing(QString url, QString text, int nt, int nl);
    void onPageParsed(QStringList newUrls, QString completedUrl, QString errStr, int count);
    void stopParsing();
    void onPause();
    void onResume();

private:
    //QThread** arrThread;
    //PageParser** arrPageParser;

    QVector<QThread*> vecThread;
    QVector<PageParser*> vecPageParser;

    QStringList queue;
    QSet<QString> history;

    int maxLinks;

public:
    QAtomicInt pause, stop;
};

#endif // WORKER_H
