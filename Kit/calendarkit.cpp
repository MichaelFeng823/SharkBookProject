#include "calendarkit.h"
#include "Util/clog.h"
#include <QEvent>
#include <QDate>

CalendarKit::CalendarKit(QWidget *parent) : QWidget(parent)
{
   gridlayout.addWidget(&m_labelyear,0,0);
   gridlayout.addWidget(&m_labelspqce,0,1);
   gridlayout.addWidget(&m_labelmonth,1,0);
   gridlayout.addWidget(&m_labelarrow,1,1);
   gridlayout.setRowStretch(0,1);
   gridlayout.setRowStretch(1,2);
   gridlayout.setColumnStretch(0,3);
   gridlayout.setColumnStretch(1,1);
   gridlayout.setSpacing(0);
   gridlayout.setMargin(0);
   //this->setFixedSize(200,120);
   this->setLayout(&gridlayout);
   this->setYear(QString::number(QDate::currentDate().year()));
   this->setMonth(QString::number(QDate::currentDate().month()));
   m_labelarrow.setStyleSheet("border-image: url(:/DetialToolButton/image/calendar_arrow.jpg);");
   QString str = "#m_labelyear{background:rgb(0,0,0);color:rgb(255,0,0)} #m_labelmonth{background:rgb(0,0,0);color:rgb(255,0,0)}";
   this->setStyleSheet("background:rgb(249,219,97);");
   m_labelyear.setStyleSheet("color:rgba(0,0,0,125);font-size:14pt;");
//color:rgb(0,0,0);
//	background-color: rgb(255, 217, 24);
   m_labelmonth.setStyleSheet("color:rgb(0,0,0);font-size:20pt");
   m_labelyear.setAlignment(Qt::AlignCenter);
   m_labelmonth.setAlignment(Qt::AlignCenter);
   this->installEventFilter(this);
}

bool CalendarKit::eventFilter(QObject *obj, QEvent *event)  //事件过滤器
{
    if(obj == this)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
           LOG("Calendar was pressed!");
           //this->setStyleSheet("background:rgb(30,219,97);");
           CallCalendar(getYear(),getMonth());
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
           // this->setStyleSheet("background:rgb(249,219,97);");
        }
    }
    return QWidget::eventFilter(obj, event);
}
//调用日历控件
void CalendarKit::CallCalendar(QString year,QString month)
{
    LOG("year:%d,month:%d",year.toInt(),month.toInt());
    m_Calendar = new Calendar(CalendarType::YearMonth,year.toInt(),month.toInt(),this->nativeParentWidget());
    //Calendar m_Calendar(CalendarType::YearMonth,year.toInt(),month.toInt());
    connect(m_Calendar,&Calendar::returnTimeInfo,this,&CalendarKit::receiveTimeInfo);
}

 void CalendarKit::receiveTimeInfo(int year,int month)
 {
     this->setYear(QString::number(year));
     this->setMonth(QString::number(month));
     sendSetDateSignal(m_currentyear.toInt(),m_currentmonth.toInt());
     //m_Calendar->close();
 }
