#include "msgqueue.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QPointer>
#include <Util/clog.h>

MsgQueue* MsgQueue::m_pMsgQueue = NULL;
MsgQueue::MsgQueue( QObject *parent ) : QThread(parent)
{

}

MsgQueue::~MsgQueue()
{
    Clear();
}

MsgQueue *MsgQueue::GetInstance()
{
    if (NULL == m_pMsgQueue)
    {
        m_pMsgQueue = new MsgQueue();
        m_pMsgQueue->start();
    }

    return m_pMsgQueue;
}

void MsgQueue::ExitInstance()
{
    if (m_pMsgQueue != NULL)
    {
        m_pMsgQueue->requestInterruption();
        m_pMsgQueue->wait();

        delete m_pMsgQueue;
        m_pMsgQueue = NULL;
    }
}

bool MsgQueue::Push(TMsgItem item)
{
    QMutexLocker locker(&m_mutex);
    LOG("Push item");
    m_listMsg.append(item);

    return true;
}

void MsgQueue::run()
{
    while (!isInterruptionRequested() /*TRUE*/)
    {
        bool hasMsg = false;
        {
            QMutexLocker locker(&m_mutex);
            if (!m_listMsg.isEmpty())
            {
                hasMsg = true;

                QPointer<QNetworkAccessManager>m_pHttpMgr = new QNetworkAccessManager();
                QNetworkRequest requestInfo;
                requestInfo.setUrl(QUrl(m_listMsg.at(0).strUrl));

                QEventLoop eventLoop;
                QNetworkReply *reply =  m_pHttpMgr->get(requestInfo);
                connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
                eventLoop.exec();       // block until finish
                if (reply->error() == QNetworkReply::NoError)
                {
                    LOG("request protobufHttp NoError");
                }
                else
                {
                    LOG("request protobufHttp handle errors here");
                    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                    //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
                    LOG( "request protobufHttp found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
                    LOG("%s",reply->errorString().toStdString().c_str());
                }

                //请求返回的结果
                QByteArray responseByte = reply->readAll();
                emit SignalRecvMsg(responseByte, m_listMsg.at(0).pObj);
                m_listMsg.removeFirst();
            }
        }

        if (!hasMsg)
        {
            msleep(200);
        }
    }
}

void MsgQueue ::Clear()
{
    QMutexLocker locker(&m_mutex);

    if (!m_listMsg.isEmpty())
    {
        m_listMsg.clear();
    }
}


