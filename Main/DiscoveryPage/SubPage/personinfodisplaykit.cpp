#include "personinfodisplaykit.h"
#include "ui_personinfodisplaykit.h"
#include <QDate>
#include "Kit/LogInfo/clog.h"

PersonInfoDisplayKit::PersonInfoDisplayKit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonInfoDisplayKit)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    ui->buttonDelete->setVisible(false);
    connect(ui->buttondetail,&QPushButton::clicked,this,&PersonInfoDisplayKit::onDetailButtonClicked);
    connect(ui->buttonDelete,&QPushButton::clicked,this,&PersonInfoDisplayKit::onDeleteButtonClicked);
    connect(&timer,&QTimer::timeout,[=]()
    {
        onPressedTimerFunc();
    });
}
//设置人名
void PersonInfoDisplayKit::setPersonName(QString name)
{
    m_mailtablestruct.personName = name;
    ui->label_personname->setText(name);
}
//设置性别
void PersonInfoDisplayKit::setSex(int sex)
{
    QString sexstr;
    m_mailtablestruct.sex = sex;
    sex == 1 ? sexstr = "男":sexstr = "女";
    ui->label_personsex->setText(sexstr);
    LOG("sex:%d",sex);
}
//设置年龄
void PersonInfoDisplayKit::setAge(int age)
{
    m_mailtablestruct.age = age;
    ui->label_personage->setText(QString::number(age)+"岁");
}
//设置出生年月日
void PersonInfoDisplayKit::setBirthDate(QDate date)
{
    m_mailtablestruct.birthdate = date;
    ui->label_personbrithday->setText(QString("%1年%2月%3日").arg(date.year()).arg(date.month()).arg(date.day()));
}
//设置行业名称
void PersonInfoDisplayKit::setIndustry(QString industry)
{
    m_mailtablestruct.industry = industry;
    ui->label_Industry->setText(industry);
}
//设置岗位
void PersonInfoDisplayKit::setOccupation(QString occupation)
{
    m_mailtablestruct.occupation = occupation;
    ui->label_occupation->setText(QString(occupation));
}
//设置电话号码
void PersonInfoDisplayKit::setTel(QString tel)
{
    m_mailtablestruct.tel = tel;
    ui->label_tel->setText(tel);
}
//设置qq
void PersonInfoDisplayKit::setQQ(QString qq)
{
    m_mailtablestruct.qq = qq;
    ui->label_qq->setText(qq);
}
//设置邮箱
void PersonInfoDisplayKit::setEmail(QString email)
{
    m_mailtablestruct.email = email;
    ui->label_email->setText(email);
}
//设置地址
void PersonInfoDisplayKit::setAddress(QString address)
{
    m_mailtablestruct.address = address;
    ui->label_address->setText(address);
}
//设置mail信息
void PersonInfoDisplayKit::setMailInfo(MailTableStruct mailtable)
{
    m_mailtablestruct = mailtable;
    this->setPersonName(m_mailtablestruct.personName);
    this->setSex(m_mailtablestruct.sex);
    this->setAge(m_mailtablestruct.age);
    this->setBirthDate(m_mailtablestruct.birthdate);
    this->setIndustry(m_mailtablestruct.industry);
    this->setOccupation(m_mailtablestruct.occupation);
    this->setTel(m_mailtablestruct.tel);
    this->setQQ(m_mailtablestruct.qq);
    this->setEmail(m_mailtablestruct.email);
    this->setAddress(m_mailtablestruct.address);
    this->setRecentUpdate(m_mailtablestruct.recentUpdateDate);
}

//最近更新日期
void PersonInfoDisplayKit::setRecentUpdate(QDate date)
{
    ui->label_recentupdate->setText(QString("最近更新-%1年%2月%3日").arg(date.year()).arg(date.month()).arg(date.day()));
}
//详情按钮按下槽函数
void PersonInfoDisplayKit::onDetailButtonClicked(bool)
{
    callApplyMailDetail(m_mailtablestruct);
}
//删除按钮按下槽函数
void PersonInfoDisplayKit::onDeleteButtonClicked(bool)
{
    callApplyDeleteMailInfo(m_mailtablestruct);
}
//事件过滤器
bool PersonInfoDisplayKit::eventFilter(QObject *obj, QEvent *event)
{
   if(this == obj)
    {
        if(event->type() == QEvent::MouseButtonPress){
            ui->widget->setStyleSheet("#widget{background-color: rgb(140,140,140);font: 16pt '微软雅黑';color: rgb(0, 0, 0);border:10px;border-color: rgb(209, 209, 209);border-style: solid;}");
            timer.setInterval(300);
            timer.start();
        }
        else if(event->type() == QEvent::MouseButtonRelease){
            ui->widget->setStyleSheet("#widget{background-color: rgb(250,250,250);font: 16pt '微软雅黑';color: rgb(0, 0, 0);border:10px;border-color: rgb(209, 209, 209);border-style: solid;}");
            m_PressedTime = 0;
            timer.stop();
        }
    }
    return QWidget::eventFilter(obj, event);
}
//检测当前删除按钮是否可用
void PersonInfoDisplayKit::checkDeleteButtonStatus()
{
    if(m_DeleteButtonIsvisiable){
        m_DeleteButtonIsvisiable = false;
        ui->buttonDelete->setVisible(false);
    }
    else{
        m_DeleteButtonIsvisiable = true;
        ui->buttonDelete->setVisible(true);
    }
}
//按下后的计时函数
void PersonInfoDisplayKit::onPressedTimerFunc()
{
    if(m_PressedTime < 1)
    {
        m_PressedTime++;
    }
    else
    {
        checkDeleteButtonStatus();
        m_PressedTime = 0;
        timer.stop();
    }
    LOG("m_PressedTime=%d",m_PressedTime);
}
PersonInfoDisplayKit::~PersonInfoDisplayKit()
{
    delete ui;
}
