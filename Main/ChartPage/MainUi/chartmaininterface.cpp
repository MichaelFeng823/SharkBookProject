#include "chartmaininterface.h"
#include "ui_chartmaininterface.h"
#include <QImage>
#include <QPixmap>
#include <QPicture>
#include <QPointer>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <Kit/LogInfo/clog.h>
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include "NetCore/msgqueue.h"


ChartMainInterface::ChartMainInterface(QWidget *parent) :
    BaseCustomWidget(parent),
    ui(new Ui::ChartMainInterface)
{
    ui->setupUi(this);
    //ui->label->setScaledContents(true);
    m_AtuoUpdatePicTimer = new QTimer;
    m_AtuoUpdatePicTimer->start();


    m_AtuoDisplayPicTimer = new QTimer;
    m_AtuoDisplayPicTimer->start(200);

    connect(m_AtuoDisplayPicTimer,&QTimer::timeout,this,&ChartMainInterface::onAutoDisplaySlot);
    connect(m_AtuoUpdatePicTimer,&QTimer::timeout,this,&ChartMainInterface::checkNetWorkOnline);
    connect(MSGQUEUE, SIGNAL(SignalRecvMsg(QByteArray,QObject *)), this, SLOT(onRecvMsg(QByteArray, QObject *)));
}

ChartMainInterface::~ChartMainInterface()
{
    delete ui;
}
//自动定时更新槽函数
void ChartMainInterface::onAutoUpdateSlot()
{
    if(isActiveWindow()){
        if(SYSTEMSTATE::NETWORKSTATE){
            if(m_PicVector.size() > MaxStorePics)
                return;
            TMsgItem item;
            //item.strUrl = QString("https://api.oick.cn/random/pe/28.jpg");
            item.strUrl = QString("https://www.mxnzp.com/api/image/girl/list/random?app_id=hxmmwohonjnjepw8&app_secret=OHdUUWZORk4vdkg5bnVGcmF1T1I5dz09");
            item.pObj = this;
            MSGQUEUE->Push(item);
        }
        else{
            QMessageBox::warning(0,"警告","无网络使用权限");
        }
         m_AtuoUpdatePicTimer->setInterval(25000);
    }
}
//自动轮播图片槽函数
void ChartMainInterface::onAutoDisplaySlot()
{
    if(isActiveWindow()){
        if(!m_PicVector.isEmpty()){
            //LOG("加载显示图片");
            LOG("当前容器图片数量:%d",m_PicVector.size());
            if(!loadPicture(m_PicVector.first())){
                 m_AtuoDisplayPicTimer->setInterval(1000);
                 m_PicVector.pop_front();
                 return;
            }
            m_AtuoDisplayPicTimer->setInterval(3000);
            if(m_PicVector.size() > 1)
                m_PicVector.pop_front();
        }
        else{
            LOG("当前容器里没有图片");
        }
    }
    else{
        //LOG("当前Window is not Active");
    }

}
//当收到消息时的槽函数
void ChartMainInterface::onRecvMsg(QByteArray data, QObject * obj)
{
    if(obj != this)
        return;
    //请求返回的结果
    QString datastr = QString::fromLocal8Bit(data);
    LOG("datasize:%d",data.size());
    DataType type = checkDataType(data);
    if(type == DataType::Url)
        return;
    else if(type == DataType::Bytearray)
        m_PicVector.append(data);
}
//加载图片
bool ChartMainInterface::loadPicture(QByteArray data)
{
    QPixmap pix;
    bool state = pix.loadFromData(data);
    if(state)
        ui->label->setPixmap(pix.scaledToWidth(ui->label->width()));
    //LOG("state:%d",state);
    return state;
}
//检查收到数据内容
DataType ChartMainInterface::checkDataType(QByteArray data)
{
    QString recv = QString::fromLocal8Bit(data);

    QString content;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);
    QString msg;
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        LOG("recv:%s",recv.toStdString().c_str());
        if (doucment.isObject()){
            QJsonObject object = doucment.object();
            msg= object.value("msg").toString();
            if(object.contains("data")){
              QJsonValue data_value = object.value("data");
              if(data_value.isArray()){
                  QJsonArray array = data_value.toArray();
                  for(int i = 0; i < array.size(); i++){
                      QJsonObject imageobject = array.at(i).toObject();
                      QString imageUrl = imageobject.value("imageUrl").toString();
                      QString imageSize = imageobject.value("imageSize").toString();
                      int imageFileLength = imageobject.value("imageFileLength").toInt();
                      LOG("imageUrl:%s",imageUrl.toStdString().c_str());
                      //LOG("imageSize:%s",imageSize.toStdString().c_str());
                      //LOG("imageFileLength:%d",imageFileLength);
                      TMsgItem item;
                      item.strUrl = imageUrl;
                      item.pObj = this;
                      MSGQUEUE->Push(item);
                  }
                  return DataType::Url;
              }
           }
        }
        else{
            return DataType::Bytearray;
        }
    }
    return DataType::Bytearray;
}

void ChartMainInterface::checkNetWorkOnline()
{
    QHostInfo::lookupHost("www.baidu.com",this,SLOT(onLookupHost(QHostInfo)));
}

void ChartMainInterface::onLookupHost(QHostInfo host)
{
    if(m_ReConnectionCounts > MaxReconnection){
        m_AtuoUpdatePicTimer->stop();
        m_AtuoDisplayPicTimer->stop();
        return;
    }
    if (host.error() != QHostInfo::NoError) {
        m_AtuoUpdatePicTimer->stop();
        LOG("当前无网络连接，请检测你手机的网络情况");
        m_CurrentNetConnectionState = false;
        m_ReConnectionCounts ++;
        if(MSGQUEUE->isRunning())
            MSGQUEUE->quit();
        LOG("重连次数:%d",m_ReConnectionCounts);
    }
    else{
        MSGQUEUE->start();
        m_CurrentNetConnectionState = true;
        m_ReConnectionCounts = 0;
        onAutoUpdateSlot();
    }
}



