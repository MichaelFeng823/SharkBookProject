#include "calendar.h"
#include "Controler/PublicDbFunc.h"
using namespace ScreenFunc;
Calendar::Calendar(CalendarType type,int year,int month,int day,QWidget *parent) : QDialog(parent),m_thisType(type),m_year(year),m_month(month),m_day(day)
{
    //this->setAttribute(Qt::WA_DeleteOnClose,true);
    buildLayout();
    buildStyle();
    this->raise();
    this->show();
}
Calendar::Calendar(CalendarType type,int year,int month,QWidget *parent) : QDialog(parent),m_thisType(type),m_year(year),m_month(month)
{
    //this->setAttribute(Qt::WA_DeleteOnClose,true);
    buildLayout();
    buildStyle();
    this->raise();
    this->show();
}

//构建布局
void Calendar::buildLayout()
{
    //--------------------------------------------------midwidget---------------------------------------------------
    midwidget = new QWidget();
    if(m_thisType == CalendarType::YearMonthDay){
        yearkit = new ScrollAreaKit(thisType::yearType,m_year);
        monthkit = new ScrollAreaKit(thisType::monthType,m_month);
        daykit = new ScrollAreaKit(thisType::dayType,m_day);
        midwidgetlayout.addWidget(yearkit,0,0);
        midwidgetlayout.addWidget(monthkit,0,1);
        midwidgetlayout.addWidget(daykit,0,2);

    }
    else if(m_thisType == CalendarType::YearMonth){
        yearkit = new ScrollAreaKit(thisType::yearType,m_year);
        monthkit = new ScrollAreaKit(thisType::monthType,m_month);
        midwidgetlayout.addWidget(yearkit,0,0);
        midwidgetlayout.addWidget(monthkit,0,1);
    }
    midwidgetlayout.setSpacing(50);
    midwidgetlayout.setMargin(0);

    midwidget->setLayout(&midwidgetlayout);

    //--------------------------------------------------leftspacewidget and rightspacewidget--------------------------

    leftspacewidget = new QWidget();
    rightspacewidget = new QWidget();
    contentlayout.addWidget(leftspacewidget,0,0);
    contentlayout.addWidget(midwidget,0,1);
    contentlayout.addWidget(rightspacewidget,0,2);
    contentlayout.setColumnStretch(0,2);
    contentlayout.setColumnStretch(1,8);
    contentlayout.setColumnStretch(2,2);
    contentlayout.setSpacing(0);
    contentlayout.setMargin(0);

    contentwidget = new QWidget();
    contentwidget->setLayout(&contentlayout);

    //--------------------------------------------------titlewidget---------------------------------------------------

    titlewidget = new QWidget();
    cancelButton = new QPushButton();
    cancelButton->setText(QString::fromLocal8Bit("取消"));
    makeSureButton = new QPushButton();
    makeSureButton->setText(QString::fromLocal8Bit("确定"));
    titlelabel = new QLabel();
    titlelabel->setAlignment(Qt::AlignCenter);
    titlelayout.addWidget(cancelButton,0,0);
    titlelayout.addWidget(titlelabel,0,1);
    titlelayout.addWidget(makeSureButton,0,2);
    titlelayout.setSpacing(0);
    titlelayout.setMargin(10);
    titlelayout.setColumnStretch(0,3);
    titlelayout.setColumnStretch(1,8);
    titlelayout.setColumnStretch(2,3);
    connect(makeSureButton,&QPushButton::clicked,this,&Calendar::onMakeSureButtonClicked);
    connect(cancelButton,&QPushButton::clicked,this,&Calendar::onCancelButtonClicked);

    titlewidget->setLayout(&titlelayout);

    vlayout.addWidget(titlewidget,1);
    vlayout.addWidget(contentwidget,9);

    if(m_thisType == CalendarType::YearMonthDay)
    {
        settitle(QString::fromLocal8Bit("选择日期"));
    }
    else if (m_thisType == CalendarType::YearMonth)
    {
         settitle(QString::fromLocal8Bit("选择月份"));
    }

    //--------------------------------------------------mainwidget---------------------------------------------------
    mainwidget = new QWidget(this);
    mainwidget->setLayout(&vlayout);
    //mainwidget->move(0,0);


    QSize screensize = getScreenSize();
    mainwidget->setFixedSize(screensize.width(),screensize.height()/3);
    this->setFixedSize(screensize.width(),screensize.height()/3);
    this->move(0,screensize.height()/3*2);

//    outterlayout.addWidget(mainwidget,0,0);
//    outterlayout.setSpacing(0);
//    outterlayout.setMargin(0);
//    this->setLayout(&outterlayout);
}
//设置title
void Calendar::settitle(QString titlestr)
{
    if(titlelabel != nullptr)
    {
        titlelabel->setText(titlestr);
    }
}
//构建样式
void Calendar::buildStyle()
{
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    //this->setStyleSheet("background-color:rgba(255,0,0,50);");
    titlewidget->setStyleSheet("QWidget{background:rgb(255,255,255);border-bottom:1px ;border-color: rgba(140,140,140,120);border-style: solid;}");
    mainwidget->setStyleSheet("background-color:white");
    cancelButton->setStyleSheet("border:none;color:rgb(0,0,0);font-family:'Microsoft YaHei';font-size:18pt;");
    makeSureButton->setStyleSheet("border:none;color:rgb(0,0,0);font-family:'Microsoft YaHei';font-size:18pt;");
    titlelabel->setStyleSheet("border:none;color:rgb(0,0,0);font-family:'Microsoft YaHei';font-size:18pt;");
}

void Calendar::onMakeSureButtonClicked()       //确认按钮槽函数
{
    m_year = yearkit->getCurrentValue();
    m_month = monthkit->getCurrentValue();
    if(m_thisType == CalendarType::YearMonthDay){
        m_day = daykit->getCurrentValue();
        emit returnTimeInfoToYearMonthDay(m_year,m_month,m_day);
    }
    else if(m_thisType == CalendarType::YearMonth){
        emit returnTimeInfo(m_year,m_month);
    }
    this->accept();
}

void Calendar::onCancelButtonClicked()         //取消按钮槽函数
{
    this->close();
}
 Calendar::~Calendar()
 {
     LOG("~Calendar()");
 }
