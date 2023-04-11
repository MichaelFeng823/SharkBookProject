#include "discoverymaininterface.h"
#include "ui_discoverymaininterface.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include "Main/DiscoveryPage/SubPage/personinfodisplaykit.h"
#include "Controler/DataControl/databaseobj.h"
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include "Main/DiscoveryPage/SubPage/persondetailinfosubpage.h"
#include "Kit/Chinese2PinYin/chinese2pinyin.h"
#include "Kit/LogInfo/clog.h"
#include <qpointer.h>
#include <algorithm>
#include <QMessageBox>
#include <QVector>
#include <cctype>
#include <QCompleter>
using namespace ScreenFunc;
using namespace DataQuery;
using namespace DataDelete;

QString DiscoveryMainInterface::comparedChar = "";               //被比较的首字符
QString DiscoveryMainInterface::comparedNumberChar = "";         //被比较的数字字符串

DiscoveryMainInterface::DiscoveryMainInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiscoveryMainInterface)
{
    ui->setupUi(this);
    hasConnected = DataBaseObj::getConnecttion(coon);
    connect(ui->ButtonAdd,&QPushButton::clicked,this,&DiscoveryMainInterface::onAddButtonClicked);
    connect(ui->LineEdit_Serch,&QLineEdit::textEdited,this,&DiscoveryMainInterface::onLineEditInput);
    connect(ui->ButtonQuery,&QPushButton::clicked,this,&DiscoveryMainInterface::onQueryButtonClicked);
    connect(ui->MenuButtonone,&QPushButton::clicked,this,&DiscoveryMainInterface::onAboutButtonClicked);
    model =new QStandardItemModel();
    ui->tableView->setModel(model);
    getMailInfo();
    std::sort(m_maillist.begin(),m_maillist.end(),DiscoveryMainInterface::orderByDefault);
    initTableview();

//    QCompleter *completerName = new QCompleter(nameList, this);
//    //completerName->setFilterMode(Qt::MatchContains);
//    completerName->setCompletionMode(QCompleter::PopupCompletion);
//    QCompleter *completerTel = new QCompleter(telList, this);
//    //completerTel->setFilterMode(Qt::MatchContains);
//    completerTel->setCompletionMode(QCompleter::PopupCompletion);
//    ui->LineEdit_Serch->setCompleter(completerName);
//    ui->LineEdit_Serch->setCompleter(completerTel);
}

void DiscoveryMainInterface::initTableview()
{
    model->clear();
    model->setColumnCount(1);
    model->setRowCount(m_maillist.size());
    ui->tableView->setColumnWidth(0,getScreenSize().width());
    if(m_maillist.isEmpty()){
        LOG("no data");
        ui->tableView->setStyleSheet("QTableView, QHeaderView, QTableView::item {background: white;} QTableView::item:selected { /*被选中的index*/color: black;background: white;} QTableView{border-image:url(:/public/image/tableview_NoData.jpg);}");
    }
    else{
         ui->tableView->setStyleSheet("QTableView, QHeaderView, QTableView::item {background: white;} QTableView::item:selected { /*被选中的index*/color: black;background: white;}");
    }

   for(int i = 0; i < m_maillist.size(); i++){
       ui->tableView->setRowHeight(i,getScreenSize().height()/10*8/4);
       QModelIndex index = model->index(i,0);
       QPointer<PersonInfoDisplayKit> item = new PersonInfoDisplayKit;
       connect(item,&PersonInfoDisplayKit::callApplyMailDetail,this,&DiscoveryMainInterface::onReceiveApplyMailDetail);
       connect(item,&PersonInfoDisplayKit::callApplyDeleteMailInfo,this,&DiscoveryMainInterface::onReceiveApplyDeleteMailInfo);
       item->setMailInfo(m_maillist[i]);
       ui->tableView->setIndexWidget(index,item);
   }
}

//获取通讯数据信息
void DiscoveryMainInterface::getMailInfo()
{
    if(hasConnected){
        QSqlQuery query(coon);
        mailQurey(query,UserInfo::UserID,m_maillist);
    }
}
//当收到刷新通讯录信息信号槽函数
void DiscoveryMainInterface::onReceiveUpdateMailInfo(bool status)
{
    if(status){
        getMailInfo();
        std::sort(m_maillist.begin(),m_maillist.end(),DiscoveryMainInterface::orderByDefault);
        initTableview();
    }
}
//当收到查看通讯录详情
void DiscoveryMainInterface::onReceiveApplyMailDetail(MailTableStruct mailtable)
{
    LOG("onReceiveApplyMailDetail");
    QPointer<PersonDetailInfoSubPage> pointer = new PersonDetailInfoSubPage();
    connect(pointer,&PersonDetailInfoSubPage::callApplyUpdateSignal,this,&DiscoveryMainInterface::onReceiveUpdateMailInfo);
    pointer->setBaseInfo(mailtable);
    pointer->show();
}
//当收到删除通讯录数据请求信号槽函数
void DiscoveryMainInterface::onReceiveApplyDeleteMailInfo(MailTableStruct mailtable)
{
    QMessageBox messageBox(QMessageBox::NoIcon,
                               "确认删除", "删除后数据不可恢复!",
                                QMessageBox::No|QMessageBox::Yes, NULL);
    int result=messageBox.exec();
    if(result == QMessageBox::Yes){
        if(hasConnected){
            QSqlQuery query(coon);
            if(mailDelete(coon,query,mailtable) > 0){
                LOG("删除成功");
                onReceiveUpdateMailInfo(true);
            }
            else{
                 QMessageBox::question(NULL,"删除失败","账单信息删除失败!",QMessageBox::Ok);
            }
        }
        else
            LOG("数据库未连接!");
    }
    else if(result == QMessageBox::No){
        LOG("取消删除");
    }
}
//当添加按钮按下槽函数
void DiscoveryMainInterface::onAddButtonClicked()
{
    LOG("onReceiveApplyMailDetail");
    QPointer<PersonDetailInfoSubPage> pointer = new PersonDetailInfoSubPage(true);
    connect(pointer,&PersonDetailInfoSubPage::callApplyUpdateSignal,this,&DiscoveryMainInterface::onReceiveUpdateMailInfo);
    pointer->show();
}
//当查找按钮按下槽函数
void DiscoveryMainInterface::onQueryButtonClicked()
{
    QString str = ui->LineEdit_Serch->text();
    onLineEditInput(str);
}
//当搜索输入框输入时
void DiscoveryMainInterface::onLineEditInput(QString str)
{
    LOG("%s",str.toStdString().c_str());
    strType type = checkStrType(str);
    switch (type) {
    case strType::Chinese:
    {
        LOG("chinese sort");
        std::sort(m_maillist.begin(),m_maillist.end(),DiscoveryMainInterface::orderByChinese);
        initTableview();
       break;
    }
    case strType::Letter:
    {
        LOG("letter sort!");
        std::sort(m_maillist.begin(),m_maillist.end(),DiscoveryMainInterface::orderByLitter);
        initTableview();
        break;
    }
    case strType::Number:
    {
        LOG("number sort!");
        std::sort(m_maillist.begin(),m_maillist.end(),DiscoveryMainInterface::orderByNumber);
        initTableview();
        break;
    }
    default:
        LOG("default sort!");
        std::sort(m_maillist.begin(),m_maillist.end(),DiscoveryMainInterface::orderByDefault);
        initTableview();
        break;
    }
    LOG("%d",type);
}

//检测字符串内容类型
strType DiscoveryMainInterface::checkStrType(QString str)
{
    if(!str.isEmpty())
    {
        if(isalpha(str.at(0).toLatin1())){
            comparedChar = str.at(0);
            return strType::Letter;
        }
        else if(isalnum(str.at(0).toLatin1())){
             comparedNumberChar = str;
             return strType::Number;
        }else{
            comparedChar = str;
            return strType::Chinese;   
        }
    }
    else{
        comparedChar = "";
        return strType::none;
    }
}
//关于按钮按下槽函数
void DiscoveryMainInterface::onAboutButtonClicked()
{
    QString detail = QString("此页面主要功能为通讯录功能:\n"
                             "1.记录您以及朋友的基本信息\n"
                             "2.记录您以及朋友的爱好\n"
                             "3.记录您以及朋友的优/缺点\n"
                             "4.记录您以及朋友的荣誉等事件\n"
                             "5.搜索框支持人名/QQ/电话的模糊查询\n\n"
                             "copyright@MichaelFeng");
     QMessageBox::information(0,"about",detail);
}
//通过汉字名称首字符排序
bool DiscoveryMainInterface::orderByChinese(MailTableStruct mailFirst,MailTableStruct mailSecond)
{
    int indexFirst = mailFirst.personName.contains(comparedChar);
    int indexSecond = mailSecond.personName.contains(comparedChar);

    return indexFirst > indexSecond;
}
//通过首字母字符排序
bool DiscoveryMainInterface::orderByLitter(MailTableStruct mailFirst,MailTableStruct mailSecond)
{
    int indexFirst = -1;
    int indexSecond = -1;
    QString strFirst;
    QString strSecond;
    if(!mailFirst.personName.isEmpty() && !mailSecond.personName.isEmpty()){
        strFirst = Chinese2Pinyin::getCamelChars(mailFirst.personName.at(0)).at(0);
        strSecond = Chinese2Pinyin::getCamelChars(mailSecond.personName.at(0)).at(0);
        LOG("strFirst:%s",strFirst.toStdString().c_str());
        LOG("strSecond:%s",strSecond.toStdString().c_str());
        indexFirst = strFirst.contains(comparedChar,Qt::CaseInsensitive);
        indexSecond = strSecond.contains(comparedChar,Qt::CaseInsensitive);
    }
    return indexFirst > indexSecond;
}
//通过数字字符排序
bool DiscoveryMainInterface::orderByNumber(MailTableStruct mailFirst,MailTableStruct mailSecond)
{
    int indexFirst = mailFirst.tel.contains(comparedNumberChar);
    int indexSecond = mailSecond.tel.contains(comparedNumberChar);

    return indexFirst > indexSecond;
}
//默认排序
bool DiscoveryMainInterface::orderByDefault(MailTableStruct mailFirst,MailTableStruct mailSecond)
{
    return mailFirst.mailId < mailSecond.mailId;
}

//通过准确名字来排序
bool DiscoveryMainInterface::orderByAccurate(MailTableStruct mailFirst,MailTableStruct mailSecond)
{

}
DiscoveryMainInterface::~DiscoveryMainInterface()
{
    delete ui;
}
