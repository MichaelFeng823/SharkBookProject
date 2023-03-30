#ifndef MSGQUEUE_H
#define MSGQUEUE_H

#include <QThread>
#include <QMutex>
///****************************************************************************
/// @author  :
/// @date    :
/// @file    :
/// @brief   : 网络请求消息队列 ----- 单独开个线程执行
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************

struct TMsgItem
{
    QString strUrl;
    QObject *pObj;
};

class MsgQueue : public QThread
{
    Q_OBJECT
public:
    explicit MsgQueue(QObject *parent = 0);
    virtual ~MsgQueue();

public:
    static MsgQueue *GetInstance();
    static void ExitInstance();

    bool Push(TMsgItem item);

protected:
    void run();
    void Clear();

signals:
    void SignalRecvMsg(QByteArray msg, QObject *pObj);

private:
    static MsgQueue *m_pMsgQueue;
    QList<TMsgItem> m_listMsg;
    QMutex m_mutex;
};

#define MSGQUEUE MsgQueue::GetInstance()

#endif // MSGQUEUE_H
