#include "pageparser.h"
#include <iostream>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QThread>

PageParser::PageParser(uint id, QString text) :
    QObject(NULL),
    nm(NULL),
    m_text(text),
    isReady(true),
    threadId(id)
{
}

void PageParser::processReply(QNetworkReply* reply)
{

    QString requestUrl = reply->request().url().toString();
    if ((int)reply->error())
    {
        isReady = true;
        QString errString = reply->errorString();
        reply->deleteLater();
        emit finishedParsing(m_queue, requestUrl + " " + errString, 0);
    }
    else
    {
        int matchesFound = 0;
        QRegExp re("href\\s*=\\s*\"(http://[^\"\' ]*)\"|(" + m_text + ")", Qt::CaseInsensitive);

        QString replyStr = reply->readAll();

        int pos = 0;
        while ((pos = re.indexIn(replyStr, pos, QRegExp::CaretWontMatch)) != -1)
        {
            QString url = re.cap(1);
            QString tex = re.cap(2);

            if (url.size())
                m_queue.append(url);
            if (tex.size())
                ++matchesFound;

            pos += re.matchedLength();
        }
        isReady = true;
        reply->deleteLater();
        emit finishedParsing(m_queue, requestUrl, matchesFound);
    }  
}

void PageParser::parseUrl(uint tid, QString url)
{
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
