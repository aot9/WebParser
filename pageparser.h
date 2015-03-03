#ifndef PAGEPARSER_H
#define PAGEPARSER_H

#include <QRunnable>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class PageParser : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit PageParser(QObject *parent = 0);
    virtual void run();
signals:
    
public slots:
    void replyFinished(QNetworkReply* reply);
    
};

#endif // PAGEPARSER_H
