#ifndef CHARTMAININTERFACE_H
#define CHARTMAININTERFACE_H

#include <QWidget>
#include <QTimer>
#include <QHostInfo>

namespace Ui {
class ChartMainInterface;
}
enum DataType
{
    JsonFile = 1,
    Url,
    Bytearray
};
class ChartMainInterface : public QWidget
{
    Q_OBJECT

public:
    explicit ChartMainInterface(QWidget *parent = nullptr);
    ~ChartMainInterface();
public:

protected:
    bool loadPicture(QByteArray data);               //加载图片

    DataType checkDataType(QByteArray data);         //检查收到数据内容

    void checkNetWorkOnline();                       //检测当前网络是否可用



public slots:
     void onRecvMsg(QByteArray data, QObject * obj); //当收到消息时的槽函数

     void onAutoUpdateSlot();                        //自动定时更新槽函数

     void onAutoDisplaySlot();                       //自动轮播图片槽函数

     void onLookupHost(QHostInfo host);              //查询网络连接状态槽函数

private:
    Ui::ChartMainInterface *ui;
    QVector<QByteArray> m_PicVector;                 //图片容器
    int m_ReConnectionCounts = 0;                    //重连次数
    bool m_CurrentNetConnectionState = true;         //当前网络连接状态
    QTimer * m_AtuoUpdatePicTimer;                   //自动获取图片定时器
    QTimer * m_AtuoDisplayPicTimer;                  //自动轮播图片定时器

};

#endif // CHARTMAININTERFACE_H
