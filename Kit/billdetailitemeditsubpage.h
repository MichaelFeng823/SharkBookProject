#ifndef BILLDETAILITEMEDITSUBPAGE_H
#define BILLDETAILITEMEDITSUBPAGE_H
#pragma once
#include "Controler/PublicDataStruct.h"
#include <QWidget>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/1//12
/// @file    : BillDetailItemEditSubPage.h
/// @brief   : 账单详情item编辑子页面
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
namespace Ui {
class BillDetailItemEditSubPage;
}

class BillDetailItemEditSubPage : public QWidget
{
    Q_OBJECT

public:
    explicit BillDetailItemEditSubPage(QWidget *parent = nullptr);
    BillDetailItemEditSubPage(BillTableStruct billinfo, QWidget *parent = nullptr);
    ~BillDetailItemEditSubPage();
public:
    void setInAndOutTypeText(QString str);   //设置支出和收入类型

    void setMoneyText(QString str);           //设置金额

    void setDateText(QDate date);            //设置日期

    void setRemarks(QString str);            //设置备注

    bool deleteBillInfo(BillTableStruct billinfo); //删除账单信息

public slots:
    void onReturnButtonClicked();            //返回按钮槽函数

    void onEditButtonClicked();              //编辑按钮槽函数

    void onDeleteButtonClicked();            //删除按钮槽函数

signals:
    void callApplyModify(BillTableStruct billinfo); //申请修改信号

    void callApplyDeleteSuccessUpdateParent(bool status); //申请删除成功后刷新

private:
    Ui::BillDetailItemEditSubPage *ui;

    BillTableStruct m_billiteminfo;
};

#endif // BILLDETAILITEMEDITSUBPAGE_H
