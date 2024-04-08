#include "budgetsubitem.h"
#include "ui_budgetsubitem.h"
#include "Kit/LogInfo/clog.h"
#include <QMouseEvent>

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
    this->installEventFilter(this);
    ui->EditButton->installEventFilter(this);
}
BudgetSubItem::~BudgetSubItem()
{
    delete ui;
}
//设置预算数据
void BudgetSubItem::setBudgetData(int id,double budget,double expand)
{
    setCurrentId(id);
    setCurrentBudget(budget);
    setCurrentExpand(expand);
    calculatePercent();
    updateData();
}
//更换数据显示
void BudgetSubItem::updateData()
{
    ui->labelnum_CurrentBudget->setText(QString::number(m_CurrentBudget,'f',2));
    ui->labelnum_CurrentExpand->setText(QString::number(m_CurrentExpand,'f',2));
    calculatePercent();
    checkBudgetType();
    ui->labelnum_RemainBudget->setText(QString::number(m_CurrentRemainBudget,'f',2));
    ui->circulardiagram->setAnimationEndValue(m_FreedPercent);
}
//计算百分比
void BudgetSubItem::calculatePercent()
{
    if(m_CurrentBudget <= 0){
         setFreedPercent(-1);
         return;
    }
    setCurrentRemainBudget(m_CurrentBudget - m_CurrentExpand);
    setFreedPercent(m_CurrentRemainBudget/m_CurrentBudget);
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
    ui->MainTitle->setText(QString("%1月总预算").arg(m_CurrentId));
    ui->labeltext_CurrentBudget->setText(QString("本月预算"));
    ui->labeltext_CurrentExpand->setText(QString("本月支出"));
}
//初始化年预算风格样式
void BudgetSubItem::initYearBudgetStyle()
{
    ui->MainTitle->setText(QString("%1年总预算").arg(m_CurrentId));
    ui->labeltext_CurrentBudget->setText(QString("本年预算"));
    ui->labeltext_CurrentExpand->setText(QString("本年支出"));
}
//事件过滤器
bool BudgetSubItem::eventFilter(QObject *obj, QEvent *event)
{
    QMouseEvent * mouseEvent = dynamic_cast<QMouseEvent*>(event);
    if(event->type() == QMouseEvent::MouseButtonPress){
        if(mouseEvent->button() == Qt::LeftButton){
        }

    }else if(event->type() == QMouseEvent::MouseButtonRelease){
        if(mouseEvent->button() == Qt::LeftButton){
            emit requestModify(m_BudgetType);
        }
    }
    return QWidget::eventFilter(obj, event);
}
