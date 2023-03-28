#include "inandoutdetailpage.h"
#include "ui_inandoutdetailpage.h"
#include "Util/clog.h"
#include "Controler/PublicDbFunc.h"
#include <QPointer>
#include <QVector>
#include <algorithm>
#include "Kit/billdetailitemeditsubpage.h"
using namespace  ScreenFunc;

InAndOutDetailPage::InAndOutDetailPage(QVector<BillTableStruct> BillList,QDate date,double num,InAndOutType inandouttype,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InAndOutDetailPage)
{
    ui->setupUi(this);
    m_BillList = BillList;
    m_inandouttype = inandouttype;
    initLayout();
    setTitleName(m_inandouttype);
    setTitlenum(num);
    setTitleDate(date);
    removeDifferentFromInAndOutType();
    customSort();
    initTableview();
    this->show();
}
 //初始化布局
void InAndOutDetailPage::initLayout()
{
    connect(ui->returnButton,&QPushButton::clicked,this,&InAndOutDetailPage::onReturnButtonClicked);
    connect(ui->sortBynum_Button,&QPushButton::clicked,this,&InAndOutDetailPage::onSortByMoneyButtonClicked);
    connect(ui->sortBytime_Button,&QPushButton::clicked,this,&InAndOutDetailPage::onSortByTimeButtonClicked);
    model = new QStandardItemModel();
    ui->detailview->setModel(model);
    this->raise();
}

//初始化表格内容
void InAndOutDetailPage::initTableview()
{
    model->clear();
    model->setColumnCount(1);
    model->setRowCount(m_BillList.size());
    ui->detailview->setColumnWidth(0,getScreenSize().width());

    if(m_BillList.isEmpty()){
         ui->sortBynum_Button->setVisible(false);
         ui->sortBytime_Button->setVisible(false);
         ui->detailview->setStyleSheet("QTableView, QHeaderView, QTableView::item {background: white;} QTableView::item:selected { /*被选中的index*/color: black;background: white;} QTableView{border-image:url(:/public/image/tableview_NoData.jpg);}");
    }
    else{
        ui->sortBynum_Button->setVisible(true);
        ui->sortBytime_Button->setVisible(true);
        ui->detailview->setStyleSheet("QTableView, QHeaderView, QTableView::item {background: white;} QTableView::item:selected { /*被选中的index*/color: black;background: white;}");
    }
   for(int i = 0; i < m_BillList.size(); i++){
       ui->detailview->setRowHeight(i,getScreenSize().height()/12);
       QModelIndex index = model->index(i,0);
       QPointer<BillDataItemWithProcessBar> item = new BillDataItemWithProcessBar(m_BillList[i]);
       item->setPercent(calculatePercent(m_BillList[i].moneyAmount));
       m_Itemlist.append(item);
       connect(item,&BillDataItemWithProcessBar::getDetailBillInfo,this,&InAndOutDetailPage::receiveGetBillInfoSignal);
       ui->detailview->setIndexWidget(index,item);
       LOG("billno:%d",m_BillList[i].billNo);
   }
}
//收到申请查看账单item详情子页面
void InAndOutDetailPage::receiveGetBillInfoSignal(BillTableStruct billinfo)
{
    LOG("receiveGetBillInfoSignal()");
    QPointer<BillDetailItemEditSubPage> pointer = new BillDetailItemEditSubPage(billinfo);
    connect(pointer,&BillDetailItemEditSubPage::callApplyModify,this,&InAndOutDetailPage::onReceiveModifyBillInfoSignal);
    connect(pointer,&BillDetailItemEditSubPage::callApplyDeleteSuccessUpdateParent,this,&InAndOutDetailPage::onReceiveAfterDeleteUpdatePage);
}
 //返回按钮按下槽函数
void InAndOutDetailPage::onReturnButtonClicked()
{
    this->close();
}
//设置标题支出或收入金额
void InAndOutDetailPage::setTitlenum(double num)
{
    ui->label_num->setText(QString::number(num,'f',2));
}
 //设置标题日期
void InAndOutDetailPage::setTitleDate(QDate date)
{
    ui->label_Title_Date->setText(QString("%1年%2月").arg(date.year()).arg(date.month()));
}
//设置TitleName(本月xxx)
void InAndOutDetailPage::setTitleName(InAndOutType type)
{
    QString titlestr;
    QString rankstr;
    if(type == InAndOutType::InType){
        titlestr = QString("本月总收入");
        rankstr = QString("单笔收入排行");
    }
    else if(type == InAndOutType::OutType){
        titlestr = QString("本月总支出");
        rankstr = QString("单笔支出排行");
    }
    ui->label_InorOut_Type->setText(titlestr);
    ui->label_singelnumrank_subtitle->setText(rankstr);
}
 //自定义排序
void InAndOutDetailPage::customSort()
{
    if(m_sorttype == SortType::sortByMoney){
        std::sort(m_BillList.begin(),m_BillList.end(),InAndOutDetailPage::sortByMoney);
    }
    else if(m_sorttype == SortType::sortByTime){
        std::sort(m_BillList.begin(),m_BillList.end(),InAndOutDetailPage::sortByTime);
    }
}
 //通过金额排序
bool InAndOutDetailPage::sortByMoney(const BillTableStruct & billfirst,const BillTableStruct & billsecond)
{
    return billfirst.moneyAmount > billsecond.moneyAmount;
}

//通过时间排序
bool InAndOutDetailPage::sortByTime(const BillTableStruct & billfirst,const BillTableStruct & billsecond)
{
    return billfirst.date > billsecond.date;
}
//去除不同类型的数据
void InAndOutDetailPage::removeDifferentFromInAndOutType()
{
    LOG("removeDifferentFromInAndOutType");
    LOG("m_BillList.size():%d",m_BillList.size());
    QVector<BillTableStruct> billlist;
    for(int i = 0 ; i < m_BillList.size(); i++){
        if(m_BillList[i].InOrOut == m_inandouttype)
           billlist.append(m_BillList[i]);
    }
    m_BillList.clear();
    m_BillList = billlist;
}

//计算占比比值
double InAndOutDetailPage::calculatePercent(double moneynum)
{
    double MoneyCount = 0;
    for(int i = 0; i < m_BillList.size(); i++){
        MoneyCount += m_BillList[i].moneyAmount;
    }
    double percent = moneynum/MoneyCount;
    return percent;
}
InAndOutDetailPage::~InAndOutDetailPage()
{
    LOG("~InAndOutDetailPage()");
    delete ui;
}

//按金额按钮按下槽函数
void InAndOutDetailPage::onSortByMoneyButtonClicked()
{
    ui->sortBynum_Button->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(200, 200, 200,100);font-size:12pt;");
    ui->sortBytime_Button->setStyleSheet("color: rgb(0, 0, 0);background-color: rgb(255,255,255);font-size:12pt;");
    m_sorttype = SortType::sortByMoney;
    customSort();
    initTableview();
}
//按时间按钮按下槽函数
void InAndOutDetailPage::onSortByTimeButtonClicked()
{
    ui->sortBynum_Button->setStyleSheet("color: rgb(0, 0, 0);background-color: rgb(255,255,255);font-size:12pt;");
    ui->sortBytime_Button->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(200, 200, 200,100);font-size:12pt;");
    m_sorttype = SortType::sortByTime;
    customSort();
    initTableview();
}
//收到修改账单信息的信号
void InAndOutDetailPage::onReceiveModifyBillInfoSignal(BillTableStruct billinfo)
{
    if(bookkeepingselectarea != nullptr)
        delete  bookkeepingselectarea;
    bookkeepingselectarea = new BookkeepingSelectArea(billinfo,true);
    connect(bookkeepingselectarea,&BookkeepingSelectArea::sendSignalToUpdateDetailBillPage,this,&InAndOutDetailPage::onReceiveUpdateDetailBiilPageSignal);
}
//收到更新BillPage槽函数
void InAndOutDetailPage::onReceiveUpdateDetailBiilPageSignal(BillTableStruct billinfo)
{
    LOG("收到更新BillPage槽函数");
    callApplyUpdateBillPage(true);
    receiveGetBillInfoSignal(billinfo);
    if(bookkeepingselectarea != nullptr)
        delete  bookkeepingselectarea;
    bookkeepingselectarea = nullptr;
    this->close();
}
//收到在删除成功后刷新页面槽函数
void InAndOutDetailPage::onReceiveAfterDeleteUpdatePage(bool status)
{
    if(status){
        LOG("收到在删除成功后刷新页面槽函数");
        callApplyUpdateBillPage(true);
        this->close();
    }
}
