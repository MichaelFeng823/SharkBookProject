#include "budgetsubpage.h"
#include "ui_budgetsubpage.h"
#include <QPointer>
#include <QStandardItemModel>
#include "Kit/LogInfo/clog.h"
#include "Main/DetaiPage/SubPage/Budget/addbudgetpane.h"
#include "Controler/PublicApi/PublicDbFunc.h"


using namespace ScreenFunc;

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
    initDateIdContent();
    initMap();
    setTitleByBudgetType();
    CheckBeforeOpen();
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
}
//收到关闭页面请求
void BudGetSubPage::onRecieveCloseRequest(bool status)
{
    if(status){
        LOG("关闭页面");
        this->close();
        delete  this;
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
   if(m_IsInitMonthBudget)
       initMonthlyBudgetWidgetContent();
   showMonthlyBudgetWidget();

}
 //打开年预算界面
void BudGetSubPage::openAnnualBudgetWidget()
{
    if(m_IsInitAnnualBudget)
        initAnnualBudgetWidgetContent();
    showAnnualBudgetWidget();
}
//显示月预算界面
void BudGetSubPage::showMonthlyBudgetWidget()
{
    ui->widget_AnnualTypeBudget->setVisible(false);
    ui->widget_MonthlyTypeBudget->setVisible(true);
    ui->bottomwidget->setVisible(false);
    QGridLayout * layout = (QGridLayout*)ui->backgroundwidget->layout();
    layout->setRowStretch(0,1);
    layout->setRowStretch(1,0);
    layout->setRowStretch(2,0);
    layout->setRowStretch(3,20);
    updateDataOnPage();
}
//显示年预算界面
void BudGetSubPage::showAnnualBudgetWidget()
{
    ui->widget_AnnualTypeBudget->setVisible(true);
    ui->widget_MonthlyTypeBudget->setVisible(false);
    ui->bottomwidget->setVisible(false);
    QGridLayout * layout = (QGridLayout*)ui->backgroundwidget->layout();
    layout->setRowStretch(0,1);
    layout->setRowStretch(1,0);
    layout->setRowStretch(2,20);
    layout->setRowStretch(3,0);
    updateDataOnPage();
}
//打开添加预算界面
void BudGetSubPage::openAddBudgetPane()
{
    LOG("打开添加预算界面");
    QPointer<AddBudgetPane> pointer;
    pointer = new AddBudgetPane(m_TypeBudget,this);
    connect(pointer,&AddBudgetPane::sendSetBudgetData,this,&BudGetSubPage::onRecieveBudgetBySet);

}
//初始化月预算界面内容
void BudGetSubPage::initMonthlyBudgetWidgetContent()
{
     m_MonthBudgetTableview = new DetialTableview;
     QGridLayout * layout = new QGridLayout;
     layout->addWidget(m_MonthBudgetTableview);
     layout->setMargin(0);
     ui->widget_MonthlyTypeBudget->setLayout(layout);
     initTableViewContent(m_MonthBudgetTableview,3);
     m_IsInitMonthBudget = false;
     LOG("初始化月预算界面内容");
}
//初始化年预算界面内容
void BudGetSubPage::initAnnualBudgetWidgetContent()
{
     m_AnnualBudgetTableview = new DetialTableview;
     QGridLayout * layout = new QGridLayout;
     layout->setMargin(0);
     layout->addWidget(m_AnnualBudgetTableview);
     ui->widget_AnnualTypeBudget->setLayout(layout);
     initTableViewContent(m_AnnualBudgetTableview,3);
     m_IsInitAnnualBudget = false;
     LOG("初始化年预算界面内容");
}
//初始化表格内容
void BudGetSubPage::initTableViewContent(DetialTableview * tableview,int rowcounts)
{
    m_BudgetItemMap[m_TypeBudget].clear();
    QStandardItemModel * model = new QStandardItemModel();
    tableview->setModel(model);
    model->setColumnCount(1);
    model->setRowCount(rowcounts);
    tableview->setColumnWidth(0,getScreenSize().width());

   for(int i = 0; i < rowcounts; i++){
       tableview->setRowHeight(i,430);
       QModelIndex index = model->index(i,0);
       QPointer<BudgetSubItem> item = new BudgetSubItem(m_TypeBudget);
       item->setBudgetData(m_DateId[int(m_TypeBudget)],m_Budget[(int)m_TypeBudget],m_Expand[int(m_TypeBudget)]);
       tableview->setIndexWidget(index,item);
       m_BudgetItemMap[m_TypeBudget].append(item);
   }
}
//初始化日期ID内容
void BudGetSubPage::initDateIdContent()
{
    QDate date = QDate::currentDate();
    m_DateId[int(TypeBudget::MonthlyBudget)] = date.month();
    m_DateId[int(TypeBudget::YearBudget)] = date.year();
}
//初始化map容器
void BudGetSubPage::initMap()
{
    m_BudgetItemMap[TypeBudget::MonthlyBudget] = monthlyBudgetItemVector;
    m_BudgetItemMap[TypeBudget::YearBudget] = AnnualBudgetItemVector;
}
//更新页面上的数据
void BudGetSubPage::updateDataOnPage()
{
    QVector<QPointer<BudgetSubItem>> iter = m_BudgetItemMap[m_TypeBudget];
    for(int i = 0; i < iter.size(); i++){
        iter[i]->updateData();
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


        }
    }
    else if(m_TypeBudget == TypeBudget::YearBudget){
       if(m_IsEmptyOfAnuualBudget){
           openAddBudgetPane();
       }
    }
}

//收到设置预算数据槽函数
void BudGetSubPage::onRecieveBudgetBySet(TypeBudget type,double data)
{
    bool savedatatoDB = false;
    if(type == TypeBudget::MonthlyBudget){
        //存入月预算数据
        m_IsEmptyOfMonthltyBudget = !m_IsEmptyOfMonthltyBudget;
        m_MonthlyBudget = data;


    }else if(TypeBudget::YearBudget){
        //存入年预算数据
        m_IsEmptyOfAnuualBudget = !m_IsEmptyOfAnuualBudget;
        m_AnnualBudget = data;
    }
    m_Budget[(int)type] = data;
    CheckBeforeOpen();
    LOG("onRecieveBudgetBySet:%s",QString::number(data).toStdString().c_str());
}


