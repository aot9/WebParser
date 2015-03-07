#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include <QStringList>
#include <QRegExp>

class QNetworkReply;
class QNetworkAccessManager;

class PageParser : public QObject
{
    Q_OBJECT

public:
    QAtomicInt m_isReady;
    QAtomicInt m_isPause;

private:
    QNetworkAccessManager* m_pNetManager;
    QStringList m_queue;
    uint m_threadId;
    QRegExp m_re;

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
