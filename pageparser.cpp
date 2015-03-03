#include "pageparser.h"
#include <iostream>
#include <QDebug>
#include <QRegExp>
#include <QStringList>

PageParser::PageParser(QObject *parent)
{
}

void PageParser::processReply(QNetworkReply* reply)
{
    if ((int)reply->error())
        std::cout << reply->errorString().toStdString() << std::endl;
    else
    {
        QRegExp re("href\\s*=\\s*\"(http://[^\"\' ]*)\"", Qt::CaseInsensitive);
        QString replyStr = reply->readAll();
        int pos = 0;
        while ((pos = re.indexIn(replyStr, pos, QRegExp::CaretWontMatch)) != -1)
        {
            std::cout << re.cap(1).toStdString() << std::endl;
            pos += re.matchedLength();
        }
    }
}

void PageParser::run()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(processReply(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("testPage.html")));
}
