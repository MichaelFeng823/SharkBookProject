#ifndef CALENDARKIT_H
#define CALENDARKIT_H
#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include "Kit/Calendar/calendar.h"
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2022/12/14
/// @file    : CalendarKit.cpp
/// @brief   : 明细页面日历控件类
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
class CalendarKit : public QWidget
{
    Q_OBJECT
public:
    explicit CalendarKit(QWidget *parent = nullptr);

public:
    void setYear(const QString datestr){m_currentyear = datestr;setDate(m_labelyear,"     "+m_currentyear+"年");}
    void setMonth(const QString datestr){m_currentmonth = datestr;setDate(m_labelmonth,"   " + m_currentmonth + "月");}
    QString & getYear(){return m_currentyear;}
    QString & getMonth(){return m_currentmonth;}

public slots:
    void receiveTimeInfo(int year,int month);                  //收到设置日期信号槽函数


signals:
    void sendSetDateSignal(int year,int month);                //发送设置日期信号



private:
    void setDate(QLabel & label,const QString datestr){label.setText(datestr);}
    void CallCalendar(QString year,QString month);             //调用日历控件
    bool eventFilter(QObject *obj, QEvent *event);             //事件过滤器

private:
    QGridLayout gridlayout;         //栅格布局
    QLabel m_labelyear;             //年标签
    QLabel m_labelmonth;            //月标签
    QLabel m_labelarrow;            //下箭头标签
    QLabel m_labelspqce;            //空白部分标签

    Calendar * m_Calendar;          //日历
    //Calendar m_Calendar;

    QString m_currentyear;          //当前年份
    QString m_currentmonth;         //当前月份
};

#endif // CALENDARKIT_H
