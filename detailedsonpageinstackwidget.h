#ifndef DETAILEDSONPAGEINSTACKWIDGET_H
#define DETAILEDSONPAGEINSTACKWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include "Kit/billdataitem.h"
#include "Kit/billdataitemcopy.h"
#include "Kit/calendarkit.h"
#include "Kit/inandoutkit.h"
#include "detialtableview.h"
#include "Controler/PublicDataStruct.h"
#include <QSqlDatabase>
#include <QStandardItemModel>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2022/12/16
/// @file    : DetailedSonPageInStackwidget.h
/// @brief   : 明细主界面在stackwidget中
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
namespace Ui {
class DetailedSonPageInStackwidget;
}

class DetailedSonPageInStackwidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailedSonPageInStackwidget(QWidget *parent = nullptr);
    ~DetailedSonPageInStackwidget();

public:
    void UpdateBillContent();                           //刷新账单界面

    void UpdateBillDetailPage();                        //刷新账单详情界面

public slots:
    void receiveSetDateInfo(int year,int month);         //收到设置日期信息槽函数

    void receiveCallGetInAndOutInfo(int type,double num=0); //收到获取账单详情信息

    void receiveUpdateBillInfoSignal(bool status); //收到刷新账单信息的信号

    void onBillSubButtonClicked();                 //打开账单子界面槽函数

    void onBudgetButtonClicked();                  //打开预算子界面槽函数

    void onAssetManagerButtonClicked();            //打开资产管理子界面槽函数

protected:
    void buildUiLayout();     //构建UI布局

    void buildConnect();      //建立槽函数连接

    void getBilldata();       //获取账单数据

    void initTableview();     //初始化表格内容

    void initInAndOutKitContent();  //初始化收入支出控件内容

    bool eventFilter(QObject *obj, QEvent *event);   //事件过滤器

    static bool sortBillInfoByDate(const BillTableStruct & billfirst,const BillTableStruct & billsecond);      //通过日期排序账单信息

signals:



private:
    QGridLayout outterlayout;

    //QGridLayout titlelayout;
    QGridLayout inAndoutlayout;
    //QGridLayout toollayout;
    QGridLayout tableviewlayout;

    DetialTableview * tableview;
    QStandardItemModel * model;

    CalendarKit * calendarkit;
    InAndOutKit * inkit;
    InAndOutKit * outkit;
    QPointF m_TempPoint;

    QDate m_CurrentDate;                            //当前日期
    QSqlDatabase coon;
    bool hasConnected;
private:
    QVector<BillTableStruct> m_BillList;             //账单列表
    QVector<BillTableStruct> m_BillListOfYear;       //近一年的账单列表
    QVector<BillDataItem *> m_BillDateItemList;       //itemlist;
    double m_Current_InNumber = 0.00;                //当前收入数据
    double m_Current_OutNumber = 0.00;               //当前支出数据
    InAndOutType m_currentOpenType;

private:
    Ui::DetailedSonPageInStackwidget *ui;
};

#endif // DETAILEDSONPAGEINSTACKWIDGET_H
