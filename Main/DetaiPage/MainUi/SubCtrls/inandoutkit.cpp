#include "inandoutkit.h"
#include "Kit/LogInfo/clog.h"
#include <QEvent>

InAndOutKit::InAndOutKit(InAndOutType type,QWidget *parent) : QWidget(parent)
{
    gridlayout.addWidget(&m_labeInorOutText,0,0);
    gridlayout.addWidget(&m_labeInorOutnum,1,0);
    gridlayout.setRowStretch(0,1);
    gridlayout.setRowStretch(1,2);
    gridlayout.setSpacing(0);
    gridlayout.setMargin(0);
    //this->setFixedSize(400,120);
    this->setLayout(&gridlayout);
   /* ---------------------------------------------------------------------------*/
    this->setStyleSheet("background:rgb(249,219,97);");
    m_labeInorOutText.setStyleSheet("color:rgba(0,0,0,125);font-size:14pt;");
    m_labeInorOutnum.setStyleSheet("color:rgb(0,0,0);font-size:20pt");
    m_labeInorOutText.setAlignment(Qt::AlignCenter);
   // m_labeInorOutnum.setAlignment(Qt::AlignHCenter);
    m_labeInorOutnum.setAlignment(Qt::AlignCenter);
    this->installEventFilter(this);
    /*---------------------------------------------------------------------------------*/
    currenttype = type;
    if(currenttype == InAndOutType::InType){
        m_currenttext = QString("  收入");
    }else if(currenttype == InAndOutType::OutType){
        m_currenttext = QString("  支出");
    }
    this->settext(m_currenttext);
    this->setnum(m_currentnum);
}

bool InAndOutKit::eventFilter(QObject *obj, QEvent *event)  //事件过滤器
{
    if(obj == this)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
           LOG("InAndOutKit was pressed!");
           this->setStyleSheet("background:rgb(30,219,97);");
           emit callGetInorOutData(currenttype,m_currentnum.toDouble());
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            this->setStyleSheet("background:rgb(249,219,97);");
        }
    }
    return QWidget::eventFilter(obj, event);
}

