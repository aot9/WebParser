#include "pageparser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QRegExp>
#include <QStringList>
#include <QThread>

PageParser::PageParser(uint aId, QString aText) :
    QObject(NULL),
    m_pNetManager(NULL),
    m_text(aText),
    m_isReady(true),
    m_isPause(false),
    m_threadId(aId)
{
}

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
        QRegExp re("href\\s*=\\s*\"(http://[^\"\' ]*)\"|(" + m_text + ")", Qt::CaseInsensitive);

        QString replyStr = aReply->readAll();

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
