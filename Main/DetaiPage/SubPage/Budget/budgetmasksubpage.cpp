#include "budgetmasksubpage.h"
#include "ui_budgetmasksubpage.h"
#include <QPainter>
#include "Kit/LogInfo/clog.h"
#include "Controler/PublicApi/PublicDbFunc.h"

using namespace ScreenFunc;

BudgetMaskSubpage::BudgetMaskSubpage(TypeBudget typebudget,QWidget *parent):
   QWidget(parent), m_TypeBudget(typebudget),
    ui(new Ui::BudgetMaskSubpage)
{
    ui->setupUi(this);
    ui->widget_AnuallyBudget->installEventFilter(this);
    ui->widget_MonthlyBudget->installEventFilter(this);
    this->setFixedSize(getScreenSize().width(),300);
    ckeckBudgetType();
}
BudgetMaskSubpage::~BudgetMaskSubpage()
{
    delete ui;
}
//事件过滤器
bool BudgetMaskSubpage::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        m_IsClicked = true;
    }

    if(event->type() == QEvent::MouseButtonRelease)
    {
       if(m_IsClicked){
           if(obj == ui->widget_MonthlyBudget){
               setTypeBudget(TypeBudget::MonthlyBudget);
           }
           if(obj == ui->widget_AnuallyBudget){
               setTypeBudget(TypeBudget::YearBudget);
           }
           emit sendTypeBudget(m_TypeBudget);
           ckeckBudgetType();
           this->close();
       }
        m_IsClicked = false;
    }
    return QWidget::eventFilter(obj, event);
}
//检查预算类型
void BudgetMaskSubpage::ckeckBudgetType()
{
    ui->label_MonthBudgetChoose->clear();
    ui->label_YearBudgetChoose->clear();
    if(m_TypeBudget == TypeBudget::MonthlyBudget){
         ui->label_MonthBudgetChoose->setText("√");
    }
    else if(m_TypeBudget == TypeBudget::YearBudget){
         ui->label_YearBudgetChoose->setText("√");
    }
}

