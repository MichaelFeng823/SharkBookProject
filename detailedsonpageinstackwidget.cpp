#include "detailedsonpageinstackwidget.h"
#include "ui_detailedsonpageinstackwidget.h"
#include <QModelIndex>
#include <QTouchEvent>
#include <QScrollBar>
#include "Util/clog.h"
#include "Controler/PublicDbFunc.h"
#include "Controler/databaseobj.h"
#include "inandoutdetailpage.h"
#include "Kit/billsubarea.h"
#include "Kit/budgetsubpage.h"
#include "Kit/assetmanagerpage.h"
#include <QPointer>
using namespace ScreenFunc;
using namespace DataQuery;
DetailedSonPageInStackwidget::DetailedSonPageInStackwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetailedSonPageInStackwidget)
{
    ui->setupUi(this);
    hasConnected = DataBaseObj::getConnecttion(coon);
    buildUiLayout();
    buildConnect();
    m_CurrentDate = QDate::currentDate();
    getBilldata();
    initTableview();
    initInAndOutKitContent();
}

///****************************************************************************
/// @data    :
/// @input   :
/// @output  :
/// @brief   :  构建UI布局
///****************************************************************************
 void DetailedSonPageInStackwidget::buildUiLayout()
 {
     //------------------------------收入支出部分---------------------------------
     calendarkit = new CalendarKit(ui->InAndOutShowwidget);
     connect(calendarkit,&CalendarKit::sendSetDateSignal,this,&DetailedSonPageInStackwidget::receiveSetDateInfo);
     inkit = new InAndOutKit(InAndOutType::InType,ui->InAndOutShowwidget);
     connect(inkit,&InAndOutKit::callGetInorOutData,this,&DetailedSonPageInStackwidget::receiveCallGetInAndOutInfo);
     outkit= new InAndOutKit(InAndOutType::OutType,ui->InAndOutShowwidget);
     connect(outkit,&InAndOutKit::callGetInorOutData,this,&DetailedSonPageInStackwidget::receiveCallGetInAndOutInfo);
     inAndoutlayout.addWidget(calendarkit,0,0);
     inAndoutlayout.addWidget(inkit,0,1);
     inAndoutlayout.addWidget(outkit,0,2);
     inAndoutlayout.setColumnStretch(0,2);
     inAndoutlayout.setColumnStretch(1,4);
     inAndoutlayout.setColumnStretch(2,4);
     inAndoutlayout.setSpacing(0);
     inAndoutlayout.setMargin(0);
     ui->InAndOutShowwidget->setLayout(&inAndoutlayout);
     //------------------------------tableview部分-------------------------------
     tableview = new DetialTableview;
     model = new QStandardItemModel();
     tableview->setModel(model);
     tableviewlayout.setSpacing(0);
     tableviewlayout.setMargin(0);
     tableview->setLayout(&tableviewlayout);
     tableview->installEventFilter(this);
     //--------------------------------整体部分-----------------------------------
     outterlayout.addWidget(ui->titlewidget,0,0);
     outterlayout.addWidget(ui->InAndOutShowwidget,1,0);
     outterlayout.addWidget(ui->toolwidget,2,0);
     outterlayout.addWidget(tableview,3,0);
     outterlayout.setRowStretch(0,2);
     outterlayout.setRowStretch(1,3);
     outterlayout.setRowStretch(2,3);
     outterlayout.setRowStretch(3,22);
     outterlayout.setSpacing(0);
     outterlayout.setMargin(0);
     //--------------------------------设置整体布局--------------------------------
     this->setLayout(&outterlayout);
 }

 //建立槽函数连接
 void DetailedSonPageInStackwidget::buildConnect()
 {
     connect(ui->pushButton_Bill,&QPushButton::clicked,this,&DetailedSonPageInStackwidget::onBillSubButtonClicked);
     connect(ui->pushButton_Budget,&QPushButton::clicked,this,&DetailedSonPageInStackwidget::onBudgetButtonClicked);
     connect(ui->pushButton_AssetManager,&QPushButton::clicked,this,&DetailedSonPageInStackwidget::onAssetManagerButtonClicked);
 }
//初始化表格内容
void DetailedSonPageInStackwidget::initTableview()
{
     model->setColumnCount(1);
     model->setRowCount(m_BillList.size());
     tableview->setColumnWidth(0,getScreenSize().width());
     std::sort(m_BillList.begin(),m_BillList.end(),DetailedSonPageInStackwidget::sortBillInfoByDate);
     if(m_BillList.isEmpty())
         tableview->setStyleSheet("QTableView, QHeaderView, QTableView::item {background: white;} QTableView::item:selected { /*被选中的index*/color: black;background: white;} QTableView{border-image:url(:/public/image/tableview_NoData.jpg);}");
     else
         tableview->setStyleSheet("QTableView, QHeaderView, QTableView::item {background: white;} QTableView::item:selected { /*被选中的index*/color: black;background: white;}");

    for(int i = 0; i < m_BillList.size(); i++){
        tableview->setRowHeight(i,getScreenSize().height()/32*30/30*22/6);
        QModelIndex index = model->index(i,0);
        QPointer<BillDataItem> item = new BillDataItem;
        item->setDate(m_BillList[i].date);
        item->setTitlePayNum(m_BillList[i].moneyAmount);
        item->setPayType("餐饮");
        item->setDetialPayNum(m_BillList[i].moneyAmount);
        item->setPayDetailSoucres(m_BillList[i].remarks);
        tableview->setIndexWidget(index,item);
    }
}
//获取账单数据
void DetailedSonPageInStackwidget::getBilldata()
{
      QSqlQuery query(coon);
      billQurey(query,UserInfo::UserID,m_CurrentDate,m_BillList);
      billQureyOfYear(query,UserInfo::UserID,m_CurrentDate,m_BillListOfYear);
}
bool DetailedSonPageInStackwidget::eventFilter(QObject *obj, QEvent *event)
{
    if(tableview == obj)
    {
        QTouchEvent *touchevent = (QTouchEvent*)event;
        if(event->type() == QEvent::TouchBegin)
        {
            event->accept();
            m_TempPoint = touchevent->touchPoints().first().scenePos().toPoint();
            return true;
        }
        if(event->type() == QEvent::TouchUpdate)
        {
            event->accept();

            QList<QTouchEvent::TouchPoint> touchUpdatePoints = touchevent->touchPoints();
            //  假设此时场景，手指滑动了
            QPoint nowPoint = touchUpdatePoints.at(0).screenPos().toPoint();  // screenPos 当前坐标的位置
            // endPoint = nowPoint;
            QScrollBar *  vbar   = tableview->verticalScrollBar();
            auto offset = m_TempPoint.y() - nowPoint.y();
            //获取当前滚动条位置
            auto val    = vbar->value();
            //获取滚动条pagetemp区域高度
            auto step   = vbar->pageStep();
            //设计移动位置
            auto move   = offset * step / this->height();
            //if(move + val < 0 || move + val > ui->textEdit->height()) return false;
            //移动
            vbar->setValue(move+val);
            return true;
        }
        if(event->type() == QEvent::TouchCancel)
        {
            event->accept();
            return false;
        }
        if(event->type() == QEvent::TouchEnd)
        {
           event->accept();
            return false;
        }

         return false;
    }
     return QWidget::eventFilter(obj, event);
}
//初始化收入支出控件内容
void DetailedSonPageInStackwidget::initInAndOutKitContent()
{
     m_Current_InNumber = 0.00;                //当前收入数据
     m_Current_OutNumber = 0.00;               //当前支出数据
     for(int i = 0; i < m_BillList.size();i++){
         if(m_BillList[i].InOrOut == 1)
             m_Current_OutNumber+=m_BillList[i].moneyAmount;
         else if(m_BillList[i].InOrOut == 2)
             m_Current_OutNumber+=m_BillList[i].moneyAmount;
     }

     inkit->setnum(QString::number(m_Current_InNumber,'f',2));
     outkit->setnum(QString::number(m_Current_OutNumber,'f',2));
}
 //收到设置日期信息槽函数
void DetailedSonPageInStackwidget::receiveSetDateInfo(int year,int month)
{
    LOG("receive setDateInfo Func!");
    m_CurrentDate.setDate(year,month,1);
    getBilldata();                        //获取账单数据
    initTableview();                      //初始化账单表格内容
    initInAndOutKitContent();             //初始化收入支出控件内容
}
//刷新账单界面
void DetailedSonPageInStackwidget::UpdateBillContent()
{
    LOG("update BillContent Func!");
    getBilldata();                        //获取账单数据
    initTableview();                      //初始化账单表格内容
    initInAndOutKitContent();             //初始化收入支出控件内;
}
//通过日期排序账单信息
bool DetailedSonPageInStackwidget::sortBillInfoByDate(const BillTableStruct & billfirst,const BillTableStruct & billsecond)
{
    return billfirst.date > billsecond.date;
}
//刷新账单详情界面
void DetailedSonPageInStackwidget::UpdateBillDetailPage()
{
    if(m_currentOpenType == InAndOutType::InType){
        receiveCallGetInAndOutInfo(m_currentOpenType,m_Current_InNumber);
    }
    else if(m_currentOpenType == InAndOutType::OutType){
        receiveCallGetInAndOutInfo(m_currentOpenType,m_Current_OutNumber);
    }
}
//收到获取账单详情信息
void DetailedSonPageInStackwidget::receiveCallGetInAndOutInfo(int type,double num)
{
    QPointer<InAndOutDetailPage> pointer;
    m_currentOpenType = (InAndOutType)type;
    if(type == InAndOutType::InType){
        pointer= new InAndOutDetailPage(m_BillList,m_CurrentDate,num,InAndOutType::InType);
    }else if(type == InAndOutType::OutType){
        pointer = new InAndOutDetailPage(m_BillList,m_CurrentDate,num,InAndOutType::OutType);
    }
    connect(pointer,&InAndOutDetailPage::callApplyUpdateBillPage,this,&DetailedSonPageInStackwidget::receiveUpdateBillInfoSignal);
}
//收到刷新账单信息的信号
void DetailedSonPageInStackwidget::receiveUpdateBillInfoSignal(bool status)
{
    if(status){
        UpdateBillContent();
        UpdateBillDetailPage();
    }
}
//打开账单子界面槽函数
void DetailedSonPageInStackwidget::onBillSubButtonClicked()
{
    QPointer<BillSubArea> pointer;
    pointer = new BillSubArea(m_BillListOfYear);
}
//打开预算子界面槽函数
void DetailedSonPageInStackwidget::onBudgetButtonClicked()
{
    QPointer<BudGetSubPage> pointer;
    pointer = new BudGetSubPage();
}
//打开资产管理子界面槽函数
void DetailedSonPageInStackwidget::onAssetManagerButtonClicked()
{
    QPointer<AssetManagerPage> pointer;
    pointer = new AssetManagerPage();
}
DetailedSonPageInStackwidget::~DetailedSonPageInStackwidget()
{
    delete ui;
}
