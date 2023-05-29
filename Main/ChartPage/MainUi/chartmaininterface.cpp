#include "chartmaininterface.h"
#include "ui_chartmaininterface.h"
#include <QImage>
#include <QPixmap>
#include <QPicture>
#include <QPointer>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <Kit/LogInfo/clog.h>
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include "NetCore/msgqueue.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include <QMouseEvent>
#include <QPainter>

using namespace ScreenFunc;
ChartMainInterface::ChartMainInterface(QWidget *parent) :
    BaseCustomWidget(parent),
    ui(new Ui::ChartMainInterface)
{
    ui->setupUi(this);
    initButtonGroupInChartPage();
    initWeekLayout();
    initMonthLayout();
    initYearLayout();
    initWeekPageCtrls();
    initMonthPageCtrls();
    initYearPageCtrls();
    ui->pushButton_Week->clicked();
}

ChartMainInterface::~ChartMainInterface()
{
    delete ui;
}
//设置当前Active状态
void ChartMainInterface::setActiveWindow(bool state)
{
    LOG("ChartMainInterface::setActiveWindow %d",state);
    m_IsActiveWindow = state;
}
//初始化图表中的按钮组
void ChartMainInterface::initButtonGroupInChartPage()
{
    buttongroup_in_chart = new QButtonGroup();
    buttongroup_in_chart->addButton(ui->pushButton_Week,ChartSelectType::week);
    buttongroup_in_chart->addButton(ui->pushButton_Month,ChartSelectType::month);
    buttongroup_in_chart->addButton(ui->pushButton_Year,ChartSelectType::year);
    connect(buttongroup_in_chart,SIGNAL(buttonClicked(int)),this,SLOT(on_ButtonGroup_In_Chart_Clicked(int)));
}

//图表界面中按钮组中按钮点击槽函数
void ChartMainInterface::on_ButtonGroup_In_Chart_Clicked(int pagetype)
{
    for(QAbstractButton * button :buttongroup_in_chart->buttons()){
        setButtonStyleOnNormal((QPushButton*)button);}
    LOG("pagetype is :%d",pagetype);
    switch (pagetype) {
        case ChartSelectType::week:{
            setButtonStyleAfterClicked((QPushButton*)buttongroup_in_chart->button(pagetype));
            ui->stackedWidget->setCurrentWidget(ui->weekpage);
            break;}
        case ChartSelectType::month:{
            setButtonStyleAfterClicked((QPushButton*)buttongroup_in_chart->button(pagetype));
            ui->stackedWidget->setCurrentWidget(ui->monthpage);
            break;};
        case ChartSelectType::year:{
            setButtonStyleAfterClicked((QPushButton*)buttongroup_in_chart->button(pagetype));
            ui->stackedWidget->setCurrentWidget(ui->yearpage);
            break;};
    }
}

//设置按钮点击后的样式
void ChartMainInterface::setButtonStyleAfterClicked(QPushButton * button)
{
    button->setStyleSheet("color:rgb(249,219,97);"
                          "background-color:rgb(35,35,35);"
                          "border:none;"
                          "font: 19pt \"微软雅黑\";");
}
//设置按钮正常时样式
void ChartMainInterface::setButtonStyleOnNormal(QPushButton * button)
{
    button->setStyleSheet("background-color: rgb(249, 219, 97);"
                          "color:rgb(0,0,0);"
                          "border:3px solid;"
                          "border-color:rgb(0,0,0);"
                          "font: 19pt \"微软雅黑\";");
}

void ChartMainInterface::initWeekPageCtrls()
{
    SelectScrollBar * bar = new SelectScrollBar(ChartSelectType::week);
    bar->setParent(ui->weekchartwidget);
    bar->setGeometry(0,0,getScreenSize().width(),getScreenSize().height()/22);
    bar->show();


}
void ChartMainInterface::initMonthPageCtrls()
{
    SelectScrollBar * bar = new SelectScrollBar(ChartSelectType::month);
    bar->setParent(ui->monthchartwidget);
    bar->setGeometry(0,0,getScreenSize().width(),getScreenSize().height()/22);
    bar->show();


}
void ChartMainInterface::initYearPageCtrls()
{
    SelectScrollBar * bar = new SelectScrollBar(ChartSelectType::year);
    bar->setParent(ui->yearchartwidget);
    bar->setGeometry(0,0,getScreenSize().width(),getScreenSize().height()/22);
    bar->show();
}
void ChartMainInterface::initWeekLayout()
{
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->weekchartwidget,0,0);
    layout->addWidget(ui->weekdatarankwidget,1,0);
    layout->setRowStretch(0,5);
    layout->setRowStretch(0,8);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->weekpage->setLayout(layout);
}
void ChartMainInterface::initMonthLayout()
{
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->monthchartwidget,0,0);
    layout->addWidget(ui->monthdatarankwidget,1,0);
    layout->setRowStretch(0,5);
    layout->setRowStretch(0,8);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->monthpage->setLayout(layout);

}
void ChartMainInterface::initYearLayout()
{
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->yearchartwidget,0,0);
    layout->addWidget(ui->yeardatarankwidget,1,0);
    layout->setRowStretch(0,5);
    layout->setRowStretch(0,8);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->yearpage->setLayout(layout);
}
