#include "scrollareakit.h"
#include <QTouchEvent>
#include "Util/clog.h"
#include <QDate>

ScrollAreaKit::ScrollAreaKit(thisType type,int setnum,QWidget *parent) : QWidget(parent),m_currentnum(setnum)
{
    m_thistype = type;
    goUptimer = new QTimer;
    goDowntimer = new QTimer;

    goUptimer->setSingleShot(true);
    goDowntimer->setSingleShot(true);
    connect(goUptimer,&QTimer::timeout,this,[=]{
            goUp();
    });
    connect(goDowntimer,&QTimer::timeout,this,[=]{
            goDown();
    });
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    this->installEventFilter(this);
    buildLayout();
    buildstyle();
    buildvector();

    setdate(setnum);
}
 //构建布局
void ScrollAreaKit::buildLayout()
{
    TopLabel.setAlignment(Qt::AlignCenter);
    MidLabel.setAlignment(Qt::AlignCenter);
    ButtomLabel.setAlignment(Qt::AlignCenter);

    TopSpliterLabel.setMinimumHeight(1);
    ButtomSpliterLabel.setMinimumHeight(1);

    m_vlayout.addWidget(&TopLabel,0,0);
    m_vlayout.addWidget(&TopSpliterLabel,1,0);
    m_vlayout.addWidget(&MidLabel,2,0);
    m_vlayout.addWidget(&ButtomSpliterLabel,3,0);
    m_vlayout.addWidget(&ButtomLabel,4,0);

    m_vlayout.setRowStretch(0,20);
    m_vlayout.setRowStretch(1,1);
    m_vlayout.setRowStretch(2,20);
    m_vlayout.setRowStretch(3,1);
    m_vlayout.setRowStretch(4,20);

    m_vlayout.setSpacing(0);
    m_vlayout.setMargin(0);

    m_gridlayout.addLayout(&m_vlayout,0,0);
    m_gridlayout.setSpacing(0);
    m_gridlayout.setMargin(0);

    this->setLayout(&m_gridlayout);
}
//构建样式
void ScrollAreaKit::buildstyle()
{
    this->setStyleSheet("background-color:white;");
    TopLabel.setStyleSheet("background-color:white;color:rgba(0,0,0,125);font-family:'Microsoft YaHei';font-size:20pt;");
    TopSpliterLabel.setStyleSheet("background-color:rgb(134,134,134);");
    MidLabel.setStyleSheet("background-color:white;color:rgb(0,0,0);font-family:'Microsoft YaHei';font-size:20pt;");
    ButtomSpliterLabel.setStyleSheet("background-color:rgb(134,134,134);");
    ButtomLabel.setStyleSheet("background-color:white;color:rgba(0,0,0,125);font-family:'Microsoft YaHei';font-size:20pt;");
}
//构建容器
void ScrollAreaKit::buildvector()
{
    if(m_thistype == thisType::yearType)
    {
        m_Years.clear();
        for(int i = QDate::currentDate().year() - 50;i < QDate::currentDate().year()+10; i++)
        {
            m_Years.append(i);
        }
         m_vector = m_Years;
    }
    else if(m_thistype == thisType::monthType)
    {
        m_vector = m_Months;
    }
    else if(m_thistype == thisType::dayType)
    {
        m_Days.clear();
        for(int i = 1; i < 32; i++)
        {
            m_Days.append(i);
        }
        m_vector = m_Days;
    }
    Buttom = m_vector.size();
}
////设置日期
//void ScrollAreaKit::setdate()
//{
//    if(m_thistype == thisType::yearType)
//    {
//        LOG("arrived line 91");
//        m_currentIndex = m_vector.indexOf(QDate::currentDate().year());
//        LOG("arrived line 93");
//    }
//    else if(m_thistype == thisType::monthType)
//    {
//        LOG("arrived line 95");
//        m_currentIndex = m_vector.indexOf(QDate::currentDate().month());
//        LOG("arrived line 97");
//    }
//    else if(m_thistype == thisType::dayType)
//    {
//        LOG("arrived line 101");
//        m_currentIndex = m_vector.indexOf(QDate::currentDate().day());
//        LOG("arrived line 103");
//    }
//    setcontent();
//}

//设置日期
void ScrollAreaKit::setdate(int datenum)
{
    LOG("the num is:%d",datenum);
    m_currentIndex = m_vector.indexOf(datenum);
    setcontent();
}
bool ScrollAreaKit::eventFilter(QObject *obj, QEvent *event)
{
    if(this == obj)
    {
        QTouchEvent *touchevent = (QTouchEvent*)event;
        if(event->type() == QEvent::TouchBegin)
        {
            event->accept();
            m_TempPoint = touchevent->touchPoints().first().scenePos().toPoint();
            return true;
        }
        if(event->type() == QEvent::TouchUpdate)
        {
            event->accept();

            QList<QTouchEvent::TouchPoint> touchUpdatePoints = touchevent->touchPoints();
            //  假设此时场景，手指滑动了
            QPoint nowPoint = touchUpdatePoints.at(0).screenPos().toPoint();  // screenPos 当前坐标的位置
            // endPoint = nowPoint;
            auto offset = m_TempPoint.y() - nowPoint.y();
            if(offset > 0){
                if(m_currentIndex + 1 < Buttom){
                     goDowntimer->start(25);
                }
            }
            else{
                if(m_currentIndex > Top){
                    goUptimer->start(25);
                }
            }
            return true;
        }
        if(event->type() == QEvent::TouchCancel)
        {
            event->accept();
            return false;
        }
        if(event->type() == QEvent::TouchEnd)
        {
           event->accept();
            return false;
        }
         return false;
    }
     return QWidget::eventFilter(obj, event);
}
//设置内容
void ScrollAreaKit::setcontent()
{
    if(m_currentIndex == Buttom-1)
    {
        LOG("arrived line 153!");
        TopLabel.setText(QString::number(m_vector[Buttom-2]));
        MidLabel.setText(QString::number(m_vector[Buttom-1]));
        ButtomLabel.setText(QString::number(m_vector[Top]));
    }
    else if(m_currentIndex == Top)
    {
        LOG("arrived line 160!");
        TopLabel.setText(QString::number(m_vector[Buttom-1]));
        MidLabel.setText(QString::number(m_vector[Top]));
        ButtomLabel.setText(QString::number(m_vector[Top+1]));
    }
    else
    {
        LOG("arrived line 167!");
        TopLabel.setText(QString::number(m_vector[m_currentIndex-1]));
        MidLabel.setText(QString::number(m_vector[m_currentIndex]));
        ButtomLabel.setText(QString::number(m_vector[m_currentIndex+1]));
    }
    LOG("m_currentIndex is %d",m_currentIndex);
}
 //向上滑动
void ScrollAreaKit::goUp()
{
    --m_currentIndex;
    setcontent();
}
//向下滑动
void ScrollAreaKit::goDown()
{
    ++m_currentIndex;
    setcontent();
}
