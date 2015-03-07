#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include <QString>
#include <QSet>

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
    void onPageParsed(QStringList aNewUrls, QString aCompletedUrl, QString aErrStr, int aMatchNum);
    void stopParsing();
    void onPause();
    void onResume();

private:
    QVector<QThread*> m_vecThread;
    QVector<PageParser*> m_vecPageParser;

    QStringList m_queue;
    QSet<QString> m_history;
    //QStringList m_history;

    int m_maxLinkNum;
    int m_activeTaskCount;
    bool m_needPause;
};

#endif // WORKER_H
