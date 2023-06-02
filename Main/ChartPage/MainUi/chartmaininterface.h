#ifndef CHARTMAININTERFACE_H
#define CHARTMAININTERFACE_H

#include <QWidget>
#include <QTimer>
#include <QHostInfo>
#include <QLabel>
#include <QButtonGroup>
#include <QPointer>
#include "BaseClass/basecustomwidget.h"
#include "Main/ChartPage/Other/selectscrollbar.h"
#include "Main/ChartPage/Other/chartmodel.h"
#include "Main/ChartPage/Other/chartmasksubpage.h"
#include "BaseClass/detialtableview.h"
#include <QSqlDatabase>
#include <QStandardItemModel>

///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023-06-01
/// @file    : chartmaininterface.h
/// @brief   : 图表主界面
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************

namespace Ui {
class ChartMainInterface;
}

class ChartMainInterface : public BaseCustomWidget
{
    Q_OBJECT
public:
    explicit ChartMainInterface(QWidget *parent = nullptr);
    ~ChartMainInterface();
     void setActiveWindow(bool state);                //设置当前Active状态
public slots:
     void on_ButtonGroup_In_Chart_Clicked(int pagetype);   //当按钮组中任意按钮点击时收到的槽函数
     void on_TypeChoose_Clicked();     //当选中类型转换按钮时的槽函数
     void on_ReceiveTypeSignal(InOrOut);                   //当收到类型信号时
     void on_ReceiveSelectScorllBarSignal(ChartSelectType,int);//当收到SelectScrollBar信号时
private:
     void initButtonGroupInChartPage();                    //初始化按钮组
     void setButtonStyleAfterClicked(QPushButton * button);
     void setButtonStyleOnNormal(QPushButton * button);

     void initWeekPageCtrls();
     void initMonthPageCtrls();
     void initYearPageCtrls();

     void initWeekLayout();
     void initMonthLayout();
     void initYearLayout();

     void modifyTitleType();                                   //修改标题类型
     void modifyLoadDataType();                                //修改加载数据类型
     void modifyRankTitleType();                               //修改排行榜类型
     void modifyLoadRankDataType();                            //修改加载的排行榜数据类型

     void setRankInfo(QVector<BillTableStruct> list,DetialTableview * table,QStandardItemModel * model);          //设置排行榜数据信息
     double calculatePercent(double moneynum,QVector<BillTableStruct>list);                                       //计算占比比值
     QVector<BillTableStruct> statisticalClassification(QVector<BillTableStruct> list);                           //按消费支出/收入类别统计分类

     QVector<QDate> getDateByWeekNum(int weeknum);             //获取日期通过周数
     QVector<BillTableStruct> getWeekBills(int id);            //获取周账单
     QVector<BillTableStruct> getMonthBills(int id);           //获取月账单
     QVector<BillTableStruct> getYearBills(int id);            //获取年账单

     void updateWeekPage();                                    //更新周页面
     void updateMonthPage();                                   //更新月页面
     void updateYearPage();                                    //更新年月面
     void updatePage();                                        //更新整个页面
private:
     QLabel * weekRankTitle;                                   //周页面排行版title
     QLabel * monthRankTitle;                                  //月页面排行版title
     QLabel * yearRankTitle;                                   //年月面排行版title
private:
     InOrOut m_Type = InOrOut::Expand;                        //当前预算类型
     bool isExpandOrClose = true;
     bool isChartAreaFirstShow = true;                        //是否是图表界面第一次显示
     bool hasConnected = false;                               //获取数据库对象是否成功
     QSqlDatabase coon;                                       //数据库连接对象
     QVector<BillTableStruct> m_WeekList;
     QVector<BillTableStruct> m_MonthList;
     QVector<BillTableStruct> m_YearList;
     int m_CurrentWeekId;
     int m_CurrentMonthId;
     int m_CurrentYearId;
private:
     Ui::ChartMainInterface *ui;
     QButtonGroup * buttongroup_in_chart = nullptr;           //图表页面中的按钮组
     SelectScrollBar * weekbar;
     ChartModel * weekmodel;
     SelectScrollBar * monthbar;
     ChartModel * monthmodel;
     SelectScrollBar * yearbar;
     ChartModel * yearmodel;
     DetialTableview * weektable;
     DetialTableview * monthtable;
     DetialTableview * yeartable;

     QStandardItemModel * weekrankmodel;
     QStandardItemModel * monthrankmodel;
     QStandardItemModel * yearrankmodel;

     QPointer<ChartMaskSubPage> m_Mask;
};

#endif // CHARTMAININTERFACE_H
