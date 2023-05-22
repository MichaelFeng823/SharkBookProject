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
#include "Controler/PublicApi/PublicDbFunc.h"
#include <QMouseEvent>
#include <QPainter>
using namespace ScreenFunc;
int ChartMainInterface::m_DefaultWidth = 1080;
ChartMainInterface::ChartMainInterface(QWidget *parent) :
    BaseCustomWidget(parent),
    ui(new Ui::ChartMainInterface)
{
    ui->setupUi(this);
    photo = new PhotoFrame();
    photo->setPicVector(&m_PicVector);
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(photo);
    layout->setMargin(0);
    layout->setSpacing(0);
    ui->backgroundwidget->setLayout(layout);
    m_DefaultWidth = getScreenSize().width();
    m_AtuoUpdatePicTimer = new QTimer;
    m_AtuoUpdatePicTimer->start();
    connect(m_AtuoUpdatePicTimer,&QTimer::timeout,this,&ChartMainInterface::checkNetWorkOnline);
    connect(MSGQUEUE, SIGNAL(SignalRecvMsg(QByteArray,QObject *)), this, SLOT(onRecvMsg(QByteArray, QObject *)));
}

ChartMainInterface::~ChartMainInterface()
{
    delete ui;
}
//设置当前Active状态
void ChartMainInterface::setActiveWindow(bool state)
{
    LOG("ChartMainInterface::setActiveWindow");
    m_IsActiveWindow = state;
    photo->setActiveWindow(state);
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
         m_AtuoUpdatePicTimer->setInterval(10000);
    }
}
//当收到消息时的槽函数
void ChartMainInterface::onRecvMsg(QByteArray data, QObject * obj)
{
    if(obj != this)
        return;
    //请求返回的结果
    QString datastr = QString::fromLocal8Bit(data);
    //LOG("datasize:%d",data.size());
    DataType type = checkDataType(data);
    if(type == DataType::Url)
        return;
    else if(type == DataType::Bytearray)
        m_PicVector.append(data);
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

