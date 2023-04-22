#include "budgetsubitem.h"
#include "ui_budgetsubitem.h"

BudgetSubItem::BudgetSubItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BudgetSubItem)
{
    ui->setupUi(this);
}

BudgetSubItem::BudgetSubItem(TypeBudget type,QWidget *parent):
    QWidget(parent),
    ui(new Ui::BudgetSubItem),m_BudgetType(type)
{
    ui->setupUi(this);
    checkBudgetType();
}
BudgetSubItem::~BudgetSubItem()
{
    delete ui;
}
//检测预算类型
void BudgetSubItem::checkBudgetType()
{
    if(TypeBudget::MonthlyBudget == m_BudgetType)
        initMonthBudgetStyle();
    else if(TypeBudget::YearBudget == m_BudgetType)
        initYearBudgetStyle();
}
//初始化月预算风格样式
void BudgetSubItem::initMonthBudgetStyle()
{
    ui->MainTitle->setText(QString("04月总预算"));
    ui->labeltext_CurrentBudget->setText(QString("本月预算"));
    ui->labeltext_CurrentExpand->setText(QString("本月支出"));
}
//初始化年预算风格样式
void BudgetSubItem::initYearBudgetStyle()
{
    ui->MainTitle->setText(QString("2023总预算"));
    ui->labeltext_CurrentBudget->setText(QString("本年预算"));
    ui->labeltext_CurrentExpand->setText(QString("本年支出"));
}
