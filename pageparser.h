#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include <QStringList>
#include <QRegExp>
#include "sharedinfo.h"

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
    uint m_threadId;
    QRegExp m_re;
    SharedInfo& m_infoRef;

public:
    explicit PageParser(QObject *parent = 0);
    PageParser(uint aId, QString aText, SharedInfo& aInfo);

signals:
    void finishedParsing(QString, QString, int);

public slots:
    void processReply(QNetworkReply* reply);
    void parseUrl(uint tid, QString url);

};

#endif // PAGEPARSER_H
