#include "dotdetaildatawindow.h"

DotDetailDataWindow::DotDetailDataWindow(DataState state,QWidget * parent):m_CurrentState(state)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(500,400);
    initTimer();
}
//初始化定时器
void DotDetailDataWindow::initTimer()
{
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,[=](){
        stopTimer();
        this->close();
    });
}
//启动定时器
void DotDetailDataWindow::startTimer()
{
    if(!timer->isActive()){
        timer->start(3500);
    }
}
//关闭定时器
void DotDetailDataWindow::stopTimer()
{
    if(timer->isActive()){
        timer->stop();
    }
}
//重写绘画事件
void DotDetailDataWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.fillRect(this->rect(),QColor(0,0,0,0));
    drawThisInNoData(&painter);
}
//绘制没有数据时候的窗体
void DotDetailDataWindow::drawThisInNoData(QPainter * painter)
{
    QRect rect(0,0,450,150);
    drawFrameRect(rect,painter);
    drawTextInNoData(painter,rect.center());
}
//绘制文本当没有数据时
void DotDetailDataWindow::drawTextInNoData(QPainter * painter,QPoint pos)
{
    QFont font;
    font.setPixelSize(30);
    painter->setFont(font);
    pos = QPoint(pos.x()-font.pixelSize()*2,pos.y()+font.pixelSize()/2);
    painter->setPen(QPen(QColor(255,255,255)));
    painter->drawText(pos,"没有费用");
}
//绘制有数据时候的窗体
void DotDetailDataWindow::drawThisInHaveData(QPainter * painter)
{ 
    QRect rect(0,0,400,400);
    drawFrameRect(rect,painter);
}
//绘制窗体框架
void DotDetailDataWindow::drawFrameRect(QRect rect,QPainter * painter)
{
    painter->setBrush(QBrush(QColor(60,60,60)));
    painter->save();
    painter->drawRoundedRect(rect,10,10);
    painter->restore();
}



