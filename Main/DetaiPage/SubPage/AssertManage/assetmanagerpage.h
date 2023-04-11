#ifndef ASSETMANAGERPAGE_H
#define ASSETMANAGERPAGE_H

#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/3/23
/// @file    : AssetManagerPage.h
/// @brief   : 资产管理界面
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
namespace Ui {
class AssetManagerPage;
}

class AssetManagerPage : public QWidget
{
    Q_OBJECT

public:
    explicit AssetManagerPage(QWidget *parent = nullptr);
    ~AssetManagerPage();

    enum PageTypeInChartPage
    {
      Assets = 1,
      BeInDebt,
      NetAssets
    };

public slots:
    void on_Button_AssetsMainArea_Clicked();       //资产按钮点击槽函数

    void on_Button_Chart_Clicked();                //图表按钮点击槽函数

    void on_ButtonGroup_In_Chart_Clicked(int);     //图表界面中按钮组中按钮点击槽函数

protected:
    void init_AssetsPageControl_In_ChartPage();              //初始化图表页面中的资产子界面的控件
    void init_AssetsPageLayout_InChartPage();                //初始化图表界面中的资产子界面的布局
    void init_BeInDebtPageControl_In_ChartPage();            //初始化图表页面中的负债子界面的控件
    void init_BeInDebtPageLayout_InChartPage();              //初始化图表界面中的负债子界面的布局
    void init_NetAssetsPageControl_In_ChartPage();           //初始化图表页面中的净资产子界面的控件
    void init_NetAssetsPageLayout_InChartPage();             //初始化图表界面中的净资产子界面的布局
    void initButtonGroupInChartPage();                       //初始化图表中的按钮组
    void setButtonStyleAfterClicked(QPushButton * button);   //设置按钮点击后的样式
    void setButtonStyleOnNormal(QPushButton * button);       //设置按钮正常时样式\

private:
    bool eventFilter(QObject *obj, QEvent *event); //事件过滤器

private:
    Ui::AssetManagerPage * ui;

    QButtonGroup * buttongroup_in_chart = nullptr;           //图表页面中的按钮组
};

#endif // ASSETMANAGERPAGE_H
