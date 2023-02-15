#include "persondetailinfosubpage.h"
#include "ui_persondetailinfosubpage.h"
#include "Controler/databaseobj.h"
#include "Util/clog.h"
#include "Controler/PublicDbFunc.h"
#include "Controler/PublicUerInfo.h"
#include <QMessageBox>

using namespace DataUpdate;
using namespace DataInsert;

PersonDetailInfoSubPage::PersonDetailInfoSubPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonDetailInfoSubPage)
{
    ui->setupUi(this);
    this->raise();
    ui->lineEdit_Birthday->installEventFilter(this);
    ui->comboBox_Sex->addItem("男");
    ui->comboBox_Sex->addItem("女");
    connect(ui->ButtonReturn,&QPushButton::clicked,this,&PersonDetailInfoSubPage::onReturnButtonClicked);
    connect(ui->Button_Edit,&QPushButton::clicked,this,&PersonDetailInfoSubPage::onEditButtonClicked);
    connect(ui->Button_Save,&QPushButton::clicked,this,&PersonDetailInfoSubPage::onSaveButtonClicked);
    connect(ui->lineEdit_Birthday,&QLineEdit::textEdited,this,&PersonDetailInfoSubPage::onBirthdayLineEditClicked);
    setInputStatus(false);
}

PersonDetailInfoSubPage::PersonDetailInfoSubPage(bool is_add,QWidget *parent):
    QWidget(parent),
    ui(new Ui::PersonDetailInfoSubPage)
{
    m_IsAdd = is_add;
    if(is_add){
        ui->setupUi(this);
        this->raise();
        ui->lineEdit_Birthday->installEventFilter(this);
        ui->comboBox_Sex->addItem("男");
        ui->comboBox_Sex->addItem("女");
        connect(ui->ButtonReturn,&QPushButton::clicked,this,&PersonDetailInfoSubPage::onReturnButtonClicked);
        connect(ui->Button_Edit,&QPushButton::clicked,this,&PersonDetailInfoSubPage::onEditButtonClicked);
        connect(ui->Button_Save,&QPushButton::clicked,this,&PersonDetailInfoSubPage::onSaveButtonClicked);
        connect(ui->lineEdit_Birthday,&QLineEdit::textEdited,this,&PersonDetailInfoSubPage::onBirthdayLineEditClicked);
        setInputStatus(true);
        ui->Button_Edit->setVisible(false);
        ui->Button_Save->setText("添加");
    }
}
//设置基本信息
void PersonDetailInfoSubPage::setBaseInfo(MailTableStruct mailtable)
{
    m_mailInfo = mailtable;
    this->setPersonName(m_mailInfo.personName);
    this->setPersonSex(m_mailInfo.sex);
    this->setPersonNickName(m_mailInfo.maildetailinfo.nickName);
    this->setPersonAge(m_mailInfo.age);
    this->setPersonBirthday(m_mailInfo.birthdate);
    this->setPersonIndustry(m_mailInfo.industry);
    this->setPersonOccupation(m_mailInfo.occupation);
    this->setPersonTel(m_mailInfo.tel);
    this->setPersonQQ(m_mailInfo.qq);
    this->setPersonEmail(m_mailInfo.email);
    this->setPersonAddress(m_mailInfo.address);
    this->setPersonHobby(m_mailInfo.maildetailinfo.hobby);
    this->setPersonAdvantage(m_mailInfo.maildetailinfo.advantage);
    this->setPersonShortComming(m_mailInfo.maildetailinfo.shortcomming);
    this->setPersonMajorEvent(m_mailInfo.maildetailinfo.majorEvent);
}
//设置输入状态
void PersonDetailInfoSubPage::setInputStatus(bool status)
{
    QList < QLineEdit * > list_lineEdit = this->findChildren < QLineEdit * > ();

    for (QLineEdit * lineEdit : list_lineEdit) {
        lineEdit->setDisabled(!status);
    }
    ui->comboBox_Sex->setDisabled(!status);
    ui->textEdit_MajorEvent->setDisabled(!status);
}
//设置姓名
void PersonDetailInfoSubPage::setPersonName(QString name)
{
    m_mailInfo.personName = name;
    ui->lineEdit_PersonName->setText(name);
}
//设置性别
void PersonDetailInfoSubPage::setPersonSex(int sex)
{
    int index = -1;
    m_mailInfo.sex = sex;
    sex == 1 ? index = 0:index = 1;
    ui->comboBox_Sex->setCurrentIndex(index);
}
//设置昵称
void PersonDetailInfoSubPage::setPersonNickName(QString nickname)
{
    m_mailInfo.maildetailinfo.nickName = nickname;
    ui->lineEdit_NickName->setText(nickname);
}
//设置年龄
void PersonDetailInfoSubPage::setPersonAge(int age)
{
    m_mailInfo.age = age;
    ui->lineEdit_Age->setText(QString::number(age));
}
//设置生日
void PersonDetailInfoSubPage::setPersonBirthday(QDate date)
{
    m_mailInfo.birthdate = date;
    ui->lineEdit_Birthday->setText(QString("%1年%2月%3日").arg(date.year()).arg(date.month()).arg(date.day()));
}
//设置行业
void PersonDetailInfoSubPage::setPersonIndustry(QString industry)
{
    m_mailInfo.industry = industry;
    ui->lineEdit_Industry->setText(industry);
}
//设置职业
void PersonDetailInfoSubPage::setPersonOccupation(QString occupation)
{
    m_mailInfo.occupation = occupation;
    ui->lineEdit_Occupation->setText(occupation);
}
//设置电话
void PersonDetailInfoSubPage::setPersonTel(QString tel)
{
    m_mailInfo.tel = tel;
    ui->lineEdit_Tel->setText(tel);
}
//设置QQ
void PersonDetailInfoSubPage::setPersonQQ(QString qq)
{
    m_mailInfo.qq = qq;
    ui->lineEdit_QQ->setText(qq);
}
//设置邮箱
void PersonDetailInfoSubPage::setPersonEmail(QString email)
{
    m_mailInfo.email = email;
    ui->lineEdit_Email->setText(email);
}
//设置住址
void PersonDetailInfoSubPage::setPersonAddress(QString address)
{
    m_mailInfo.address = address;
    ui->lineEdit_Address->setText(address);
}
//设置爱好
void PersonDetailInfoSubPage::setPersonHobby(QString hobby)
{
    m_mailInfo.maildetailinfo.hobby = hobby;
    ui->lineEdit_Hobby->setText(hobby);
}
//设置优点
void PersonDetailInfoSubPage::setPersonAdvantage(QString advantage)
{
    m_mailInfo.maildetailinfo.advantage = advantage;
    ui->lineEdit_Advantage->setText(advantage);
}
//设置缺点
void PersonDetailInfoSubPage::setPersonShortComming(QString shortcomming)
{
    m_mailInfo.maildetailinfo.shortcomming = shortcomming;
    ui->lineEdit_ShortComming->setText(shortcomming);
}
//设置主要事件
void PersonDetailInfoSubPage::setPersonMajorEvent(QString majorevent)
{
    m_mailInfo.maildetailinfo.majorEvent = majorevent;
    ui->textEdit_MajorEvent->clear();
    ui->textEdit_MajorEvent->setText(majorevent);
}
//当编辑按钮点击槽函数
void PersonDetailInfoSubPage::onEditButtonClicked()
{
    LOG("编辑");
    setInputStatus(true);


}
//当保存按钮点击槽函数
void PersonDetailInfoSubPage::onSaveButtonClicked()
{
    LOG("保存");
    hasConnected = DataBaseObj::getConnecttion(coon);
    QSqlQuery query(coon);
    QDate date = QDate::currentDate();
    m_mailInfo.recentUpdateDate = date;
    if(m_IsAdd == false)
    {
        if(InputIsEmptyCheck()){
            if(mailUpdate(coon,query,m_mailInfo)){
                callApplyUpdateSignal(true);
                LOG("保存成功");
            QMessageBox::information(0,"Tips","保存成功");
            }
            else
                QMessageBox::warning(0,"Tips","保存失败");
        }
    }
    else
    {
        if(InputIsEmptyCheck()){
            getInputData();
            m_mailInfo.userId = UserInfo::UserID;
            if(mailInsert(coon,query,m_mailInfo)){
                 callApplyUpdateSignal(true);
                 QMessageBox::information(0,"Tips","添加成功");
                 this->close();
            }
            else
                QMessageBox::warning(0,"Tips","添加失败");
        }
    }
}
//当返回按钮点击槽函数
void PersonDetailInfoSubPage::onReturnButtonClicked()
{
    LOG("返回");
    this->close();
}
//当生日编辑框点击时槽函数
void PersonDetailInfoSubPage::onBirthdayLineEditClicked()
{
    LOG("触发日历按钮槽函数");
    if(m_IsAdd)
        m_mailInfo.birthdate = QDate::currentDate();
    m_Calendar = new Calendar(CalendarType::YearMonthDay,m_mailInfo.birthdate.year(),m_mailInfo.birthdate.month(),m_mailInfo.birthdate.day());
    connect(m_Calendar,&Calendar::returnTimeInfoToYearMonthDay,this,&PersonDetailInfoSubPage::receiveTimeInfo);
}

//收到时间信息槽函数
void PersonDetailInfoSubPage::receiveTimeInfo(int year,int month,int day)
{
    QDate date;
    date.setDate(year,month,day);
    this->setPersonBirthday(date);
}

//事件过滤器
bool PersonDetailInfoSubPage::eventFilter(QObject *obj, QEvent *event)
{
    if(ui->lineEdit_Birthday == obj)
    {
        if(event->type() == QEvent::MouseButtonPress){
            if(ui->lineEdit_Birthday->isEnabled()){
                onBirthdayLineEditClicked();
            }
        }

    }
    return QWidget::eventFilter(obj, event);
}
//获取修改后输入数据
void PersonDetailInfoSubPage::getInputData()
{
    int sex = -1;
    m_mailInfo.personName = ui->lineEdit_PersonName->text();
    ui->comboBox_Sex->currentText() == "男" ? sex = 1:sex = 2;
    m_mailInfo.sex = sex;
    m_mailInfo.age = ui->lineEdit_Age->text().toInt();
    m_mailInfo.industry = ui->lineEdit_Industry->text();
    m_mailInfo.occupation = ui->lineEdit_Occupation->text();
    m_mailInfo.tel = ui->lineEdit_Tel->text();
    m_mailInfo.qq = ui->lineEdit_QQ->text();
    m_mailInfo.email = ui->lineEdit_Email->text();
    m_mailInfo.address = ui->lineEdit_Address->text();
    //详情
    m_mailInfo.maildetailinfo.nickName = ui->lineEdit_NickName->text();
    m_mailInfo.maildetailinfo.hobby = ui->lineEdit_Hobby->text();
    m_mailInfo.maildetailinfo.advantage = ui->lineEdit_Advantage->text();
    m_mailInfo.maildetailinfo.shortcomming = ui->lineEdit_ShortComming->text();
    m_mailInfo.maildetailinfo.majorEvent = ui->textEdit_MajorEvent->toPlainText();
}

//插入数据时非空检测
bool PersonDetailInfoSubPage::InputIsEmptyCheck()
{
    bool ret = true;
    if(ui->lineEdit_PersonName->text().isEmpty()){
        QMessageBox::warning(0,"Tip","姓名为必填项,想要添加请输入!");
        ret = false;
    }
    else if(ui->lineEdit_Age->text().isEmpty()){
        QMessageBox::warning(0,"Tip","年龄为必填项,想要添加请输入!");
        ret = false;
    }
    else if(ui->lineEdit_Industry->text().isEmpty()){
        QMessageBox::warning(0,"Tip","行业为必填项,想要添加请输入!");
        ret = false;
    }
    else if(ui->lineEdit_Occupation->text().isEmpty()){
        QMessageBox::warning(0,"Tip","职业为必填项,想要添加请输入!");
        ret = false;
    }
    return ret;
}
//析构函数
PersonDetailInfoSubPage::~PersonDetailInfoSubPage()
{
    delete ui;
}
