#include "pageparser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QThread>

PageParser::PageParser(uint id, QString text) :
    QObject(NULL),
    nm(NULL),
    m_text(text),
    isReady(true),
    isPause(false),
    threadId(id)
{
}

void PageParser::processReply(QNetworkReply* reply)
{
    QString requestUrl = reply->request().url().toString();
    QString errString;
    int matchesFound = 0;

    if ((int)reply->error())
    {
        errString = reply->errorString();
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

            if (url.size())
                m_queue.append(url);
            if (tex.size())
                ++matchesFound;

            pos += re.matchedLength();
        }
    }

    isReady = true;
    reply->deleteLater();

    emit finishedParsing(m_queue, requestUrl, errString, matchesFound);
}

void PageParser::parseUrl(uint tid, QString url)
{
    if (tid != threadId || isPause)
        return;

    isReady = false;

    m_queue.clear();

    if (nm == NULL)
    {
        nm = new QNetworkAccessManager();
        QObject::connect(nm, SIGNAL(finished(QNetworkReply*)), this, SLOT(processReply(QNetworkReply*)));
        QObject::connect(this->thread(), SIGNAL(finished()), nm, SLOT(deleteLater()));
    }

    nm->get(QNetworkRequest(url));
}
