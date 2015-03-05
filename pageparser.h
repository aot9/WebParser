#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include <QRunnable>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStringList>

class Worker;

class PageParser : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager* nm;
    QStringList m_queue;
    QString m_text;
    uint threadId;

public:
    QAtomicInt isReady;

public:
    explicit PageParser(QObject *parent = 0);
    PageParser(uint id, QString text);
signals:
    void finishedParsing(QStringList, QString, int);

public slots:
    void processReply(QNetworkReply* reply);
    void parseUrl(uint tid, QString url);
    
};

#endif // PAGEPARSER_H
