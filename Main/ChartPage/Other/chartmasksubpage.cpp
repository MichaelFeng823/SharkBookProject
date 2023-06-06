#include "chartmasksubpage.h"
#include "ui_chartmasksubpage.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include <QPainter>
using namespace ScreenFunc;
ChartMaskSubPage::ChartMaskSubPage(InOrOut type,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartMaskSubPage),m_Type(type)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(getScreenSize().width(),getScreenSize().height()*0.8);
    ui->ExpandWidget->installEventFilter(this);
    ui->IncomeWidget->installEventFilter(this);
    ckeckBudgetType();
}

ChartMaskSubPage::~ChartMaskSubPage()
{
    delete ui;
}
//事件过滤器
bool ChartMaskSubPage::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        m_IsClicked = true;
    }

    if(event->type() == QEvent::MouseButtonRelease)
    {
       if(m_IsClicked){
           if(obj == ui->ExpandWidget){
               setType(InOrOut::Expand);
           }
           if(obj == ui->IncomeWidget){
               setType(InOrOut::Income);
           }
           emit sendButtonType(m_Type);
           ckeckBudgetType();
           this->close();
       }
       m_IsClicked = false;
    }
    return QWidget::eventFilter(obj, event);
}
//重写绘画事件
void ChartMaskSubPage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.fillRect(this->rect(),QColor(0,0,0,80));
}
//检查预算类型
void ChartMaskSubPage::ckeckBudgetType()
{
    ui->label_ExpandChoose->clear();
    ui->label_InComeChooose->clear();
    if(m_Type == InOrOut::Expand){
         ui->label_ExpandChoose->setText("√");
    }
    else if(m_Type == InOrOut::Income){
         ui->label_InComeChooose->setText("√");
    }
}
