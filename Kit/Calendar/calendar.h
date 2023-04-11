#ifndef CALENDAR_H
#define CALENDAR_H
#pragma once
#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "Kit/Calendar/SubCtrls/scrollareakit.h"

///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2022/12/28
/// @file    : Calendar.h
/// @brief   : 日历小控件
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
///
enum CalendarType
{
  Year = 1,
  YearMonth,
  YearMonthDay
};
class Calendar : public QDialog
{
    Q_OBJECT
public:
    explicit Calendar(CalendarType type,int year,QWidget *parent = nullptr);
    explicit Calendar(CalendarType type,int year,int month,QWidget *parent = nullptr);
    explicit Calendar(CalendarType type,int year,int month,int day,QWidget *parent = nullptr);
    ~Calendar();

public:
    void settitle(QString titlestr);            //设置title

private:
    void buildLayout();                          //构建布局
    void buildStyle();                           //构建样式
private:
   QWidget * mainwidget;                         //主题部分widget

   QVBoxLayout vlayout;                          //内布局

   QWidget * titlewidget;                        //标题widget
   QGridLayout titlelayout;                      //title部分布局
   QPushButton * cancelButton;                   //取消按钮
   QPushButton * makeSureButton;                 //确认按钮
   QLabel * titlelabel = nullptr;                //title label

   QWidget * contentwidget;                      //内容区widget
   QGridLayout contentlayout;                    //内容区布局

   QWidget * leftspacewidget;                    //左边空白空间widget
   QWidget * rightspacewidget;                   //右边空白空间widget

   QWidget * midwidget;                          //中部widget
   QGridLayout midwidgetlayout;                  //中部widgetlayout
   ScrollAreaKit * yearkit = nullptr;            //yearkit
   ScrollAreaKit * monthkit = nullptr;           //monthkit
   ScrollAreaKit * daykit = nullptr;             //daykit

public slots:
   void onMakeSureButtonClicked();                //确认按钮槽函数

   void onCancelButtonClicked();                  //取消按钮槽函数

signals:
   void returnTimeInfo(int year,int month);                         //返回时间信息(年、月)

   void returnTimeInfoToYearMonthDay(int year,int month,int day);   //返回时间信息(年、月、日)

   void returnTimeInfoYear(int year);                               //返回时间信息(年)

private:
   CalendarType m_thisType;                      //当前类型
   int m_year = 0;                               //当前年
   int m_month = 0;                              //当前月
   int m_day = 0;                                //当前日

};

#endif // CALENDAR_H
