#include "assetmanagerpage.h"
#include "ui_assetmanagerpage.h"
#include "Util/clog.h"

AssetManagerPage::AssetManagerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AssetManagerPage)
{
    ui->setupUi(this);
    ui->label_Quit->installEventFilter(this);
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->widget);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->page_chart->setLayout(layout);
    connect(ui->pushButton_assets_mainarea,&QPushButton::clicked,this,&AssetManagerPage::on_Button_AssetsMainArea_Clicked);
    connect(ui->pushButton_Chart,&QPushButton::clicked,this,&AssetManagerPage::on_Button_Chart_Clicked);
    initButtonGroupInChartPage();
    init_AssetsPageControl_In_ChartPage();
    init_AssetsPageLayout_InChartPage();
    init_BeInDebtPageControl_In_ChartPage();
    init_BeInDebtPageLayout_InChartPage();
    init_NetAssetsPageControl_In_ChartPage();
    init_NetAssetsPageLayout_InChartPage();
    ui->pushButton_assets_mainarea->click();
    buttongroup_in_chart->button(PageTypeInChartPage::Assets)->click();
    this->raise();
    this->show();
}

AssetManagerPage::~AssetManagerPage()
{
    delete ui;
}

//初始化图表中的按钮组
void AssetManagerPage::initButtonGroupInChartPage()
{
    buttongroup_in_chart = new QButtonGroup();
    buttongroup_in_chart->addButton(ui->pushButton_asset,PageTypeInChartPage::Assets);
    buttongroup_in_chart->addButton(ui->pushButton_BeInDebt,PageTypeInChartPage::BeInDebt);
    buttongroup_in_chart->addButton(ui->pushButton_NetAssets,PageTypeInChartPage::NetAssets);

    connect(buttongroup_in_chart,SIGNAL(buttonClicked(int)),this,SLOT(on_ButtonGroup_In_Chart_Clicked(int)));
}
//初始化图表页面中的资产子界面的控件
void AssetManagerPage::init_AssetsPageControl_In_ChartPage()
{


}
//初始化图表界面中的资产子界面的布局
void AssetManagerPage::init_AssetsPageLayout_InChartPage()
{
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->label_10);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->page_chart_assets->setLayout(layout);
}
//初始化图表页面中的负债子界面的控件
void AssetManagerPage::init_BeInDebtPageControl_In_ChartPage()
{

}
//初始化图表界面中的负债子界面的布局
void AssetManagerPage::init_BeInDebtPageLayout_InChartPage()
{
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->label_9);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->page_BeInDebt->setLayout(layout);

}
//初始化图表页面中的净资产子界面的控件
void AssetManagerPage::init_NetAssetsPageControl_In_ChartPage()
{

}
//初始化图表界面中的净资产子界面的布局
void AssetManagerPage::init_NetAssetsPageLayout_InChartPage()
{
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->label_8);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->page_NetAssets->setLayout(layout);
}
//图表界面中按钮组中按钮点击槽函数
void AssetManagerPage::on_ButtonGroup_In_Chart_Clicked(int pagetype)
{
    for(QAbstractButton * button :buttongroup_in_chart->buttons()){
        setButtonStyleOnNormal((QPushButton*)button);}
    LOG("pagetype is :%d",pagetype);
    switch (pagetype) {
        case PageTypeInChartPage::Assets:{
            setButtonStyleAfterClicked((QPushButton*)buttongroup_in_chart->button(pagetype));
            ui->stackedWidget_inside->setCurrentWidget(ui->page_chart_assets);
            break;}
        case PageTypeInChartPage::BeInDebt:{
            setButtonStyleAfterClicked((QPushButton*)buttongroup_in_chart->button(pagetype));
            ui->stackedWidget_inside->setCurrentWidget(ui->page_BeInDebt);
            break;};
        case PageTypeInChartPage::NetAssets:{
            setButtonStyleAfterClicked((QPushButton*)buttongroup_in_chart->button(pagetype));
            ui->stackedWidget_inside->setCurrentWidget(ui->page_NetAssets);
            ui->stackedWidget_inside->setCurrentWidget(ui->page_chart_assets);
            break;};
    }
}
//设置按钮点击后的样式
void AssetManagerPage::setButtonStyleAfterClicked(QPushButton * button)
{
    button->setStyleSheet("color:rgb(249,219,97);"
                          "background-color:rgb(35,35,35);"
                          "border:none;"
                          "font: 19pt \"微软雅黑\";");
}
//设置按钮正常时样式
void AssetManagerPage::setButtonStyleOnNormal(QPushButton * button)
{
    button->setStyleSheet("background-color: rgb(249, 219, 97);"
                          "color:rgb(0,0,0);"
                          "border:3px solid;"
                          "border-color:rgb(0,0,0);"
                          "font: 19pt \"微软雅黑\";");
}
//资产按钮点击槽函数
void AssetManagerPage::on_Button_AssetsMainArea_Clicked()
{
    ui->stackedWidget_outside->setCurrentWidget(ui->page_Asset);
    ui->label_title->setText("资产管家");
}
//图表按钮点击槽函数
void AssetManagerPage::on_Button_Chart_Clicked()
{
    ui->stackedWidget_outside->setCurrentWidget(ui->page_chart);
    ui->label_title->setText("图表");
}
//事件过滤器
bool AssetManagerPage::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress){
        //if(obj == ui->label_Quit)
        //    this->close();
    }
    if(event->type() == QEvent::MouseButtonRelease){
        if(obj == ui->label_Quit)
            this->close();
    }
    return QWidget::eventFilter(obj, event);
}
