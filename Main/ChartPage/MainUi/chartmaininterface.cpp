#include "chartmaininterface.h"
#include "ui_chartmaininterface.h"
#include <QImage>
#include <QPixmap>
#include <QPicture>
#include <QPointer>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <Kit/LogInfo/clog.h>
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include "NetCore/msgqueue.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include <QMouseEvent>
#include <QPainter>
#include "Main/ChartPage/Other/chartmasksubpage.h"
#include "Controler/DataControl/databaseobj.h"
#include "Main/DetaiPage/SubPage/Income/SubCtrls/billdataitemwithprocessbar.h"
using namespace ScreenFunc;
using namespace DataQuery;
using namespace PublicSort;
ChartMainInterface::ChartMainInterface(QWidget *parent) :
    BaseCustomWidget(parent),
    ui(new Ui::ChartMainInterface)
{
    ui->setupUi(this);
    hasConnected = DataBaseObj::getConnecttion(coon);
    initButtonGroupInChartPage();
    initWeekLayout();
    initMonthLayout();
    initYearLayout();
    initWeekPageCtrls();
    initMonthPageCtrls();
    initYearPageCtrls();
    connect(ui->pushButton_TypeChoose,&QPushButton::clicked,this,&ChartMainInterface::on_TypeChoose_Clicked);

}

ChartMainInterface::~ChartMainInterface()
{
    delete ui;
}
//设置当前Active状态
void ChartMainInterface::setActiveWindow(bool state)
{
    //LOG("ChartMainInterface::setActiveWindow %d",state);
    m_IsActiveWindow = state;
    if(isChartAreaFirstShow && state){
        buttongroup_in_chart->button(ChartSelectType::week)->click();
        weekbar->setDefaultClicked();
        buttongroup_in_chart->button(ChartSelectType::month)->click();
        monthbar->setDefaultClicked();
        buttongroup_in_chart->button(ChartSelectType::year)->click();
        yearbar->setDefaultClicked();
        buttongroup_in_chart->button(ChartSelectType::week)->click();
        isChartAreaFirstShow = false;
    }
    if(state)
         updatePage();
}
//更新整个页面
void ChartMainInterface::updatePage()
{
    m_WeekList = getWeekBills(m_CurrentWeekId);
    m_MonthList = getMonthBills(m_CurrentMonthId);
    m_YearList = getYearBills(m_CurrentYearId);
    updateWeekPage();
    updateMonthPage();
    updateYearPage();
}
//初始化图表中的按钮组
void ChartMainInterface::initButtonGroupInChartPage()
{
    buttongroup_in_chart = new QButtonGroup();
    buttongroup_in_chart->addButton(ui->pushButton_Week,ChartSelectType::week);
    buttongroup_in_chart->addButton(ui->pushButton_Month,ChartSelectType::month);
    buttongroup_in_chart->addButton(ui->pushButton_Year,ChartSelectType::year);
    connect(buttongroup_in_chart,SIGNAL(buttonClicked(int)),this,SLOT(on_ButtonGroup_In_Chart_Clicked(int)));
}
//图表界面中按钮组中按钮点击槽函数
void ChartMainInterface::on_ButtonGroup_In_Chart_Clicked(int pagetype)
{
    for(QAbstractButton * button :buttongroup_in_chart->buttons()){
        setButtonStyleOnNormal((QPushButton*)button);}
    switch (pagetype) {
        case ChartSelectType::week:{
            setButtonStyleAfterClicked((QPushButton*)buttongroup_in_chart->button(pagetype));
            ui->stackedWidget->setCurrentWidget(ui->weekpage);
            updateWeekPage();
            break;}
        case ChartSelectType::month:{
            setButtonStyleAfterClicked((QPushButton*)buttongroup_in_chart->button(pagetype));
            ui->stackedWidget->setCurrentWidget(ui->monthpage);
            updateMonthPage();
            break;};
        case ChartSelectType::year:{
            setButtonStyleAfterClicked((QPushButton*)buttongroup_in_chart->button(pagetype));
            ui->stackedWidget->setCurrentWidget(ui->yearpage);
            updateYearPage();
            break;};
    }
}
//更新周页面
void ChartMainInterface::updateWeekPage()
{
    weekmodel->setBillInfo(m_WeekList);
    weekmodel->updateLoadData();
    setRankInfo(statisticalClassification(m_WeekList),weektable,weekrankmodel);
}
 //更新月页面
void ChartMainInterface::updateMonthPage()
{
    monthmodel->setBillInfo(m_MonthList);
    monthmodel->updateLoadData();
    setRankInfo(statisticalClassification(m_MonthList),monthtable,monthrankmodel);

}
//更新年月面
void ChartMainInterface::updateYearPage()
{
    yearmodel->setBillInfo(m_YearList);
    yearmodel->updateLoadData();
    setRankInfo(statisticalClassification(m_YearList),yeartable,yearrankmodel);
}
//当选中类型转换按钮时的槽函数 //收到展开或者关闭的信号槽函数
void ChartMainInterface::on_TypeChoose_Clicked()
{
    if(isExpandOrClose){
        m_Mask = new ChartMaskSubPage(m_Type,this);
        connect(m_Mask,&ChartMaskSubPage::sendButtonType,this,&ChartMainInterface::on_ReceiveTypeSignal);
        m_Mask->hide();
        QPoint pos = ui->titlewidget->pos();
        m_Mask->move(pos.x(),pos.y()+ui->titlewidget->height());
        m_Mask->show();
    }
    else{
        m_Mask->close();
        m_Mask->deleteLater();
    }
    isExpandOrClose = !isExpandOrClose;
}
//当收到类型信号时
void ChartMainInterface::on_ReceiveTypeSignal(InOrOut type)
{
     m_Type = type;
     modifyTitleType();
     modifyRankTitleType();
     modifyLoadDataType();
     modifyLoadRankDataType();
}
//修改标题类型
void ChartMainInterface::modifyTitleType()
{
    if(m_Type == InOrOut::Expand)
        ui->pushButton_TypeChoose->setText("支出V");
    else if(m_Type == InOrOut::Income)
        ui->pushButton_TypeChoose->setText("收入V");
    isExpandOrClose = true;
}
//修改加载数据类型
void ChartMainInterface::modifyLoadDataType()
{
    weekmodel->setInOrOutType(m_Type);
    monthmodel->setInOrOutType(m_Type);
    yearmodel->setInOrOutType(m_Type);
    weekmodel->updateLoadData();
    monthmodel->updateLoadData();
    yearmodel->updateLoadData();
}
//修改排行榜类型
void ChartMainInterface::modifyRankTitleType()
{
    QString str;
    m_Type == InOrOut::Expand ? str = "支出排行榜" : str = "收入排行榜";
    weekRankTitle->setText(str);
    monthRankTitle->setText(str);
    yearRankTitle->setText(str);
}
//修改加载的排行榜数据类型
void ChartMainInterface::modifyLoadRankDataType()
{
    setRankInfo(statisticalClassification(m_WeekList),weektable,weekrankmodel);
    setRankInfo(statisticalClassification(m_MonthList),monthtable,monthrankmodel);
    setRankInfo(statisticalClassification(m_YearList),yeartable,yearrankmodel);
}
//当收到SelectScrollBar信号时
void ChartMainInterface::on_ReceiveSelectScorllBarSignal(ChartSelectType type,int id)
{
    LOG("on_ReceiveSelectScorllBarSignal");
    LOG("id:%d",id);
    switch (type){
        case ChartSelectType::week :{
           m_CurrentWeekId = id;
           weekmodel->setId(id);
           m_WeekList = getWeekBills(id);
           weekmodel->setBillInfo(m_WeekList);
           setRankInfo(statisticalClassification(m_WeekList),weektable,weekrankmodel);
           break;
        }
        case ChartSelectType::month : {
            m_CurrentMonthId = id;
            monthmodel->setId(id);
            m_MonthList = getMonthBills(id);
            monthmodel->setBillInfo(m_MonthList);
            setRankInfo(statisticalClassification(m_MonthList),monthtable,monthrankmodel);
            break;
        }
        case ChartSelectType::year : {
            m_CurrentYearId = id;
            yearmodel->setId(id);
            m_YearList = getYearBills(id);
            yearmodel->setBillInfo(m_YearList);
            setRankInfo(statisticalClassification(m_YearList),yeartable,yearrankmodel);
            break;
        }
    }
}
//获取周账单
QVector<BillTableStruct> ChartMainInterface::getWeekBills(int id)
{
    QSqlQuery query(coon);
    QVector<BillTableStruct> list;
    billQueryofWeek(query,UserInfo::UserID,getDateByWeekNum(id),list);
    return list;
}
//获取月账单
QVector<BillTableStruct> ChartMainInterface::getMonthBills(int id)
{
    QSqlQuery query(coon);
    QVector<BillTableStruct> list;
    QDate date = QDate::currentDate();
    billQueryofMonth(query,UserInfo::UserID,date,id,list);
    return list;
}
//获取年账单
QVector<BillTableStruct> ChartMainInterface::getYearBills(int id)
{
    QSqlQuery query(coon);
    QVector<BillTableStruct> list;
    QDate date;
    date.setDate(id,1,1);
    billQureyOfYear(query,UserInfo::UserID,date,list);
    return list;
}
//获取日期通过周数
QVector<QDate> ChartMainInterface::getDateByWeekNum(int weeknum)
{
    //1.先确定当前年的1月1日是周几 如果 大于 周一 则为去年的周数
    QVector<QDate> list;
    QDate date = QDate::currentDate();
    date.setDate(date.year(),1,1);
    if(date.weekNumber() != 1){
        date = date.addDays(7-date.dayOfWeek());
    }
    date = date.addDays((weeknum - 1) * 7);
    for(int i = 0 ; i < 7; i++){
        list.append(date.addDays(i+1));
    }
    return list;
}
//设置排行榜数据信息
void ChartMainInterface::setRankInfo(QVector<BillTableStruct> list,DetialTableview * table,QStandardItemModel * model)
{
    LOG("list.size:%d",list.size());
    model->clear();
    model->setColumnCount(1);
    model->setRowCount(list.size());
    table->setColumnWidth(0,getScreenSize().width());
    std::sort(list.begin(),list.end(),PublicSort::sortByMoney);
    if(list.isEmpty()){
         table->setStyleSheet("QTableView, QHeaderView, QTableView::item {background: white;} QTableView::item:selected { /*被选中的index*/color: black;background: white;} QTableView{border-image:url(:/public/image/tableview_NoData.jpg);}");
    }
    else{
        table->setStyleSheet("QTableView, QHeaderView, QTableView::item {background: white;} QTableView::item:selected { /*被选中的index*/color: black;background: white;}");
    }
   for(int i = 0; i < list.size(); i++){
       LOG("list[i].InOrOut:%d",list[i].InOrOut);
       if(list[i].InOrOut == m_Type){
           table->setRowHeight(i,getScreenSize().height()/12);
           QModelIndex index = model->index(i,0);
           QPointer<BillDataItemWithProcessBar> item = new BillDataItemWithProcessBar(list[i]);
           item->setPercent(calculatePercent(list[i].moneyAmount,list));
           item->hideDate();
           table->setIndexWidget(index,item);
       }
   }
}
//计算占比比值
double ChartMainInterface::calculatePercent(double moneynum,QVector<BillTableStruct>list)
{
    double MoneyCount = 0;
    for(int i = 0; i < list.size(); i++){
        MoneyCount += list[i].moneyAmount;
    }
    double percent = moneynum/MoneyCount;
    return percent;
}
//按消费支出/收入类别统计分类
QVector<BillTableStruct> ChartMainInterface::statisticalClassification(QVector<BillTableStruct> list)
{
    QMap<int,BillTableStruct> sortmap;
    QVector<BillTableStruct> sortlist;
    for(int i = 0; i < list.size(); i++){
        if(list[i].InOrOut == m_Type){
            sortmap[list[i].typeId].moneyAmount += list[i].moneyAmount;
            sortmap[list[i].typeId].InOrOut = list[i].InOrOut;
            sortmap[list[i].typeId].typeId = list[i].typeId;
        }
    }
    for(BillTableStruct bill : sortmap.values())
    {
        if(bill.InOrOut == InOrOut::Income){
                bill.IconPath = QString(":/BillPage/image/Bill_Income_%1.jpg").arg(bill.typeId);
                bill.PayType = GLOBALDATA::incomemenuvector[bill.typeId];
        }
        else if (bill.InOrOut == InOrOut::Expand){
                bill.IconPath = QString(":/BillPage/image/BillPage_Expand_%1.jpg").arg(bill.typeId);
                bill.PayType = GLOBALDATA::expandituremenuvector[bill.typeId-1];
        }
        sortlist.append(bill);
    }
    return sortlist;
}
//设置按钮点击后的样式
void ChartMainInterface::setButtonStyleAfterClicked(QPushButton * button)
{
    button->setStyleSheet("color:rgb(249,219,97);"
                          "background-color:rgb(35,35,35);"
                          "border:none;"
                          "font: 19pt \"微软雅黑\";");
}
//设置按钮正常时样式
void ChartMainInterface::setButtonStyleOnNormal(QPushButton * button)
{
    button->setStyleSheet("background-color: rgb(249, 219, 97);"
                          "color:rgb(0,0,0);"
                          "border:3px solid;"
                          "border-color:rgb(0,0,0);"
                          "font: 19pt \"微软雅黑\";");
}

void ChartMainInterface::initWeekPageCtrls()
{
    weekbar = new SelectScrollBar(ChartSelectType::week);
    weekmodel = new ChartModel(ChartSelectType::week);
    weekmodel->setId(22);

    QGridLayout * weeklayout = new QGridLayout;
    weeklayout->addWidget(weekbar,0,0);
    weeklayout->addWidget(weekmodel,1,0);
    weeklayout->setSpacing(0);
    weeklayout->setMargin(0);
    weeklayout->setRowStretch(0,1);
    weeklayout->setRowStretch(1,8);
    ui->weekchartwidget->setLayout(weeklayout);
    connect(weekbar,&SelectScrollBar::ItemClicked,this,&ChartMainInterface::on_ReceiveSelectScorllBarSignal);
    //-------------------------------------------------------------------------------------------------------
    QGridLayout * weektablelayout = new QGridLayout;
    weektable = new DetialTableview();
    weekrankmodel = new QStandardItemModel();
    weektable->setModel(weekrankmodel);
    weekRankTitle = new QLabel();
    QFont ft;
    ft.setPixelSize(40);
    ft.setBold(true);
    weekRankTitle->setFont(ft);
    weekRankTitle->setText("支出排行榜");
    weekRankTitle->setStyleSheet("border:none;padding-left:30px;");
    weektablelayout->addWidget(weekRankTitle,0,0);
    weektablelayout->addWidget(weektable,1,0);
    weektablelayout->setRowStretch(0,1);
    weektablelayout->setRowStretch(1,8);
    weektablelayout->setSpacing(0);
    weektablelayout->setMargin(0);
    ui->weekdatarankwidget->setLayout(weektablelayout);
}
void ChartMainInterface::initMonthPageCtrls()
{
    monthbar = new SelectScrollBar(ChartSelectType::month);
    monthmodel = new ChartModel(ChartSelectType::month);
    monthmodel->setId(4);
    QGridLayout * monthlayout = new QGridLayout;
    monthlayout->addWidget(monthbar,0,0);
    monthlayout->addWidget(monthmodel,1,0);
    monthlayout->setSpacing(0);
    monthlayout->setMargin(0);
    monthlayout->setRowStretch(0,1);
    monthlayout->setRowStretch(1,8);
    ui->monthchartwidget->setLayout(monthlayout);
    connect(monthbar,&SelectScrollBar::ItemClicked,this,&ChartMainInterface::on_ReceiveSelectScorllBarSignal);


    QGridLayout * monthtablelayout = new QGridLayout;
    monthtable = new DetialTableview();
    monthrankmodel = new QStandardItemModel();
    monthtable->setModel(monthrankmodel);
    monthRankTitle = new QLabel();
    QFont ft;
    ft.setPixelSize(40);
    ft.setBold(true);
    monthRankTitle->setFont(ft);
    monthRankTitle->setText("支出排行榜");
    monthRankTitle->setStyleSheet("border:none;padding-left:30px;");
    monthtablelayout->addWidget(monthRankTitle,0,0);
    monthtablelayout->addWidget(monthtable,1,0);
    monthtablelayout->setRowStretch(0,1);
    monthtablelayout->setRowStretch(1,8);
    monthtablelayout->setSpacing(0);
    monthtablelayout->setMargin(0);
    ui->monthdatarankwidget->setLayout(monthtablelayout);
}
void ChartMainInterface::initYearPageCtrls()
{
    yearbar = new SelectScrollBar(ChartSelectType::year);
    yearmodel = new ChartModel(ChartSelectType::year);
    QGridLayout * yearlayout = new QGridLayout;
    yearlayout->addWidget(yearbar,0,0);
    yearlayout->addWidget(yearmodel,1,0);
    yearlayout->setSpacing(0);
    yearlayout->setMargin(0);
    yearlayout->setRowStretch(0,1);
    yearlayout->setRowStretch(1,8);
    ui->yearchartwidget->setLayout(yearlayout);
    connect(yearbar,&SelectScrollBar::ItemClicked,this,&ChartMainInterface::on_ReceiveSelectScorllBarSignal);

    QGridLayout * yeartablelayout = new QGridLayout;
    yeartable = new DetialTableview();
    yearrankmodel = new QStandardItemModel();
    yeartable->setModel(yearrankmodel);
    yearRankTitle = new QLabel();
    QFont ft;
    ft.setPixelSize(40);
    ft.setBold(true);
    yearRankTitle->setFont(ft);
    yearRankTitle->setText("支出排行榜");
    yearRankTitle->setStyleSheet("border:none;padding-left:30px;");
    yeartablelayout->addWidget(yearRankTitle,0,0);
    yeartablelayout->addWidget(yeartable,1,0);
    yeartablelayout->setRowStretch(0,1);
    yeartablelayout->setRowStretch(1,8);
    yeartablelayout->setSpacing(0);
    yeartablelayout->setMargin(0);
    ui->yeardatarankwidget->setLayout(yeartablelayout);
}
void ChartMainInterface::initWeekLayout()
{
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->weekchartwidget,0,0);
    layout->addWidget(ui->weekdatarankwidget,1,0);
    layout->setRowStretch(0,5);
    layout->setRowStretch(1,8);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->weekpage->setLayout(layout);
}
void ChartMainInterface::initMonthLayout()
{
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->monthchartwidget,0,0);
    layout->addWidget(ui->monthdatarankwidget,1,0);
    layout->setRowStretch(0,5);
    layout->setRowStretch(1,8);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->monthpage->setLayout(layout);

}
void ChartMainInterface::initYearLayout()
{
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->yearchartwidget,0,0);
    layout->addWidget(ui->yeardatarankwidget,1,0);
    layout->setRowStretch(0,5);
    layout->setRowStretch(1,8);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->yearpage->setLayout(layout);
}
