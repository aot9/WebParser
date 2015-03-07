#include "pageparser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QRegExp>
#include <QStringList>
#include <QThread>
#include <iostream>

#define URL_PATTERN "href\\s*=\\s*\"(http://[^\"\' ]*)\""

PageParser::PageParser(uint aId, QString aText) :
    QObject(NULL),
    m_isReady(true),
    m_isPause(false),
    m_pNetManager(NULL),
    m_text(aText),
    m_threadId(aId),
    m_re(URL_PATTERN"|("+aText+")",  Qt::CaseInsensitive)
{}

void PageParser::processReply(QNetworkReply* aReply)
{
    QString requestUrl = aReply->request().url().toString();
    QString errString;
    int matchesFound = 0;

    if ((int)aReply->error())
    {
        errString = aReply->errorString();
    }
    else
    {
        QString replyStr = aReply->readAll();

        int pos = 0;
        while ((pos = m_re.indexIn(replyStr, pos, QRegExp::CaretWontMatch)) != -1)
        {
            QString url = m_re.cap(1);
            QString tex = m_re.cap(2);

            if (url.size())
                m_queue.append(url);
            if (tex.size())
                ++matchesFound;

            pos += m_re.matchedLength();
        }
    }

    m_isReady = true;
    aReply->deleteLater();

    emit finishedParsing(m_queue, requestUrl, errString, matchesFound);
}

void PageParser::parseUrl(uint aThreadId, QString aUrl)
{
    if (aThreadId != m_threadId || m_isPause)
        return;

    m_isReady = false;

    m_queue.clear();

    if (m_pNetManager == NULL)
    {
        m_pNetManager = new QNetworkAccessManager();
        QObject::connect(m_pNetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processReply(QNetworkReply*)));
        QObject::connect(this->thread(), SIGNAL(finished()), m_pNetManager, SLOT(deleteLater()));
    }

    m_pNetManager->get(QNetworkRequest(aUrl));
}
