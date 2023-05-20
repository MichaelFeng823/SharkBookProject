#ifndef CHARTMAININTERFACE_H
#define CHARTMAININTERFACE_H

#include <QWidget>
#include <QTimer>
#include <QHostInfo>
#include <QLabel>
#include "BaseClass/basecustomwidget.h"
#include <Main/ChartPage/SubPage/photoframe.h>

namespace Ui {
class ChartMainInterface;
}
enum DataType
{
    JsonFile = 1,
    Url,
    Bytearray
};
class ChartMainInterface : public BaseCustomWidget
{
    Q_OBJECT

public:
    explicit ChartMainInterface(QWidget *parent = nullptr);
    ~ChartMainInterface();

protected:
    bool amplifyloadPicture();                       //双击放大或缩小加载图片
    DataType checkDataType(QByteArray data);         //检查收到数据内容
    void checkNetWorkOnline();                       //检测当前网络是否可用
public slots:
     void onRecvMsg(QByteArray data, QObject * obj); //当收到消息时的槽函数
     void onAutoUpdateSlot();                        //自动定时更新槽函数
     void onLookupHost(QHostInfo host);              //查询网络连接状态槽函数

private:
    Ui::ChartMainInterface *ui;
    QVector<QByteArray> m_PicVector;                 //图片容器
    QByteArray m_CurrentPic;                         //当前图片数据
    QSize m_CurrentPicSize;                          //当前图片size
    int m_ReConnectionCounts = 0;                    //重连次数
    bool m_CurrentNetConnectionState = true;         //当前网络连接状态
    QTimer * m_AtuoUpdatePicTimer;                   //自动获取图片定时器
    PhotoFrame * photo;                              //照片类
    static int m_DefaultWidth;                       //默认图片宽度
    const int MaxReconnection = 10;
    const int MaxStorePics = 20;
};

#endif // CHARTMAININTERFACE_H
