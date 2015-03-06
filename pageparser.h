#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include <QStringList>

class QNetworkReply;
class QNetworkAccessManager;

class PageParser : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager* m_pNetManager;
    QStringList m_queue;
    QString m_text;
    uint m_threadId;

public:
    QAtomicInt m_isReady;
    QAtomicInt m_isPause;

public:
    explicit PageParser(QObject *parent = 0);
    PageParser(uint aId, QString aText);

signals:
    void finishedParsing(QStringList, QString, QString, int);

public slots:
    void processReply(QNetworkReply* reply);
    void parseUrl(uint tid, QString url);

};

#endif // PAGEPARSER_H
