#include "billdetailitemeditsubpage.h"
#include "ui_billdetailitemeditsubpage.h"
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include "Kit/LogInfo/clog.h"
#include <QMessageBox>
#include "Controler/PublicApi/PublicDbFunc.h"
#include "Controler/DataControl/databaseobj.h"
using namespace DataDelete;

BillDetailItemEditSubPage::BillDetailItemEditSubPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BillDetailItemEditSubPage)
{
    ui->setupUi(this);
    connect(ui->returnButton,&QPushButton::clicked,this,&BillDetailItemEditSubPage::onReturnButtonClicked);

    this->raise();
    this->show();
}

BillDetailItemEditSubPage::BillDetailItemEditSubPage(BillTableStruct billinfo,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BillDetailItemEditSubPage)
{
    ui->setupUi(this);
    connect(ui->returnButton,&QPushButton::clicked,this,&BillDetailItemEditSubPage::onReturnButtonClicked);
    connect(ui->editButton,&QPushButton::clicked,this,&BillDetailItemEditSubPage::onEditButtonClicked);
    connect(ui->deleteButton,&QPushButton::clicked,this,&BillDetailItemEditSubPage::onDeleteButtonClicked);
    m_billiteminfo = billinfo;
    setType();
    setTypeIcon(getIconPath());
    setMoneyText(QString::number(m_billiteminfo.moneyAmount,'f',2));
    setDateText(m_billiteminfo.date);
    setRemarks(m_billiteminfo.remarks);
    setTypetext(m_billiteminfo.PayType);
    this->raise();
    this->show();
}
//获取IconPath
QString BillDetailItemEditSubPage::getIconPath()
{
    QString iconpath;
    if(m_billiteminfo.InOrOut == InAndOutType::OutType)
        iconpath = QString(":/BillPage/image/Expand_%1_On_SubPage.jpg").arg(m_billiteminfo.typeId);
    else if(m_billiteminfo.InOrOut == InAndOutType::InType)
        iconpath = QString(":/BillPage/image/Income_%1_On_Subpage.jpg").arg(m_billiteminfo.typeId);
    return iconpath;
}
//获取类型
void BillDetailItemEditSubPage::setType()
{
    if(m_billiteminfo.InOrOut == InAndOutType::InType)
        setInAndOutTypeText("收入");
    else if(m_billiteminfo.InOrOut == InAndOutType::OutType)
        setInAndOutTypeText("支出");
}
//设置支出和收入类型
void BillDetailItemEditSubPage::setInAndOutTypeText(QString str)
{
    ui->label_InAndOut_Type->setText(str);
}
//设置金额
void BillDetailItemEditSubPage::setMoneyText(QString str)
{
    ui->label_Money->setText(str);
}
//设置日期
void BillDetailItemEditSubPage::setDateText(QDate date)
{
    int dayindex = date.dayOfWeek();
    dayindex == 7 ? dayindex = 0:dayindex;
    ui->label_Date->setText(QString("%1年%2月%3日 星期%4").arg(date.year()).arg(date.month()).arg(date.day()).arg(dayOfWeak[dayindex]));
}
 //设置备注
void BillDetailItemEditSubPage::setRemarks(QString str)
{
    ui->label_Remark->setText(str);
}
//设置类型内容
void BillDetailItemEditSubPage::setTypetext(QString typetext)
{
    ui->label_Type->setText(typetext);
}
//设置类型图标
void BillDetailItemEditSubPage::setTypeIcon(QString iconpath)
{
    LOG("iconpath:%s",iconpath.toStdString().c_str());
    ui->label_TypeIcon->setStyleSheet(QString("border-image: url(%1);").arg(iconpath));
}
//返回按钮槽函数
void BillDetailItemEditSubPage::onReturnButtonClicked()
{
    this->close();
}
//编辑按钮槽函数
void BillDetailItemEditSubPage::onEditButtonClicked()
{
    callApplyModify(m_billiteminfo);
    this->close();
}
//删除按钮槽函数
void BillDetailItemEditSubPage::onDeleteButtonClicked()
{
    QMessageBox messageBox(QMessageBox::NoIcon,
                               "确认删除", "删除后数据不可恢复!",
                                QMessageBox::No|QMessageBox::Yes, NULL);
    int result=messageBox.exec();
    if(result == QMessageBox::Yes){
        if(deleteBillInfo(m_billiteminfo)){
            LOG("删除成功");
            this->close();
            callApplyDeleteSuccessUpdateParent(true);
        }
        else
            QMessageBox::question(NULL,"删除失败","账单信息删除失败!",QMessageBox::Ok);
    }
    else if(result == QMessageBox::No){
        LOG("取消删除");
    }
}

//删除账单信息
bool BillDetailItemEditSubPage::deleteBillInfo(BillTableStruct billinfo)
{
    bool result = false;
    QSqlDatabase coon;
    bool hasConnected = DataBaseObj::getConnecttion(coon);
    if(hasConnected){
        QSqlQuery query(coon);
       if(billDelete(coon,query,billinfo) > 0){
            LOG("billDelete Success!");
            result = true;
       }
       else{
            LOG("billDelete failure!");
       }
    }
    else{
        LOG("数据库连接失败!");
    }
    return result;
}
BillDetailItemEditSubPage::~BillDetailItemEditSubPage()
{
    delete ui;
}
