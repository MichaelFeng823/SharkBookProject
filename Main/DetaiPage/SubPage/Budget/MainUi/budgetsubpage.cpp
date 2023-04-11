#include "budgetsubpage.h"
#include "ui_budgetsubpage.h"
#include <QPointer>
#include "Kit/LogInfo/clog.h"
#include "Main/DetaiPage/SubPage/Budget/addbudgetpane.h"

BudGetSubPage::BudGetSubPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BudGetSubPage)
{
    ui->setupUi(this);
    ui->label_title->installEventFilter(this);
    ui->label_Quit->installEventFilter(this);
    ui->label_Menu->installEventFilter(this);
    connect(this,&BudGetSubPage::expandOrClose,this,&BudGetSubPage::onRecieveExpandOrCloseSignal);
    connect(this,&BudGetSubPage::requestMenuSignal,this,&BudGetSubPage::onRecieveMenuRequest);
    connect(this,&BudGetSubPage::requestCloseSignal,this,&BudGetSubPage::onRecieveCloseRequest);
    connect(ui->pushButton_AddBudget,&QPushButton::clicked,this,&BudGetSubPage::onAddBudgetClicked);
    setTitleByBudgetType();
    CheckBeforeOpen();
//    m_MonthlBudgetWidget = new QWidget();    //月预算界面widget
//    QLayout * layout = ui->backgroundwidget->layout();
//    layout->addWidget(m_MonthlBudgetWidget);
//    m_AnnualBudgetWidget = new QWidget();    //年预算界面widget
//    layout->addWidget(m_AnnualBudgetWidget);
//    m_MonthlBudgetWidget->setVisible(false);
//    m_AnnualBudgetWidget->setVisible(false);
    ui->widget_AnnualTypeBudget->setVisible(false);
    ui->widget_MonthlyTypeBudget->setVisible(false);
    this->raise();
    this->show();
}

BudGetSubPage::~BudGetSubPage()
{
    delete ui;
}

//事件过滤器
bool BudGetSubPage::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(obj == ui->label_title | obj == ui->label_Menu | obj == ui->label_Quit){
           m_IsClicked = true;
        }
    }

    if(event->type() == QEvent::MouseButtonRelease)
    {
        if(obj == ui->label_title){
            if(m_IsClicked){
                m_IsExpand == false ? m_IsExpand = true : m_IsExpand = false;
                emit expandOrClose(m_IsExpand);
            }
            m_IsClicked = false;
        }

        if(obj == ui->label_Menu){
            if(m_IsClicked)
                emit requestMenuSignal(true);
        }

        if(obj == ui->label_Quit){
            if(m_IsClicked)
                emit requestCloseSignal(true);
        }
    }
    return QWidget::eventFilter(obj, event);
}

//收到展开或者关闭的信号槽函数
void BudGetSubPage::onRecieveExpandOrCloseSignal(bool isExpandOrClose)
{
    if(isExpandOrClose){
        m_Mask = new BudgetMaskSubpage(m_TypeBudget,this);
        connect(m_Mask,&BudgetMaskSubpage::sendTypeBudget,this,&BudGetSubPage::onRecieveSetBudgetType);
        m_Mask->hide();
        QPoint pos = ui->titlewidget->pos();
        LOG("pos:x %d y %d",pos.x(),pos.y()+ui->titlewidget->height());
        m_Mask->move(pos.x(),pos.y()+ui->titlewidget->height());
        m_Mask->show();
    }
    else{
        m_Mask->close();
    }
}
//收到设置打开预算界面类型槽函数
void BudGetSubPage::onRecieveSetBudgetType(TypeBudget type)
{
    m_IsExpand == false ? m_IsExpand = true : m_IsExpand = false;
    m_TypeBudget = type;
    setTitleByBudgetType();
    CheckBeforeOpen();
    //if(m_TypeBudget == TypeBudget::MonthlyBudget)
    //    openMonthlyBudgetWidget();
    //else if(m_TypeBudget == TypeBudget::YearBudget)
    //    openAnnualBudgetWidget();
}
//收到关闭页面请求
void BudGetSubPage::onRecieveCloseRequest(bool status)
{
    if(status){
        LOG("关闭页面");
        this->close();
    }
}
//收到打开菜单请求
void BudGetSubPage::onRecieveMenuRequest(bool status)
{
    if(status){
        LOG("打开菜单");
    }
}
//打开月预算界面
void BudGetSubPage::openMonthlyBudgetWidget()
{
    ui->widget_AnnualTypeBudget->setVisible(false);
    ui->widget_MonthlyTypeBudget->setVisible(true);
    ui->bottomwidget->setVisible(false);
//    m_MonthlBudgetWidget->setVisible(true);
//    m_AnnualBudgetWidget->setVisible(false);
   QGridLayout * layout = (QGridLayout*)ui->backgroundwidget->layout();
   layout->setRowStretch(0,1);
   layout->setRowStretch(1,0);
   layout->setRowStretch(2,0);
   layout->setRowStretch(3,20);
   //layout->setRowStretch(1,1);
   //layout->setRowStretch(1,1);

//    initMonthlyBudgetWidgetContent();
}
 //打开年预算界面
void BudGetSubPage::openAnnualBudgetWidget()
{
    ui->widget_AnnualTypeBudget->setVisible(true);
    ui->widget_MonthlyTypeBudget->setVisible(false);
    ui->bottomwidget->setVisible(false);
    QGridLayout * layout = (QGridLayout*)ui->backgroundwidget->layout();
    layout->setRowStretch(0,1);
    layout->setRowStretch(1,0);
    layout->setRowStretch(2,20);
    layout->setRowStretch(3,0);
    //layout->setRowStretch(1,1);
    //layout->setRowStretch(1,1);

//    m_MonthlBudgetWidget->setVisible(false);
//    m_AnnualBudgetWidget->setVisible(true);
//    QLayout * layout = ui->bottomwidget->layout();
//    layout->setEnabled(false);
//    initAnnualBudgetWidgetContent();
}
//打开添加预算界面
void BudGetSubPage::openAddBudgetPane()
{
    LOG("打开添加预算界面");
    QPointer<AddBudgetPane> pointer;
    pointer = new AddBudgetPane(m_TypeBudget,this);
}
//初始化月预算界面内容
void BudGetSubPage::initMonthlyBudgetWidgetContent()
{
     if(!m_MonthlBudgetWidget.isNull()){
         QLabel * label = new QLabel("月预算界面");
         label->setAlignment(Qt::AlignCenter);
         QGridLayout * layout = new QGridLayout;
         layout->addWidget(label);
         m_MonthlBudgetWidget->setLayout(layout);
         m_MonthlBudgetWidget->setStyleSheet("background:rgb(0,255,255);color:black;fontsize:30px;");
     }
}
//初始化月预算界面内容
void BudGetSubPage::initAnnualBudgetWidgetContent()
{
     if(!m_AnnualBudgetWidget.isNull()){
         QLabel * label = new QLabel("年预算界面");
         label->setAlignment(Qt::AlignCenter);
         QGridLayout * layout = new QGridLayout;
         layout->addWidget(label);
         m_AnnualBudgetWidget->setLayout(layout);
         m_AnnualBudgetWidget->setStyleSheet("background:rgb(255,0,255);color:black;fontsize:30px;");
     }
}
//设置标题内容根据预算类型
void BudGetSubPage::setTitleByBudgetType()
{
    if(m_TypeBudget == TypeBudget::MonthlyBudget){
        ui->label_title->setText("月预算V");
    }
    else if(m_TypeBudget == TypeBudget::YearBudget){
        ui->label_title->setText("年预算V");
    }
}
//打开页面前检测打开的该类型是否为空
void BudGetSubPage::CheckBeforeOpen()
{
    if(m_TypeBudget == TypeBudget::MonthlyBudget){
        if(m_IsEmptyOfMonthltyBudget){
            openAddBudget();
        }
        else{
            openMonthlyBudgetWidget();
        }
    }
    else if(m_TypeBudget == TypeBudget::YearBudget){
       if(m_IsEmptyOfAnuualBudget){
           openAddBudget();
       }
       else
       {
           openAnnualBudgetWidget();
       }
    }
}
//打开添加预算界面
void BudGetSubPage::openAddBudget()
{
    ui->widget_AnnualTypeBudget->setVisible(false);
    ui->widget_MonthlyTypeBudget->setVisible(false);
    ui->bottomwidget->setVisible(true);
    QGridLayout * layout = (QGridLayout*)ui->backgroundwidget->layout();
    layout->setRowStretch(0,1);
    layout->setRowStretch(1,20);
    layout->setRowStretch(2,0);
    layout->setRowStretch(3,0);
    onAddBudgetClicked();
}
//点击添加预算槽函数
void BudGetSubPage::onAddBudgetClicked()
{
    if(m_TypeBudget == TypeBudget::MonthlyBudget){
        if(m_IsEmptyOfMonthltyBudget){
            openAddBudgetPane();
            //m_IsEmptyOfMonthltyBudget = !m_IsEmptyOfMonthltyBudget;
        }
    }
    else if(m_TypeBudget == TypeBudget::YearBudget){
       if(m_IsEmptyOfAnuualBudget){
           openAddBudgetPane();
           //m_IsEmptyOfAnuualBudget = !m_IsEmptyOfAnuualBudget;
       }
    }
    //CheckBeforeOpen();
}
