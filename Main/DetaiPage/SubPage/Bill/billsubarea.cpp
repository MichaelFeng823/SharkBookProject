#include "billsubarea.h"
#include "ui_billsubarea.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include "Controler/DataControl/databaseobj.h"
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include <QPushButton>
using namespace DataQuery;
using namespace ScreenFunc;
BillSubArea::BillSubArea(QVector<BillTableStruct> billinfo,QWidget *parent) :
    QWidget(parent),m_BillInfo(billinfo),
    ui(new Ui::BillSubArea)
{
    ui->setupUi(this);
    setDefaultYear();
    statisticsCount();
    calculateIncomeCount();
    calculateExpandCount();
    calculateBalanceCount();
    initModel();
    initTableStyle();
    initTableView();
    builtConnect();
    hasConnected = DataBaseObj::getConnecttion(coon);
    ui->label_Quit->installEventFilter(this);
    ui->label_Menu->installEventFilter(this);
    ui->label_year->installEventFilter(this);
    ui->label_year_arrow->installEventFilter(this);
    this->raise();
    this->show();
}
BillSubArea::~BillSubArea()
{
    delete ui;
}
//初始化Model
void BillSubArea::initModel()
{
    model = new QStandardItemModel();
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
}
//初始化表格样式
void BillSubArea::initTableStyle()
{
    ui->tableView->setStyleSheet("QTableView, QHeaderView, QTableView::item {background: white;}"
                                 "QTableView::item:selected { /*被选中的index*/color: black;background: white;}");
}
//初始化表格
void BillSubArea::initTableView()
{
    model->setColumnCount(5);
    model->setRowCount(m_BillBalanceList.size());
    ui->tableView->setColumnWidth(0,getScreenSize().width()/5);
    ui->tableView->setColumnWidth(1,getScreenSize().width()/5);
    ui->tableView->setColumnWidth(2,getScreenSize().width()/5);
    ui->tableView->setColumnWidth(3,getScreenSize().width()/5);

   for(int i = 0; i < m_BillBalanceList.size(); i++){
       ui->tableView->setRowHeight(i,getScreenSize().height()/15);

       model->setItem(i,0,new QStandardItem(QString("%1月").arg(m_BillBalanceList.at(i).month)));
       model->setItem(i,1,new QStandardItem(QString::number(m_BillBalanceList.at(i).income,'f',2)));
       model->setItem(i,2,new QStandardItem(QString::number(m_BillBalanceList.at(i).expand,'f',2)));
       model->setItem(i,3,new QStandardItem(QString::number(m_BillBalanceList.at(i).cashSurplus,'f',2)));

       QModelIndex index = model->index(i,4);
       QPushButton * button = new QPushButton(">");
       button->setEnabled(false);
       button->setStyleSheet("background-color:rgba(255,255,255,0);color:rgba(140,140,140,150);");

       model->item(i,0)->setTextAlignment(Qt::AlignCenter);
       model->item(i,1)->setTextAlignment(Qt::AlignCenter);
       model->item(i,2)->setTextAlignment(Qt::AlignCenter);
       model->item(i,3)->setTextAlignment(Qt::AlignCenter);

       ui->tableView->setIndexWidget(index,button);
   }
}
//绑定槽函数
void BillSubArea::builtConnect()
{
    connect(this,&BillSubArea::RequestCloseSignal,this,&BillSubArea::onQuitButtonClicked);
    connect(this,&BillSubArea::RequestOpenMenuSignal,this,&BillSubArea::onMenuButtonClicked);
}
//设置当前默认年
void BillSubArea::setDefaultYear()
{
    m_CurrentYear = QDate::currentDate().year();
    setCurrentYear(m_CurrentYear);
}
//设置当前年
void BillSubArea::setCurrentYear(int year)
{
    m_CurrentYear = year;
    ui->label_year->setText(QString("%1年").arg(m_CurrentYear));
}
//设置收入汇总
void BillSubArea::setIncomeCount(double incomeCount)
{
    m_IncomeCount = incomeCount;
    ui->label_IncomeNum->setText(QString::number(m_IncomeCount,'f',2));
}
//设置支出汇总
void BillSubArea::setExpandCount(double expandCount)
{
    m_ExpandCount = expandCount;
    ui->label_ExpandNum->setText(QString::number(m_ExpandCount,'f',2));
}
//设置结余汇总
void BillSubArea::setBalanceCount(double balanceCount)
{
    m_BalanceCount = balanceCount;
    ui->label_FinallyMoneyCounts->setText(QString::number(m_BalanceCount,'f',2));
}
//计算收入汇总
void BillSubArea::calculateIncomeCount()
{
    setIncomeCount(0);
    int IncomeCount = 0;
    for(int i = 0; i < m_BillBalanceList.size(); i++){
        IncomeCount += m_BillBalanceList.at(i).income;
    }
    setIncomeCount(IncomeCount);
}
//计算支出汇总
void BillSubArea::calculateExpandCount()
{
    setExpandCount(0);
    int ExpandCount = 0;
    for(int i = 0; i < m_BillBalanceList.size(); i++){
        ExpandCount += m_BillBalanceList.at(i).expand;
    }
    setExpandCount(ExpandCount);
}
//计算结余
void BillSubArea::calculateBalanceCount()
{
    setBalanceCount(0);
    int BalanceCount = 0;
    for(int i = 0; i < m_BillBalanceList.size(); i++){
        m_BillBalanceList[i].cashSurplus =  m_BillBalanceList.at(i).income - m_BillBalanceList.at(i).expand;
        BalanceCount +=m_BillBalanceList[i].cashSurplus;
    }
    setBalanceCount(BalanceCount);
}
 //统计账单数据
void BillSubArea::statisticsCount()
{
    m_BillBalanceList.clear();
    if(m_BillInfo.size()){
        for(int month = 1; month <= 12; month++){
             BillBalanceStruct billbalancestruct;
             billbalancestruct.month = month;
             for(int j = 0; j < m_BillInfo.size();j++){
                 if(m_BillInfo.at(j).date.year() == m_CurrentYear){
                     if(m_BillInfo.at(j).date.month() == month){
                         if(m_BillInfo.at(j).InOrOut == InOrOut::Income){
                             billbalancestruct.income += m_BillInfo.at(j).moneyAmount;
                         }
                         else if(m_BillInfo.at(j).InOrOut == InOrOut::Expand){
                             billbalancestruct.expand += m_BillInfo.at(j).moneyAmount;
                         }
                         billbalancestruct.isExsit = true;
                     }
                 }
             }
             if(billbalancestruct.isExsit)
                m_BillBalanceList.append(billbalancestruct);
        }
    }
}

//退出按钮点击槽函数
void BillSubArea::onQuitButtonClicked()
{
    this->close();
}
//菜单按钮槽函数
void BillSubArea::onMenuButtonClicked()
{
    LOG("打开菜单");
}
//收到设置年份槽函数
void BillSubArea::onRecieveDateInfo(int year)
{
    setCurrentYear(year);
    getBillData();
    updateDateDisplay();
}
//获取当前年份账单数据
void BillSubArea::getBillData()
{
    QSqlQuery query(coon);
    QDate m_CurrentDate;
    m_CurrentDate.setDate(m_CurrentYear,1,1);
    billQureyOfYear(query,UserInfo::UserID,m_CurrentDate,m_BillInfo);
}
//调用日历控件
void BillSubArea::callCalendar(int year)
{
    m_Calendar = new Calendar(CalendarType::Year,year,this->nativeParentWidget());
    connect(m_Calendar,&Calendar::returnTimeInfoYear,this,&BillSubArea::onRecieveDateInfo);
}
//刷新数据显示
void BillSubArea::updateDateDisplay()
{
    statisticsCount();
    calculateIncomeCount();
    calculateExpandCount();
    calculateBalanceCount();
    initTableView();
}
//事件过滤器
bool BillSubArea::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(obj == ui->label_Quit)
            quitIsClicked = true;
        else if(obj == ui->label_Menu)
            menuIsClicked = true;
        else if(obj == ui->label_year | obj == ui->label_year_arrow)
            calendarIsClicked = true;
    }

    if(event->type() == QEvent::MouseButtonRelease)
    {
        if(obj == ui->label_Quit){
           emit RequestCloseSignal(quitIsClicked);
           quitIsClicked = false;
        }
        else if(obj == ui->label_Menu){
            emit RequestOpenMenuSignal(menuIsClicked);
            menuIsClicked = false;
        }
        else if(obj == ui->label_year | obj == ui->label_year_arrow && calendarIsClicked == true){
            callCalendar(m_CurrentYear);
            calendarIsClicked = false;
        }
    }
    return QWidget::eventFilter(obj, event);
}
