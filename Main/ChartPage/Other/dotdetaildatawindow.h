#ifndef DOTDETAILDATAWINDOW_H
#define DOTDETAILDATAWINDOW_H
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include "Controler/DataStruct/PublicDataStruct.h"
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023-06-05
/// @file    : dotdetaildatawindow.h
/// @brief   : 点的详细数据窗口
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
enum DataState{ NoData = 1, HaveData};
class DotDetailDataWindow:public QWidget
{
    Q_OBJECT
public:
    DotDetailDataWindow(InOrOut type,ChartSelectType charttype,QWidget * parent = nullptr);
    void startTimer();                                             //启动定时器
    void stopTimer();                                              //关闭定时器
    void setData(DotData data);                                    //设置数据
private:
    void paintEvent(QPaintEvent *event) override;                  //重写绘画事件
    void drawThisInNoData(QPainter * painter);                     //绘制没有数据时候的窗体
    void drawTextInNoData(QPainter * painter,QPoint pos);          //绘制文本当没有数据时
    void drawThisInHaveData(QPainter * painter);                   //绘制有数据时候的窗体
    void drawFrameRect(QRect rect,QPainter * painter);             //绘制窗体框架
    void drawTitleRect(QRect rect,QPainter * painter);             //绘制title Rect （最大三比交易）
    void drawTitleText(QPoint pos,QPainter * painter);             //绘制title text
    void drawMessageContent(QPainter *);                                     //绘制最大三比交易
    void drawSingleMessageContent(QRect,MaxThreeMessage,QPainter*);          //绘制单条message content
    void drawLoweTriangleRect(QPainter * painter);                 //绘制下三角当没有数据时
    void drawCirclePic(QPoint pos,QString url,QPainter*painter);   //绘制圆形图片
    void drawDatePayTypeAmountText(QPoint pos,MaxThreeMessage message, QPainter*painter);    //绘制日期支付类型金额文本
    void drawTextInBottom(QPoint pos,DotData data, QPainter * painter);                      //绘制底部的文本
    void drawLowTriangleRectInHaveData(QPainter * painter);       //绘制左侧或者右侧三角当有数据时
    void initTimer();                                              //初始化定时器
    void checkData();                                              //检测数据
    void moveInNoData();                                           //在没有数据时移动
    void moveInHaveData();                                         //在有数据时移动
    QRect checkMessageSize();                                      //检测消息的条数
private:
    DataState m_CurrentState;                                      //当前数据状态
    InOrOut m_Type = InOrOut::Expand;                              //支出还是收入
    ChartSelectType m_ChartSelectType;                             //当前类型
    QTimer * timer;                                                //定时器
    DotData m_Data;                                                //设置数据
    QPoint trianglePos;                                            //下三角位置
    int m_triangleEndPosx;                                         //下三角尾部坐标x
    int m_triangleEndPosy;                                         //下三角尾部坐标y
    QRect mainRect;                                                //主体rect
};

#endif // DOTDETAILDATAWINDOW_H
