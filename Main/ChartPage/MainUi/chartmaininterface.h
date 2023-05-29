#ifndef CHARTMAININTERFACE_H
#define CHARTMAININTERFACE_H

#include <QWidget>
#include <QTimer>
#include <QHostInfo>
#include <QLabel>
#include <QButtonGroup>
#include "BaseClass/basecustomwidget.h"
#include "Main/ChartPage/Other/selectscrollbar.h"

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
     void on_ButtonGroup_In_Chart_Clicked(int pagetype);
private:
     void initButtonGroupInChartPage();               //初始化按钮组
     void setButtonStyleAfterClicked(QPushButton * button);
     void setButtonStyleOnNormal(QPushButton * button);

     void initWeekPageCtrls();
     void initMonthPageCtrls();
     void initYearPageCtrls();

     void initWeekLayout();
     void initMonthLayout();
     void initYearLayout();

private:
     Ui::ChartMainInterface *ui;
     QButtonGroup * buttongroup_in_chart = nullptr;           //图表页面中的按钮组
};

#endif // CHARTMAININTERFACE_H
