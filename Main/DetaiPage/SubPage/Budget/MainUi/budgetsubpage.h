#ifndef BUDGETSUBPAGE_H
#define BUDGETSUBPAGE_H

#include <QWidget>
#include "Main/DetaiPage/SubPage/Budget/budgetmasksubpage.h"
#include "BaseClass/detialtableview.h"
#include "Main/DetaiPage/SubPage/Budget/SubCtrls/budgetsubitem.h"
#include <QPointer>

///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023-3-22
/// @file    : budgetsubpage.h
/// @brief   : 预算子界面
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************

namespace Ui {
class BudGetSubPage;
}

class BudGetSubPage : public QWidget
{
    Q_OBJECT

public:
    explicit BudGetSubPage(QWidget *parent = nullptr);
    ~BudGetSubPage();

public slots:
    void onRecieveExpandOrCloseSignal(bool);            //收到展开或者关闭的信号槽函数
    void onRecieveSetBudgetType(TypeBudget type);       //收到设置打开预算界面类型槽函数
    void onRecieveCloseRequest(bool);                   //收到关闭页面请求
    void onRecieveMenuRequest(bool);                    //收到打开菜单请求
    void onAddBudgetClicked();                          //点击添加预算槽函数
    void onRecieveBudgetBySet(TypeBudget,double);       //收到设置预算数据槽函数
    void onReceiveModifyBudgetRequest(TypeBudget); //当收到修改预算请求时的槽函数

protected:
    void CheckBeforeOpen();                             //打开页面前检测打开的该类型是否为空
    void openAddBudget();                               //打开添加预算界面
    void openMonthlyBudgetWidget();                     //打开月预算界面
    void openAnnualBudgetWidget();                      //打开年预算界面
    void openAddBudgetPane();                           //打开添加预算界面
    void showMonthlyBudgetWidget();                     //显示月预算界面
    void showAnnualBudgetWidget();                      //显示年预算界面

    void initMonthlyBudgetWidgetContent();             //初始化月预算界面内容
    void initAnnualBudgetWidgetContent();              //初始化年预算界面内容
    void initTableViewContent(DetialTableview * tableview,int rowcounts);  //初始化表格内容
    void initDateIdContent();                          //初始化日期ID内容
    void initMap();                                    //初始化map容器

    void updateDataOnPage();                           //更新页面上的数据

private:
     bool eventFilter(QObject *obj, QEvent *event);    //事件过滤器
     void setTitleByBudgetType();                      //设置标题内容根据预算类型
signals:
     void expandOrClose(bool);                         //发送展开或者关闭的信号
     void requestCloseSignal(bool);                        //请求关闭页面信号
     void requestMenuSignal(bool);                         //请求菜单信号

private:
    QPointer<BudgetMaskSubpage> m_Mask;        //遮罩预算年/月切换子界面
    DetialTableview * m_MonthBudgetTableview;  //月预算界面tableview
    DetialTableview * m_AnnualBudgetTableview; //年预算界面tableview
    QMap<TypeBudget,QVector<QPointer<BudgetSubItem>>> m_BudgetItemMap; //budget item map容器
    QVector<QPointer<BudgetSubItem>> monthlyBudgetItemVector; //月预算item容器
    QVector<QPointer<BudgetSubItem>> AnnualBudgetItemVector;  //年预算item容器
    bool m_IsClicked = false;          //是否按下
    bool m_IsExpand = false;           //是否展开
    bool m_IsEmptyOfMonthltyBudget = true;     //月预算是否为空
    bool m_IsEmptyOfAnuualBudget = true;       //年预算是否为空
    bool m_IsInitMonthBudget = true;           //是否为第一次初始化显示月预算界面
    bool m_IsInitAnnualBudget = true;          //是否为第一次初始化显示年预算界面
    TypeBudget m_TypeBudget = TypeBudget::MonthlyBudget;           //当前预算类型

    double m_Budget[3] = {0.00};
    double m_Expand[3] = {0.00,50.00,500.00};
    int    m_DateId[3] = {0};
    double m_MonthlyBudget = 0.00;
    double m_MonthlyExpand = 0.00;
    double m_AnnualBudget = 0.00;
    double m_AnnualExpand = 0.00;

    Ui::BudGetSubPage *ui;
};

#endif // BUDGETSUBPAGE_H
