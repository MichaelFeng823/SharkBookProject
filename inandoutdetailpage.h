#ifndef INANDOUTDETAILPAGE_H
#define INANDOUTDETAILPAGE_H
#pragma once
#include <QWidget>
#include <QStandardItemModel>
#include "Controler/PublicDataStruct.h"
#include "Controler/PublicUerInfo.h"
#include "Kit/billdataitemwithprocessbar.h"
#include "bookkeepingselectarea.h"
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/1/10
/// @file    : InAndOutDetailPage.h
/// @brief   : 收入或支出详情页面
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************

namespace Ui {
class InAndOutDetailPage;
}

enum SortType{
    sortByMoney = 1,
    sortByTime
};
class InAndOutDetailPage : public QWidget
{
    Q_OBJECT

public:
    explicit InAndOutDetailPage(QVector<BillTableStruct> BillList,QDate date,double num = 0,InAndOutType inandouttype = InAndOutType::OutType,QWidget *parent = nullptr);
    ~InAndOutDetailPage();

public:
    void setTitlenum(double num);            //设置标题支出或收入金额

    void setTitleDate(QDate date);           //设置标题日期

    void setTitleName(InAndOutType type);    //设置TitleName(本月xxx)

public slots:
    void onReturnButtonClicked();            //返回按钮按下槽函数

    void onSortByMoneyButtonClicked();      //按金额按钮按下槽函数

    void onSortByTimeButtonClicked();      //按时间按钮按下槽函数

    void receiveGetBillInfoSignal(BillTableStruct billinfo);      //收到申请查看账单item详情子页面

    void onReceiveModifyBillInfoSignal(BillTableStruct billinfo); //收到修改账单信息的信号

    void onReceiveUpdateDetailBiilPageSignal(BillTableStruct billinfo);        //收到更新BillPage槽函数

    void onReceiveAfterDeleteUpdatePage(bool status);             //收到在删除成功后刷新页面槽函数

signals:
    void callApplyUpdateBillPage(bool status);       //发送修改账单信息信号


private:
    void initTableview();                    //初始化表格内容

    void initLayout();                       //初始化布局

    void removeDifferentFromInAndOutType();  //去除不同类型的数据

    double calculatePercent(double moneynum);   //计算占比比值

    void customSort();                       //自定义排序

    static bool sortByMoney(const BillTableStruct & billfirst,const BillTableStruct & billsecond);                      //通过金额排序

    static bool sortByTime(const BillTableStruct & billfirst,const BillTableStruct & billsecond);                       //通过时间排序
private:
    Ui::InAndOutDetailPage *ui;
    QStandardItemModel * model;              //表格模型
    BookkeepingSelectArea * bookkeepingselectarea = nullptr;
    QVector<BillTableStruct> m_BillList;     //账单信息
    QVector<BillDataItemWithProcessBar*> m_Itemlist; //item list;
    SortType m_sorttype = SortType::sortByMoney;  //排序类型
    InAndOutType m_inandouttype = InAndOutType::OutType;   //收入支出类型
};

#endif // INANDOUTDETAILPAGE_H
