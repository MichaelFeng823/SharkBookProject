#ifndef SELECTSCROLLBAR_H
#define SELECTSCROLLBAR_H
#include <QObject>
#include <BaseClass/photolistview.h>
#include <Controler/DataStruct/PublicDataStruct.h>
#include <QPushButton>
#include <QDate>
#include <QStandardItemModel>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023-5-30
/// @file    : selectscrollbar.h
/// @brief   : 图表界面滚动条选择类
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
class SelectScrollBar:public PhotoListView
{
    Q_OBJECT
public:
    SelectScrollBar(ChartSelectType type = ChartSelectType::week,QWidget * parent = nullptr);

private:
    void initCtrls();               //初始化控件内容
    void initWeekCtrls();           //初始化周控件
    void initMonthCtrls();          //初始化月控件
    void initYearCtrls();           //初始化年控件
    QPushButton * initButtonItem(int); //初始化table item
private:
    ChartSelectType m_CurrentType;  //当前类型
    QDate m_CurrentDate;            //当前日期
    QStandardItemModel * model;     //model
private:
    int m_WeekNum;                  //周数
    int m_MonthNum;                 //月数
    int m_YearNum = 2;              //年数
signals:
    void ItemClicked(ChartSelectType,int);

};

#endif // SELECTSCROLLBAR_H
