#ifndef BILLDATAITEM_H
#define BILLDATAITEM_H
#pragma once
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QDate>
#include "Controler/GlobalInfo/PublicUerInfo.h"

///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2022/12/15
/// @file    : BillDataItem.cpp
/// @brief   : 账单数据item控件
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************

class BillDataItem : public QWidget
{
    Q_OBJECT
public:
    explicit BillDataItem(InAndOutType type = InAndOutType::OutType,QWidget *parent = nullptr);
    ~BillDataItem();

public:
    void setDate(QDate date);
    void setTitlePayNum(double num);
    void setPayType(QString strType);
    void setIcon(QString strIcon);
    void setDetialPayNum(double num);
    void setPayDetailSoucres(QString strSources);

private:
    bool eventFilter(QObject *obj, QEvent *event);  //事件过滤器
    void buildUiLayout();            //构建界面布局
    void buildUiStyle();             //构建界面样式

private:
    QGridLayout outterLayout;

    QGridLayout titlelayout;
    QLabel * m_labelPayDate;
    QLabel * m_labelPayNum;
    QWidget* titleWidget;

    QGridLayout detailelayout;
    QLabel * IconLabel;
    QLabel * labelPayType;
    QLabel * labelPayDetailSources;
    QLabel * PaynumLabel;
    QWidget * detailWidget;

private:
    QString strPayDate = "%1月%2日 星期%3";
    QString strTitlePayNum = "支出: XXX";
    QString strPayNum = "-xxx";
    QString strPayType = "xx";
    QString strIconstyle = "";
    QString strPayDetailSources = "";
    InAndOutType currenttype = InAndOutType::OutType;    //当前控件类型属性


signals:

};

#endif // BILLDATAITEM_H
