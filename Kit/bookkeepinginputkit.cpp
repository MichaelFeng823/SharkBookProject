#include "bookkeepinginputkit.h"
#include "ui_bookkeepinginputkit.h"
#include "Util/clog.h"
#include "Controler/PublicDbFunc.h"

using namespace ScreenFunc;

BookkeepingInputKit::BookkeepingInputKit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookkeepingInputKit)
{
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->setupUi(this);
    ui->Calculate_label->installEventFilter(this);
    ConnectFunc();
    defaultSetDate();
    this->setFixedSize(getScreenSize().width(),parent->height()/3);
    this->move(0,parent->height()/3*2);
    this->raise();
    this->show();
}
BookkeepingInputKit::BookkeepingInputKit(BillTableStruct billinfo,bool isModify,QWidget *parent):
    QWidget(parent),
    ui(new Ui::BookkeepingInputKit)
{
    m_IsModify = isModify;
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->setupUi(this);
    ui->Calculate_label->installEventFilter(this);
    ConnectFunc();
    setYear(billinfo.date.year());
    setMonth(billinfo.date.month());
    setDay(billinfo.date.day());
    setBillNo(billinfo.billNo);
    setRemarks(billinfo.remarks);
    setResultNum(billinfo.moneyAmount);
    receiveTimeInfo(m_CurrentYear,m_CurrentMonth,m_CurrentDay);
    this->setFixedSize(getScreenSize().width(),parent->height()/3);
    this->move(0,parent->height()/3*2);
    this->raise();
    this->show();
}

//绑定函数
void BookkeepingInputKit::ConnectFunc()
{
    /*------------------------------------------数字------------------------------------------------*/
    connect(ui->NumberZeroButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);
    connect(ui->NumberOneButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);
    connect(ui->NumberTwoButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);
    connect(ui->NumberThreeButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);
    connect(ui->NumberFourButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);
    connect(ui->NumberFiveButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);
    connect(ui->NumberSixButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);
    connect(ui->NumberSevenButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);
    connect(ui->NumberEightButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);
    connect(ui->NumberNineButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);


    /*------------------------------------------小数点-----------------------------------------------*/
    connect(ui->PointerNumberButton,&QPushButton::clicked,this,&BookkeepingInputKit::numberButtonFunc);

    /*------------------------------------------操作符-----------------------------------------------*/
    connect(ui->AddNumbolButton,&QPushButton::clicked,this,&BookkeepingInputKit::operatorButtonFunc);
    connect(ui->SubNumbolButton,&QPushButton::clicked,this,&BookkeepingInputKit::operatorButtonFunc);

    /*------------------------------------------删除按钮---------------------------------------------*/
    connect(ui->DeleteButton,&QPushButton::clicked,this,&BookkeepingInputKit::deleteButtonFunc);

    /*------------------------------------------完成按钮---------------------------------------------*/
    connect(ui->FinishButton,&QPushButton::clicked,this,&BookkeepingInputKit::finishButtonFunc);

    /*------------------------------------------打开日历按钮------------------------------------------*/
    connect(ui->TodayButton,&QPushButton::clicked,this,&BookkeepingInputKit::openCalendarFunc);
}
BookkeepingInputKit::~BookkeepingInputKit()
{
    delete ui;
}
//数字按钮槽函数
void BookkeepingInputKit::numberButtonFunc()
{
    LOG("触发数字按钮槽函数");
    QPushButton * btn = static_cast<QPushButton*>(sender());
    QString str = btn->text();
    str.size();
    if (!isExsitOperatorNuMbol){
        operatorNumOne.append(str);
        strList.append(str);
        LOG("operatorNumOne:%s",operatorNumOne.toStdString().c_str());
    }
    else{
        operatorNumTwo.append(str);
        strList.append(str);
        LOG("operatorNumTwo:%s",operatorNumTwo.toStdString().c_str());
    }
    LOG("the str is:%s",str.toStdString().c_str());
    QString stroutput;
    foreach(QString strdemo,strList)
        stroutput+=strdemo;
    LOG("the Expressstr is : %s",stroutput.toStdString().c_str());
    this->update();
}
//操作符按钮槽函数
void BookkeepingInputKit::operatorButtonFunc()
{
    LOG("触发操作符按钮槽函数");
    QPushButton * btn = static_cast<QPushButton*>(sender());
    QString str = btn->text();
    str.size();
    if (!isExsitOperatorNuMbol){
        if(!operatorNumOne.isEmpty())
            operatorNumbol = str;
            strList.append(str);
            isExsitOperatorNuMbol = true;
    }
    else{
        if(operatorNumTwo.isEmpty() && !operatorNumbol.isEmpty()){
            operatorNumbol = str;
            strList.pop_back();
            strList.append(str);
        }
        else
        {
            //计算一次
            LOG("计算结果");
            CalculateResult();
            operatorNumbol = str;
            operatorNumTwo.clear();
            strList.clear();
            for (int i = 0;i < operatorNumOne.size();i++){
                strList.append(operatorNumOne.at(i));
            }
            strList.append(operatorNumbol);
        }
    }
    QString stroutput;
    foreach(QString strdemo,strList)
        stroutput+=strdemo;
    LOG("the Expressstr is : %s",stroutput.toStdString().c_str());
    this->update();
}
 void BookkeepingInputKit::setResultNum(double num)
 {
     operatorNumOne = QString::number(num);
     for(int i = 0;i < operatorNumOne.size();i++)
     {
         strList.append(operatorNumOne.at(i));
     }
 }
//设置备注内容
void BookkeepingInputKit::setRemarks(QString str)
{
    ui->Remarks_lineEdit->setText(str);
}

 //完成按钮槽函数
void BookkeepingInputKit::finishButtonFunc()
{
    LOG("触发完成按钮槽函数");
    if(!strList.isEmpty()){
        if(!operatorNumOne.isEmpty() && !operatorNumTwo.isEmpty() && !operatorNumbol.isEmpty())
        {
            CalculateResult();
            operatorNumTwo.clear();
            operatorNumbol.clear();
            strList.clear();
            for (int i = 0;i < operatorNumOne.size();i++){
                strList.append(operatorNumOne.at(i));
            }
        }
        else
        {
            if(!operatorNumbol.isEmpty()){
                strList.pop_back();
                operatorNumbol.clear();
            }
            else
            {
                if(!operatorNumOne.isEmpty())
                {
                    LOG("the final result is : %s",operatorNumOne.toStdString().c_str());
                    QDate date;
                    date.setDate(m_CurrentYear,m_CurrentMonth,m_CurrentDay);
                    m_RemarksStr = ui->Remarks_lineEdit->text();
                    if(m_IsModify){
                        LOG("Modify");
                        emit finishBookkeepingInputAndModify(m_billno,date,operatorNumOne.toDouble(),m_RemarksStr);
                    }
                    else{
                        LOG("Insert");
                        emit finishBookkeepingInput(date,operatorNumOne.toDouble(),m_RemarksStr);
                    }
                }
                else
                {
                    LOG("the operatorNumOne is empty!");
                }
            }
        }
    }
    else{
        LOG("请输入内容!");
    }
    this->update();
}
//打开日历按钮槽函数
void BookkeepingInputKit::openCalendarFunc()
{
    LOG("触发日历按钮槽函数");
    m_Calendar = new Calendar(CalendarType::YearMonthDay,m_CurrentYear,m_CurrentMonth,m_CurrentDay);
    connect(m_Calendar,&Calendar::returnTimeInfoToYearMonthDay,this,&BookkeepingInputKit::receiveTimeInfo);
}
//删除按钮槽函数
void BookkeepingInputKit::deleteButtonFunc()
{
    LOG("触发删除按钮槽函数");
    if(strList.size() > 0){
        if(strList.last() == "+" | strList.last() == "-"){
            isExsitOperatorNuMbol = false;
            operatorNumbol.clear();
        }
        else
        {
                if(!isExsitOperatorNuMbol){
                    operatorNumOne.remove(operatorNumOne.size()-1,1);
                    LOG("operatorNumOne:%s",operatorNumOne.toStdString().c_str());
                }
                else{
                    operatorNumTwo.remove(operatorNumTwo.size()-1,1);
                    LOG("operatorNumTwo:%s",operatorNumTwo.toStdString().c_str());
                }
        }
        if(strList.size() > 0)
        {
           strList.pop_back();
        }
        QString stroutput;
        foreach(QString strdemo,strList)
            stroutput+=strdemo;
        LOG("the Expressstr is : %s",stroutput.toStdString().c_str());
        this->update();
    }
}
//计算结果
double BookkeepingInputKit::CalculateResult()
{
    LOG("开始计算结果!");
    if(!operatorNumOne.isEmpty() && !operatorNumbol.isEmpty() && !operatorNumTwo.isEmpty()){
        LOG("operatorNumOne:%s operatorNumbol:%s operatorNumTwo:%s",operatorNumOne.toStdString().c_str(),operatorNumbol.toStdString().c_str(),operatorNumTwo.toStdString().c_str());
        if(operatorNumbol == "+")
        {
           operatorNumOne = QString::number(operatorNumOne.toDouble() + operatorNumTwo.toDouble());
        }
        else if(operatorNumbol == "-")
        {
           operatorNumOne = QString::number(operatorNumOne.toDouble() - operatorNumTwo.toDouble());
        }
    }

    LOG("the current calculate result is: %s",operatorNumOne.toStdString().c_str());
    return  operatorNumOne.toDouble();
}

//事件过滤器
bool BookkeepingInputKit::eventFilter(QObject *obj, QEvent *event)
{
    if(ui->Calculate_label == obj)
    {
        if(ui->Calculate_label->text().isEmpty() | strList.isEmpty())
        {
           ui->Calculate_label->setText("0.00");
        }
        else
        {
            QString stroutput;
            foreach(QString strdemo,strList)
                stroutput+=strdemo;
            ui->Calculate_label->setText(stroutput);
        }
    }
    return QWidget::eventFilter(obj, event);
}
//默认设置日期
void BookkeepingInputKit::defaultSetDate()
{
   m_CurrentYear = QDate::currentDate().year();
   m_CurrentMonth = QDate::currentDate().month();
   m_CurrentDay = QDate::currentDate().day();
}
//收到时间信息槽函数
void BookkeepingInputKit::receiveTimeInfo(int year,int month,int day)
{
    QDate date = QDate::currentDate();
    if(year == date.year() && month == date.month() && day == date.day()){
        ui->TodayButton->setText("");
        ui->TodayButton->setStyleSheet("border-image: url(:/BookkeepingInputKit/image/todayicon.jpg);");
    }
    else
    {
        ui->TodayButton->setStyleSheet("background-color:white;color:black;");
        ui->TodayButton->setText(QString("%1/%2/%3").arg(year).arg(month).arg(day));
    }
    LOG("Recieve TimeInfo - Year:%d,Month:%d,Day:%d",year,month,day);
    m_CurrentYear = year;
    m_CurrentMonth = month;
    m_CurrentDay = day;
}
