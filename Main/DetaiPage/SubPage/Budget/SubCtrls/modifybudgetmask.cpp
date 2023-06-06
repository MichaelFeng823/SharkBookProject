#include "modifybudgetmask.h"
#include "ui_modifybudgetmask.h"
#include <QPainter>

ModifyBudgetMask::ModifyBudgetMask(TypeBudget type,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyBudgetMask),m_BudgetType(type)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    setEditButtonText();
    setClearButtonText();
    initSlots();
}

ModifyBudgetMask::~ModifyBudgetMask()
{
    delete ui;
}
void ModifyBudgetMask::setEditButtonText()
{
    QString str;
    if(m_BudgetType == TypeBudget::MonthlyBudget)
        str = "编辑月度总预算";
    else if(m_BudgetType == TypeBudget::YearBudget)
        str = "编辑年度总预算";
    ui->ButtonEdit->setText(str);
}
void ModifyBudgetMask::setClearButtonText()
{
    QString str;
    if(m_BudgetType == TypeBudget::MonthlyBudget)
        str = "清除月度总预算";
    else if(m_BudgetType == TypeBudget::YearBudget)
        str = "清除年度总预算";
    ui->ButtonClear->setText(str);
}
void ModifyBudgetMask::initSlots()
{
    connect(ui->ButtonEdit,&QPushButton::clicked,this,&ModifyBudgetMask::onButtonEditClicked);
    connect(ui->ButtonClear,&QPushButton::clicked,this,&ModifyBudgetMask::onButtonClearClicked);
    connect(ui->ButtonCancel,&QPushButton::clicked,this,&ModifyBudgetMask::onButtonCancelClicked);
}
void ModifyBudgetMask::onButtonEditClicked()
{
    emit editBudget(m_BudgetType);
}
void ModifyBudgetMask::onButtonClearClicked()
{
    emit clearBudget(m_BudgetType);
}
void ModifyBudgetMask::onButtonCancelClicked()
{
    this->close();
}
//重写绘画事件
void ModifyBudgetMask::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.fillRect(this->rect(),QColor(0,0,0,125));
}

