#ifndef DOTDETAILDATAWINDOW_H
#define DOTDETAILDATAWINDOW_H
#include <QWidget>
#include <QPainter>
#include <QTimer>
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
    DotDetailDataWindow(DataState state,QWidget * parent = nullptr);
    void startTimer();                                             //启动定时器
    void stopTimer();                                              //关闭定时器
private:
    void paintEvent(QPaintEvent *event) override;                  //重写绘画事件
    void drawThisInNoData(QPainter * painter);                     //绘制没有数据时候的窗体
    void drawTextInNoData(QPainter * painter,QPoint pos);          //绘制文本当没有数据时
    void drawThisInHaveData(QPainter * painter);                   //绘制有数据时候的窗体
    void drawFrameRect(QRect rect,QPainter * painter);             //绘制窗体框架
    void drawTitleRect(QRect rect,QPainter * painter);             //绘制title Rect （最大三比交易）
    void initTimer();                                              //初始化定时器
private:
    DataState m_CurrentState;                                      //当前数据状态
    QTimer * timer;                                                //定时器
};

#endif // DOTDETAILDATAWINDOW_H
