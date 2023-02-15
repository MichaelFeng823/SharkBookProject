#ifndef BOOKKEEPINGINPUTKIT_H
#define BOOKKEEPINGINPUTKIT_H
#pragma once
#include <QWidget>
#include "Util/calendar.h"
#include "Controler/PublicDataStruct.h"
#include <QDate>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/1/4
/// @file    : BookkeepingInputKit.h
/// @brief   : 记账输入控件
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
namespace Ui {
class BookkeepingInputKit;
}

class BookkeepingInputKit : public QWidget
{
    Q_OBJECT

public:
    explicit BookkeepingInputKit(QWidget *parent = nullptr);
    BookkeepingInputKit(BillTableStruct billinfo,bool isModify,QWidget *parent = nullptr);
    ~BookkeepingInputKit();

public:
    void setYear(int year){m_CurrentYear = year;}

    void setMonth(int month){m_CurrentMonth = month;}

    void setDay(int day){m_CurrentDay = day;}

    void setBillNo(int billno){m_billno = billno;}

    void setRemarks(QString str);

    void setResultNum(double num);

public slots:
    void numberButtonFunc();              //数字按钮槽函数

    void operatorButtonFunc();            //操作符按钮槽函数

    void finishButtonFunc();              //完成按钮槽函数

    void deleteButtonFunc();              //删除按钮槽函数

    void openCalendarFunc();              //打开日历按钮槽函数

    void receiveTimeInfo(int year,int month,int day); //收到时间信息槽函数

signals:
    void finishBookkeepingInput(QDate date,double num,QString Remarkstr);     //完成输入时信号

    void finishBookkeepingInputAndModify(int billno,QDate date,double num,QString Remarkstr);  //完成输入并修改

private:
    void ConnectFunc();                   //绑定函数

    double CalculateResult();             //计算结果

    void defaultSetDate();                //默认设置日期

    bool eventFilter(QObject *obj, QEvent *event); //事件过滤器


private:
    Ui::BookkeepingInputKit *ui;
    Calendar * m_Calendar;                //日历控件
private:
    QString m_RemarksStr = "";            //备注内容
    int m_CurrentYear;                    //当前年
    int m_CurrentMonth;                   //当前月
    int m_CurrentDay;                     //当前日

    int m_billno = -1;                         //序列号

    QString operatorNumOne;                //操作数一
    QString operatorNumTwo;                //操作数二
    QString operatorNumbol;                //操作符

    bool isExsitOperatorNuMbol = false;    //是否已经存在运算操作符

    bool m_IsModify = false;

    QStringList strList;                   //当前字符串容器
};

#endif // BOOKKEEPINGINPUTKIT_H
