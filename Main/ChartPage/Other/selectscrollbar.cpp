#include "selectscrollbar.h"
#include "Kit/LogInfo/clog.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include <QScrollBar>
using namespace ScreenFunc;
SelectScrollBar::SelectScrollBar(ChartSelectType type,QWidget * parent):m_CurrentType(type)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    initCtrls();
    setDefaultClicked();
}
//初始化控件内容
void SelectScrollBar::initCtrls()
{
    model = new QStandardItemModel();
    this->setModel(model);
    switch(m_CurrentType){
        case ChartSelectType::week :{
            initWeekCtrls();
            break;
        }
        case ChartSelectType::month :{
            initMonthCtrls();
            break;
        }
        case ChartSelectType::year :{
            initYearCtrls();
            break;
        }
    }
}
//初始化周控件
void SelectScrollBar::initWeekCtrls()
{
    m_CurrentDate = QDate::currentDate();
    m_WeekNum = m_CurrentDate.weekNumber();
    LOG("weeknum:%d",m_CurrentDate.weekNumber());
    model->clear();
    model->setRowCount(1);
    model->setColumnCount(m_WeekNum);
    this->setRowHeight(0,getScreenSize().height()/21);
    for(int i = 0; i < m_WeekNum; i++,m_DefaultIndex++){
        QModelIndex index = model->index(0,i);
        this->setColumnWidth(i,getScreenSize().width()/5);
        this->setIndexWidget(index,initButtonItem(i+1));
    }
}
//初始化月控件
void SelectScrollBar::initMonthCtrls()
{
    m_CurrentDate = QDate::currentDate();
    m_MonthNum = m_CurrentDate.month();
    model->clear();
    model->setRowCount(1);
    model->setColumnCount(m_MonthNum);
    this->setRowHeight(0,getScreenSize().height()/21);
    for(int i = 0; i < m_MonthNum; i++,m_DefaultIndex++){
        QModelIndex index = model->index(0,i);
        this->setColumnWidth(i,getScreenSize().width()/5);
        this->setIndexWidget(index,initButtonItem(i+1));
    }
}
//初始化年控件
void SelectScrollBar::initYearCtrls()
{
    m_CurrentDate = QDate::currentDate();
    int year = m_CurrentDate.year();
    year = year - m_YearNum +1;
    model->clear();
    model->setRowCount(1);
    model->setColumnCount(m_YearNum);
    this->setRowHeight(0,getScreenSize().height()/21);
    for(int i = 0;i < m_YearNum; i++,year++,m_DefaultIndex++){
        QModelIndex index = model->index(0,i);
        this->setColumnWidth(i,getScreenSize().width()/5);
        this->setIndexWidget(index,initButtonItem(year));
    }
}
//初始化table item
QPushButton * SelectScrollBar::initButtonItem(int id)
{
    QString str;
    QPushButton * button = new QPushButton();
    setButtonNormal(button);
    if(m_CurrentType == ChartSelectType::week){
        str = QString("%1周").arg(id);
    }
    else if(m_CurrentType == ChartSelectType::month){
        str = QString("%1月").arg(id);
    }else if(m_CurrentType == ChartSelectType::year){
        str = QString("%1年").arg(id);
    }
    button->setText(str);
    button->setProperty("id",id);
    connect(button,&QPushButton::clicked,this,[=](){
        QPushButton * button = static_cast<QPushButton*>(sender());
        emit ItemClicked(m_CurrentType,button->property("id").toInt());
    });
    connect(button,&QPushButton::clicked,this,&SelectScrollBar::onButtonClicked);
    buttonlist.append(button);
    return button;
}
//按钮点击槽函数
void SelectScrollBar::onButtonClicked()
{
     QPushButton * button = static_cast<QPushButton*>(sender());
     int id = button->property("id").toInt();
     for(QPushButton * it : buttonlist)
     {
         if(it->property("id").toInt() == id){
             setButtonClicked(it);
             continue;
         }
         setButtonNormal(it);
     }
}
//设置按钮正常状态
void SelectScrollBar::setButtonNormal(QPushButton * button)
{
    button->setStyleSheet("color:rgb(147,147,147);"
                          "background-color:rgb(255,255,255);"
                          "border:none;"
                          "font: 19pt \"微软雅黑\";");

}
//设置按钮点击状态
void SelectScrollBar::setButtonClicked(QPushButton * button)
{
    button->setStyleSheet("color:rgb(0,0,0);"
                          "background-color:rgb(255,255,255);"
                          "border-bottom:5px;"
                          "border-color: rgb(0,0,0);"
                          "border-style: solid;"
                          "font: 19pt \"微软雅黑\";");
}
//设置默认选中
void SelectScrollBar::setDefaultClicked()
{
    if(!buttonlist.isEmpty()){
        this->setCurrentIndex(model->index(0,m_DefaultIndex-1));
        buttonlist.last()->clicked();
    }
}
