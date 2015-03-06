#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include <QStringList>

class QNetworkReply;
class QNetworkAccessManager;

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
    QAtomicInt isPause;

public:
    explicit PageParser(QObject *parent = 0);
    PageParser(uint id, QString text);
signals:
    void finishedParsing(QStringList, QString, QString, int);

public slots:
    void processReply(QNetworkReply* reply);
    void parseUrl(uint tid, QString url);

};

#endif // PAGEPARSER_H
