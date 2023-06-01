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
private:
     InOrOut m_Type = InOrOut::Expand;                        //当前预算类型
     bool isExpandOrClose = true;
     bool isChartAreaFirstShow = true;                        //是否是图表界面第一次显示
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
     QPointer<ChartMaskSubPage> m_Mask;
};

#endif // CHARTMAININTERFACE_H
