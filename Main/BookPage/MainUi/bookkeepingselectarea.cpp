#include "bookkeepingselectarea.h"
#include "ui_bookkeepingselectarea.h"
#include <QSettings>
#include <QFile>
#include <QStandardItemModel>
#include <QMessageBox>
#include "Controler/GlobalInfo/GlobalDocumentPath.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include "Kit/LogInfo/clog.h"
#include "Main/BookPage/SubCtrls/bookkeepinginputkit.h"
#include "Controler/DataControl/databaseobj.h"
#include "Controler/DataStruct/PublicDataStruct.h"
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include "Controler/PublicApi/PublicDbFunc.h"

using namespace DocPath;
using namespace ScreenFunc;
using namespace DataInsert;
using namespace DataUpdate;
BookkeepingSelectArea::BookkeepingSelectArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookkeepingSelectArea)
{
    ui->setupUi(this);
    //this->setAttribute(Qt::WA_DeleteOnClose,true);
    initMenu();
    buildLayout();
    initTablecontent();
    onExpanditurePageClicked();
    this->raise();
    this->show();
}

BookkeepingSelectArea::BookkeepingSelectArea(BillTableStruct billinfo,bool isModify,QWidget *parent):
    QWidget(parent),
    ui(new Ui::BookkeepingSelectArea),m_Billinfo(billinfo)
{
    ui->setupUi(this);
     m_IsModify = isModify;
    //this->setAttribute(Qt::WA_DeleteOnClose,true);
    initMenu();
    buildLayout();
    initTablecontent();
    this->raise();
    this->show();
    if(m_IsModify)
    {
        if(m_Billinfo.InOrOut == InAndOutType::OutType){
            onExpanditurePageClicked();
        }
        else if(m_Billinfo.InOrOut == InAndOutType::InType){
            onIncomePageClicked();
        }
        if(bookkeepinginputkit != nullptr)
            delete bookkeepinginputkit;
        bookkeepinginputkit = new BookkeepingInputKit(billinfo,true,this);
        connect(bookkeepinginputkit,&BookkeepingInputKit::finishBookkeepingInputAndModify,this,&BookkeepingSelectArea::onReceiveBillInfoModify);
    }
    resetMenuButtonstatus();

}
//重置/恢复按钮的状态---用于修改时先预选中对应的菜单按钮
void BookkeepingSelectArea::resetMenuButtonstatus()
{
    if(m_Billinfo.InOrOut == InAndOutType::OutType){
        for(QPushButton * button : m_ExpandButtonList){
            if(button->property("Id").toInt() == m_Billinfo.typeId)
                button->click();
        }
    }
    else if(m_Billinfo.InOrOut == InAndOutType::InType){
        for(QPushButton * button : m_IncomeButtonList){
            if(button->property("Id").toInt() == m_Billinfo.typeId)
                button->click();
        }
    }
}

//构建布局
void BookkeepingSelectArea::buildLayout()
{
    //构建支出表格界面布局
    expanditureview = new DetialTableview();

    expanditureviewlayout.setSpacing(0);
    expanditureviewlayout.setMargin(0);
    expanditureview->setLayout(&expanditureviewlayout);

    incomeview = new DetialTableview();
    incomeviewlayout.setSpacing(0);
    incomeviewlayout.setMargin(0);
    incomeview->setLayout(&incomeviewlayout);

    expanditurewidgetlayout.setSpacing(0);
    expanditurewidgetlayout.setMargin(0);
    expanditurewidgetlayout.addWidget(expanditureview);
    ui->pageExpanditure->setLayout(&expanditurewidgetlayout);

    incomewidgetlayout.setSpacing(0);
    incomewidgetlayout.setMargin(0);
    incomewidgetlayout.addWidget(incomeview);
    ui->pageInCome->setLayout(&incomewidgetlayout);

    connect(ui->expenditurebutton,&QPushButton::clicked,this,&BookkeepingSelectArea::onExpanditurePageClicked);
    connect(ui->incomebutton,&QPushButton::clicked,this,&BookkeepingSelectArea::onIncomePageClicked);
    connect(ui->cancelbutton,&QPushButton::clicked,this,&BookkeepingSelectArea::onCancelButtonClicked);

}
//初始化记账菜单
void BookkeepingSelectArea::initMenu()
{
    QFile file(android_local_menuproperty_dir);
    if(!file.exists()||file.size() == 0){
        QString infostr = QString::fromLocal8Bit("File is not exsit,bulit it! and path:%1").arg(android_local_menuproperty_dir);
        LOG("%s",infostr.toStdString().c_str());
        file.copy("assets:/MenuSetting.ini",android_local_menuproperty_dir);      //拷到安卓本地目录
        file.setPermissions(QFile::WriteOwner | QFile::ReadOwner); //以读写的方式进行拷贝
        file.close();
    }
    //创建操作对象
    QSettings config(android_local_menuproperty_dir,QSettings::IniFormat);
    config.setIniCodec("utf8");
    config.beginGroup("keycounts");
    int expanditurecounts = config.value("expanditurecounts").toInt();
    int incomecounts = config.value("incomecounts").toInt();
    LOG("expanditurecounts = %d,incomecounts = %d",expanditurecounts,incomecounts);
    config.endGroup();
    config.beginGroup("expanditure");
    for(int i = 1; i <= expanditurecounts;i++){
         QString value = config.value(QString("menuname_%1").arg(QString::number(i))).toString();
         expandituremenuvector.append(value);
    }
    QStringList menulist = config.childKeys();
    config.endGroup();
    LOG("menulist size is :%d",menulist.size());
    config.beginGroup("income");
    for(int i = expanditurecounts+1; i <= expanditurecounts+incomecounts;i++){
         QString value = config.value(QString("menuname_%1").arg(QString::number(i))).toString();
         incomemenuvector.append(value);
    }
    menulist = config.childKeys();
    config.endGroup();
    LOG("menulist size is :%d",menulist.size());

}
 //初始化表格内容
void BookkeepingSelectArea::initTablecontent()
{
    initExpandMenuButton();
    initIncomeMenuButton();
}
 //初始化支出菜单按钮
void BookkeepingSelectArea::initExpandMenuButton()
{
    m_ExpandButtonList.clear();
    QStandardItemModel * expandituremodel = new QStandardItemModel();
    expandituremodel->setColumnCount(4);
    int rowcounts = expandituremenuvector.size()/4;
    if(expandituremenuvector.size()%4 != 0)
        rowcounts+=1;
    expandituremodel->setRowCount(rowcounts);
    expanditureview->setModel(expandituremodel);

    expanditureview->setColumnWidth(0,getScreenSize().width()/4);
    expanditureview->setColumnWidth(1,getScreenSize().width()/4);
    expanditureview->setColumnWidth(2,getScreenSize().width()/4);
    expanditureview->setColumnWidth(3,getScreenSize().width()/4);

    LOG("expandituremenuvector size is:%d",expandituremenuvector.size());
    int curindex = 0;
    for(int i = 0; i < rowcounts; i++){
        expanditureview->setRowHeight(i,getScreenSize().height()/35*33/7.5+25);
        for(int j = 0; j < 4; j++){
            if(curindex == expandituremenuvector.size())
                break;
            QModelIndex index = expandituremodel->index(i,j);
            QString str = QString("QPushButton{border-image: url(:/MenuIcon/image/expand_%1.jpg);} QPushButton:disabled{border-image: url(:/MenuIcon/image/expand_%1_clicked.jpg);}").arg(QString::number(curindex+1)).arg(QString::number(curindex+1));
            QPushButton * button = new QPushButton;
            button->setProperty("Id",curindex+1);
            button->setProperty("Name",expandituremenuvector[curindex]);
            button->setProperty("Type","Expand");
            connect(button,&QPushButton::clicked,this,&BookkeepingSelectArea::onMenuIconButtonClicked);
            button->setStyleSheet(str);
            m_ExpandButtonList.append(button);
            LOG("str is :%s",str.toStdString().c_str());
            expanditureview->setIndexWidget(index,button);
            LOG("build the %d button",curindex);
            curindex++;
        }
    }
}
 //初始化收入菜单按钮
void BookkeepingSelectArea::initIncomeMenuButton()
{
    m_IncomeButtonList.clear();
    QStandardItemModel * incomemodel = new QStandardItemModel();
    incomemodel->setColumnCount(4);
    int rowcounts = incomemenuvector.size()/4;
    if(incomemenuvector.size()%4 != 0)
        rowcounts+=1;
    incomemodel->setRowCount(rowcounts);
    incomeview->setModel(incomemodel);

    incomeview->setColumnWidth(0,getScreenSize().width()/4);
    incomeview->setColumnWidth(1,getScreenSize().width()/4);
    incomeview->setColumnWidth(2,getScreenSize().width()/4);
    incomeview->setColumnWidth(3,getScreenSize().width()/4);

    LOG("incomemenuvector size is:%d",incomemenuvector.size());
    int curindex = 0;
    for(int i = 0; i < rowcounts; i++){
        incomeview->setRowHeight(i,getScreenSize().height()/35*33/7.5+25);
        for(int j = 0; j < 4; j++){
            if(curindex == incomemenuvector.size())
                break;
            QModelIndex index = incomemodel->index(i,j);
            QString str = QString("QPushButton{border-image: url(:/MenuIcon/image/income_%1.jpg);}QPushButton:disabled{border-image: url(:/MenuIcon/image/income_%2_clicked.jpg);}").arg(QString::number(curindex+1)).arg(QString::number(curindex+1));
            QPushButton * button = new QPushButton;
            button->setProperty("Id",curindex+1);
            button->setProperty("Name",incomemenuvector[curindex]);
            button->setProperty("Type","InCome");
            connect(button,&QPushButton::clicked,this,&BookkeepingSelectArea::onMenuIconButtonClicked);
            button->setStyleSheet(str);
            m_IncomeButtonList.append(button);
            LOG("str is :%s",str.toStdString().c_str());
            //button->setStyleSheet(str);
            incomeview->setIndexWidget(index,button);
            LOG("build the %d button",curindex);
            curindex++;
        }
    }
}

void BookkeepingSelectArea::onExpanditurePageClicked()
{
    initExpandMenuButtonStyle();
    m_InOrOut = InAndOutType::OutType;
    ui->expenditurebutton->setStyleSheet("border:none;border-top:1px;border-color:rgb(0,0,0);color:rgb(0,0,0);font-family:'Microsoft YaHei';font-size:22pt;");
    ui->incomebutton->setStyleSheet("border:none;border-top:1px;border-color:rgb(0,0,0);color:rgb(0,0,0);font-family:'Microsoft YaHei';font-size:20pt;");
    ui->stackedWidget->setCurrentIndex(0);
    if(bookkeepinginputkit != nullptr)
    {
        if(bookkeepinginputkit->isVisible()){
            delete bookkeepinginputkit;
            bookkeepinginputkit = nullptr;
        }

    }

}
void BookkeepingSelectArea::onIncomePageClicked()
{
    initIncomeMenuButtonStyle();
    m_InOrOut = InAndOutType::InType;
    ui->expenditurebutton->setStyleSheet("border:none;border-top:1px;border-color:rgb(0,0,0);color:rgb(0,0,0);font-family:'Microsoft YaHei';font-size:20pt;");
    ui->incomebutton->setStyleSheet("border:none;border-top:1px;border-color:rgb(0,0,0);color:rgb(0,0,0);font-family:'Microsoft YaHei';font-size:22pt;");
    ui->stackedWidget->setCurrentIndex(1);
    if(bookkeepinginputkit != nullptr)
    {
        if(bookkeepinginputkit->isVisible()){
             delete bookkeepinginputkit;
             bookkeepinginputkit = nullptr;
        }
    }
}
void BookkeepingSelectArea::onCancelButtonClicked()
{
    if(bookkeepinginputkit != nullptr)
    {
        if(bookkeepinginputkit->isVisible()){
            delete bookkeepinginputkit;
            bookkeepinginputkit = nullptr;
        }

    }
    this->close();
}

void BookkeepingSelectArea::onMenuIconButtonClicked()
{
    QPushButton * btn = static_cast<QPushButton*>(sender());

    int Id = btn->property("Id").toInt();
    QString Name = btn->property("Name").toString();
    QString Type = btn->property("Type").toString();


    if(Type == "Expand")
    {
        if(Id == 34){
            QMessageBox::warning(0,"warning","该设置暂时无用,后续待扩展");
            return;
        }
        for(int i = 0; i < m_ExpandButtonList.size(); i++){
            if(m_ExpandButtonList.at(i)->property("Id").toInt() != Id){
                m_ExpandButtonList.at(i)->setEnabled(true);
            }
            else{
                m_ExpandButtonList.at(i)->setDisabled(true);
            }
        }
    }
    else if(Type == "InCome"){
        if(Id == 6){
            QMessageBox::warning(0,"warning","该设置暂时无用,后续待扩展");
            return;
        }
        for(int i = 0; i < m_IncomeButtonList.size(); i++){
            if(m_IncomeButtonList.at(i)->property("Id").toInt() != Id){
                m_IncomeButtonList.at(i)->setEnabled(true);
            }
            else{
                m_IncomeButtonList.at(i)->setDisabled(true);
            }
        }
    }
    m_CurrentTypeId = Id;
    LOG("the Clicked Button Id is %d and Name is %s",Id,Name.toStdString().c_str());
    if(!m_IsModify){
        if(bookkeepinginputkit != nullptr)
            delete bookkeepinginputkit;
        bookkeepinginputkit = new BookkeepingInputKit(this);
        connect(bookkeepinginputkit,&BookkeepingInputKit::finishBookkeepingInput,this,&BookkeepingSelectArea::onReceiveBillInfo);
    }
}
 //初始化支出菜单按钮样式
void BookkeepingSelectArea::initExpandMenuButtonStyle()
{
    for(int i = 0; i < m_ExpandButtonList.size(); i++){
        m_ExpandButtonList.at(i)->setEnabled(true);
    }
}
//初始化收入菜单按钮样式
void BookkeepingSelectArea::initIncomeMenuButtonStyle()
{
    for(int i = 0; i < m_IncomeButtonList.size(); i++){
        m_IncomeButtonList.at(i)->setEnabled(true);
    }
}
//收到账单信息槽函数
void BookkeepingSelectArea::onReceiveBillInfo(QDate date,double num,QString Remarkstr)
{
     QSqlDatabase coon;
     bool hasConnected = DataBaseObj::getConnecttion(coon);
     if(hasConnected){
         QSqlQuery query(coon);
         BillTableStruct billinfo;
         billinfo.date = date;
         billinfo.userId = UserInfo::UserID;
         billinfo.remarks = Remarkstr;
         billinfo.moneyAmount = num;
         billinfo.typeId = m_CurrentTypeId;
         billinfo.InOrOut = m_InOrOut;

        if(billInsert(coon,query,billinfo) > 0){
             LOG("BillInsert Success!");
             emit sendSignalToUpdateBillArea(true);
        }
        else{
             LOG("BillInsert failure!");
        }
     }
     else{
         LOG("数据库连接失败!");
     }
     if(bookkeepinginputkit != nullptr){
         bookkeepinginputkit->close();
         bookkeepinginputkit = nullptr;
     }
}

//收到账单信息修改槽函数
void BookkeepingSelectArea::onReceiveBillInfoModify(int billno,QDate date,double num,QString Remarkstr)
{
    QSqlDatabase coon;
    bool hasConnected = DataBaseObj::getConnecttion(coon);
    if(hasConnected){
        QSqlQuery query(coon);
        BillTableStruct billinfo;
        billinfo.date = date;
        billinfo.userId = UserInfo::UserID;
        billinfo.remarks = Remarkstr;
        billinfo.moneyAmount = num;
        billinfo.billNo = billno;
        billinfo.typeId = m_CurrentTypeId;
        billinfo.InOrOut = m_InOrOut;

       if(billUpdate(coon,query,billinfo) > 0){
            LOG("billUpdate Success!");
            emit sendSignalToUpdateDetailBillPage(billinfo);
       }
       else{
            LOG("billUpdate failure!");
       }
    }
    else{
        LOG("数据库连接失败!");
    }
    if(bookkeepinginputkit != nullptr){
        bookkeepinginputkit->close();
        bookkeepinginputkit = nullptr;
    }
}
BookkeepingSelectArea::~BookkeepingSelectArea()
{
    if(bookkeepinginputkit != nullptr){
        bookkeepinginputkit->close();
        bookkeepinginputkit = nullptr;
    }
    delete ui;
}
