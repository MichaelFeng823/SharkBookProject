#ifndef BILLDATAITEMWITHPROCESSBAR_H
#define BILLDATAITEMWITHPROCESSBAR_H
#pragma once

#include <QWidget>
#include "Controler/PublicDataStruct.h"

namespace Ui {
class BillDataItemWithProcessBar;
}

class BillDataItemWithProcessBar : public QWidget
{
    Q_OBJECT

public:
    explicit BillDataItemWithProcessBar(BillTableStruct billinfo,QWidget *parent = nullptr);
    ~BillDataItemWithProcessBar();

    void setTypeText(QString typetext);                   //设置收费支出类型文本

    void setTypeIcon(QString typeiconurl);                //设置收费支出图标

    void setMoneynum(double num);                         //设置收费支出金额

    void setPercent(double percent);                      //设置所占百分比

    void setProgressBarvalue(int value);                  //设置进度条value

    void setDate(QDate date);                             //设置日期

    void setBillNo(int billno){m_BillItemInfo.billNo = billno;} //设置BillNo

    void setBillRemark(QString str){m_BillItemInfo.remarks = str;}                            //设置账单备注

    int  getBillNo(){return m_BillItemInfo.billNo;}
signals:
    void getDetailBillInfo(BillTableStruct billinfo);     //发送修改账单信息信号


private:
    bool eventFilter(QObject *obj, QEvent *event);


private:
    Ui::BillDataItemWithProcessBar *ui;
    BillTableStruct m_BillItemInfo;                       //账单item信息

};

#endif // BILLDATAITEMWITHPROCESSBAR_H
