#include "pageparser.h"
#include <iostream>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QThread>

PageParser::PageParser(uint id, QString text) :
    QObject(NULL),
    m_text(text),
    isReady(true),
    threadId(id),
    nm(NULL)
{

}

void PageParser::processReply(QNetworkReply* reply)
{
    int matchesFound = 0;
    QString requestUrl = reply->request().url().toString();
    if ((int)reply->error())
    {
        emit finishedParsing(m_queue, requestUrl + " " + reply->errorString(), matchesFound);
    }
    else
    {
        QRegExp re("href\\s*=\\s*\"(http://[^\"\' ]*)\"|(" + m_text + ")", Qt::CaseInsensitive);

        QString replyStr = reply->readAll();

        int pos = 0;
        while ((pos = re.indexIn(replyStr, pos, QRegExp::CaretWontMatch)) != -1)
        {
            QString url = re.cap(1);
            QString tex = re.cap(2);

            m_queue.append(url);

            if (tex.size())
                ++matchesFound;

            pos += re.matchedLength();
        }
        emit finishedParsing(m_queue, requestUrl, matchesFound);
    }  

    reply->deleteLater();
    isReady = true;
}

void PageParser::parseUrl(uint tid, QString url)
{
    //std::cout << tid <<  " " << threadId << std::endl;
    if (tid != threadId)
        return;

    isReady = false;

    m_queue.clear();

    if (nm == NULL)
    {
        nm = new QNetworkAccessManager();
        QObject::connect(nm, SIGNAL(finished(QNetworkReply*)), this, SLOT(processReply(QNetworkReply*)));
    }
    nm->get(QNetworkRequest(url));
}
