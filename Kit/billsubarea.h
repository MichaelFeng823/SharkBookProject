#ifndef BILLSUBAREA_H
#define BILLSUBAREA_H

#include <QWidget>
#include "Controler/PublicDataStruct.h"
#include "Util/calendar.h"
#include <QStandardItemModel>
#include <QSqlDatabase>

///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023-02-22
/// @file    : BillSubArea.h
/// @brief   : 账单详情子界面  收入 支出 结余
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
///
namespace Ui {
class BillSubArea;
}

//账单详情子界面----月份收支平衡统计数据结构体
struct BillBalanceStruct
{
    int month = 0;  //月份
    double income = 0.00; //收入
    double expand = 0.00; //支出
    double cashSurplus = 0.00; //结余
    bool isExsit = false; //该月份是否有账单
};
class BillSubArea : public QWidget
{
    Q_OBJECT

public:
    explicit BillSubArea(QVector<BillTableStruct> billinfo,QWidget *parent = nullptr);
    ~BillSubArea();
protected:
   void setDefaultYear();                     //设置当前默认年
   void setCurrentYear(int year);             //设置当前年
   void setIncomeCount(double incomeCount);   //设置收入汇总
   void setExpandCount(double expandCount);   //设置支出汇总
   void setBalanceCount(double balanceCount); //设置结余汇总

   int  getCurrentYear(){return m_CurrentYear;}           //获取当前年
   double getIncomeCount(){return m_IncomeCount;}         //获取收入汇总
   double getExpandCount(){return m_ExpandCount;}         //获取支出汇总
   double getBalanceCount(){return m_BalanceCount;}       //获取结余汇总

   void calculateIncomeCount();               //计算收入汇总
   void calculateExpandCount();               //计算支出汇总
   void calculateBalanceCount();              //计算结余
   void statisticsCount();                    //统计账单数据

   void initModel();                          //初始化Model
   void initTableStyle();                     //初始化表格样式
   void initTableView();                      //初始化表格
   void builtConnect();                       //绑定槽函数
   void callCalendar(int year);               //调用日历控件
   void updateDateDisplay();                  //刷新数据显示
   void getBillData();                        //获取当前年份账单数据


public slots:
   void onQuitButtonClicked();                //退出按钮点击槽函数
   void onMenuButtonClicked();                //菜单按钮槽函数
   void onRecieveDateInfo(int year);        //收到设置年份槽函数


protected:
    QVector<BillBalanceStruct> m_BillBalanceList;
    QVector<BillTableStruct> m_BillInfo;
    int m_CurrentYear;                         //当前年份
    double m_IncomeCount = 0;                  //收入汇总
    double m_ExpandCount = 0;                  //支出汇总
    double m_BalanceCount = 0;                 //结余汇总

signals:
    void RequestCloseSignal(bool);                 //请求关闭窗口信号
    void RequestOpenMenuSignal(bool);              //请求打开菜单信号
    void RequestOpenCalendarSignal(QDate);          //请求打开日历控件信号
private:
    bool eventFilter(QObject *obj, QEvent *event);  //事件过滤器
    bool quitIsClicked = false;                     //退出标签是否被按下
    bool menuIsClicked = false;                     //菜单标签是否被按下
    bool calendarIsClicked = false;                 //日历标签是否被按下
    QSqlDatabase coon;
    bool hasConnected;
private:
    QStandardItemModel * model = nullptr;
    Calendar * m_Calendar;                          //日历
    Ui::BillSubArea *ui;
};

#endif // BILLSUBAREA_H
