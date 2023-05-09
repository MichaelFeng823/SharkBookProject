#ifndef BUDGETSUBITEM_H
#define BUDGETSUBITEM_H

#include <QWidget>
#include "Controler/DataStruct/PublicDataStruct.h"
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/4/21
/// @file    : budgetsubitem.h
/// @brief   : 预算界面item子控件
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
namespace Ui {
class BudgetSubItem;
}

class BudgetSubItem : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetSubItem(QWidget *parent = nullptr);
    BudgetSubItem(TypeBudget type,QWidget *parent = nullptr);
    ~BudgetSubItem();
public:
    void setCurrentId(int id){m_CurrentId = id;}
    void setMainTitle(QString title){m_MainTitle = title;}
    void setCurrentBudget(double budget){m_CurrentBudget = budget;}
    void setCurrentExpand(double expand){m_CurrentExpand = expand;}
    void setCurrentRemainBudget(double remainbudget){m_CurrentRemainBudget = remainbudget;}
    void setFreedPercent(double freepercent){m_FreedPercent = freepercent;}
    void setBudgetData(int id,double budget,double expand);

    void updateData();                   //更换数据显示
protected:
    void calculatePercent();             //计算百分比
    void checkBudgetType();              //检测预算类型
    void initMonthBudgetStyle();         //初始化月预算风格样式
    void initYearBudgetStyle();          //初始化年预算风格样式
private:
    Ui::BudgetSubItem *ui;

    int m_CurrentId;                     //当前编号ID (月预算(1-12),年预算(int year))
    QString m_MainTitle;                 //主标题内容
    double m_CurrentBudget;              //当前预算
    double m_CurrentExpand;              //当前支出
    double m_CurrentRemainBudget;        //剩余预算
    double m_FreedPercent;               //剩余占比(0~1);
    TypeBudget m_BudgetType;         //当前预算类型
};

#endif // BUDGETSUBITEM_H
