#ifndef BOOKKEEPINGSELECTAREA_H
#define BOOKKEEPINGSELECTAREA_H
#pragma once

#include <QWidget>
#include <QTableView>
#include <QGridLayout>
#include <QPushButton>
#include "detialtableview.h"
#include "Kit/bookkeepinginputkit.h"
#include "Controler/PublicDataStruct.h"
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2022/12/27
/// @file    : BookkeepingSelectArea.h
/// @brief   : 记账功能菜单页面
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************

namespace Ui {
class BookkeepingSelectArea;
}

class BookkeepingSelectArea : public QWidget
{
    Q_OBJECT

public:
    explicit BookkeepingSelectArea(QWidget *parent = nullptr);
    BookkeepingSelectArea(BillTableStruct billinfo,bool isModify,QWidget *parent = nullptr);
    ~BookkeepingSelectArea();

public:
    void buildLayout();          //构建布局

protected:
    void initMenu();            //初始化记账菜单

    void initExpandMenuButtonStyle();   //初始化支出菜单按钮样式

    void initIncomeMenuButtonStyle();   //初始化收入菜单按钮样式

    void initTablecontent();    //初始化表格内容

    void initExpandMenuButton();  //初始化支出菜单按钮

    void initIncomeMenuButton();  //初始化收入菜单按钮

public slots:
    void onExpanditurePageClicked();

    void onIncomePageClicked();

    void onCancelButtonClicked();

    void onMenuIconButtonClicked();
    //收到账单信息槽函数
    void onReceiveBillInfo(QDate date,double num,QString Remarkstr);

    //收到账单信息修改槽函数
    void onReceiveBillInfoModify(int billno,QDate date,double num,QString Remarkstr);


signals:
     void sendSignalToUpdateBillArea(bool status);                             //发送信号给主界面刷新账单信息

     void sendSignalToUpdateDetailBillPage(BillTableStruct billinfo);                             //刷新账单详情界面信号

private:
     bool m_IsModify = false;

private:
    DetialTableview * expanditureview;
    DetialTableview * incomeview;

    BookkeepingInputKit * bookkeepinginputkit = nullptr;  //记账输入工具

    QGridLayout expanditurewidgetlayout;
    QGridLayout incomewidgetlayout;
    QGridLayout expanditureviewlayout;
    QGridLayout incomeviewlayout;

    QVector<QString> expandituremenuvector;  //记账菜单容器
    QVector<QString> incomemenuvector;  //记账菜单容器
    QVector<QPushButton*> m_ExpandButtonList;  //支出按钮容器
    QVector<QPushButton*> m_IncomeButtonList;  //收入按钮容器

    Ui::BookkeepingSelectArea *ui;
};

#endif // BOOKKEEPINGSELECTAREA_H
