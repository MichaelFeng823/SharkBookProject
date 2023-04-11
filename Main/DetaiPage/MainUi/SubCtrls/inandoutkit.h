#ifndef INANDOUTKIT_H
#define INANDOUTKIT_H
#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include "Controler/GlobalInfo/PublicUerInfo.h"
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2022/12/15
/// @file    : InAndOutKit.cpp
/// @brief   : 明细页面收入和支出显示控件
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************

class InAndOutKit : public QWidget
{
    Q_OBJECT
public:
    explicit InAndOutKit(InAndOutType type = OutType,QWidget *parent = nullptr);

    void settext(const QString textstr){m_currenttext = textstr;setlabeltext(m_labeInorOutText,m_currenttext);}
    void setnum(const QString numstr){m_currentnum = numstr;setlabeltext(m_labeInorOutnum,m_currentnum);}

private:
    void setlabeltext(QLabel & label,const QString textstr){label.setText(textstr);}
    bool eventFilter(QObject *obj, QEvent *event);  //事件过滤器

private:
    QGridLayout gridlayout;         //栅格布局
    QLabel m_labeInorOutText;       //支出(收入)文本标签
    QLabel m_labeInorOutnum;        //支出(收入)数值标签

    QString m_currenttext;          //当前支出(收入)文本
    QString m_currentnum = "  30000000.00";           //当前月份

    InAndOutType currenttype = InAndOutType::OutType;    //当前控件类型属性

signals:
    void callGetInorOutData(int type,double num=0);

};

#endif // INANDOUTKIT_H
