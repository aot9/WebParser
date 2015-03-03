#include "pageparser.h"
#include <iostream>
#include <QDebug>


PageParser::PageParser(QObject *parent)
{
    std::cout << "parser ctor\n";
}


void PageParser::replyFinished(QNetworkReply* reply)
{
    if ((int)reply->error())
        std::cout << reply->errorString().toStdString() << std::endl;
    else
        std::cout << QString(reply->readAll()).toStdString() << std::endl;
}

void PageParser::run()
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://abrbr.ru"));
    request.setRawHeader("User-Agent", "WebParser");
    //request.setRawHeader("Content-Language", "en, ru");
    //request.setRawHeader("Accept-Language", "en, ru");
    //request.setRawHeader("Content-Type", "text/html;charset=utf-8");

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(request);
}
