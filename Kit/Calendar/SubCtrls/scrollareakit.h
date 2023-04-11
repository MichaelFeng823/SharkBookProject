#ifndef SCROLLAREAKIT_H
#define SCROLLAREAKIT_H
#pragma once
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QVector>
#include <QLine>
#include <QTimer>

///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2022/12/28
/// @file    : ScrollAreaKit.h
/// @brief   : 日历控件滚动区域
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
enum thisType
{
    yearType = 1,
    monthType,
    dayType
};
class ScrollAreaKit : public QWidget
{
    Q_OBJECT
public:
    explicit ScrollAreaKit(thisType type,int setnum,QWidget *parent = nullptr);

public:

    void buildLayout();                              //构建布局
    void buildstyle();                               //构建样式
    void buildvector();                              //构建容器
    void goUp();                                     //向上滑动
    void goDown();                                   //向下滑动
    void setdate(int datenum);                       //设置日期
    void setcontent();                               //设置内容
    int  getCurrentValue(){return m_vector[m_currentIndex];}                          //获取当前内容

private:
    bool eventFilter(QObject *obj, QEvent *event);   //事件过滤器
private:
    QGridLayout m_gridlayout;
    QGridLayout m_vlayout;
    QLabel TopLabel;
    QLabel TopSpliterLabel;
    QLabel MidLabel;
    QLabel ButtomSpliterLabel;
    QLabel ButtomLabel;
    QVector<int> m_Years;
    QVector<int> m_Months = {1,2,3,4,5,6,7,8,9,10,11,12};
    QVector<int> m_Days;
    QVector<int> m_vector;
    int m_currentnum;
    thisType m_thistype;
    int Top = 0;
    int Buttom = 0;
    int m_currentIndex = 0;
    QPointF m_TempPoint;
    QTimer * goUptimer;
    QTimer * goDowntimer;
};

#endif // SCROLLAREAKIT_H
