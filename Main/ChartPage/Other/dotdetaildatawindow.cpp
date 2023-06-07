#include "dotdetaildatawindow.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include "Controler/GlobalInfo/PublicUerInfo.h"
using namespace ScreenFunc;
DotDetailDataWindow::DotDetailDataWindow(InOrOut type,QWidget * parent):m_Type(type)
{
    Q_UNUSED(parent);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(450,400);
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
//设置数据
void DotDetailDataWindow::setData(DotData data){
    m_Data = data;
    checkData();
    update();
}
//检测数据
void DotDetailDataWindow::checkData()
{
    if(m_Data.message.isEmpty()){
        m_CurrentState = DataState::NoData;
        moveInNoData();
    }
    else{
        m_CurrentState = DataState::HaveData;
        moveInHaveData();
    }

}
//在没有数据时移动
void DotDetailDataWindow::moveInNoData()
{
    int x = m_Data.x-this->width()/2;
    trianglePos = this->width()/2;
    if(m_Data.x - this->width() / 2 < 0){
       x = 0;
       trianglePos = trianglePos - qAbs(m_Data.x - this->width() / 2);
    }
    if(m_Data.x + this->width()/ 2 > getScreenSize().width()){
       x = getScreenSize().width() - this->width();
       trianglePos = trianglePos + qAbs(m_Data.x + this->width()/ 2 - getScreenSize().width());
    }
    LOG("trianglePos:%d",trianglePos);
    this->move(x,m_Data.y-200);
}
//在有数据时移动
void DotDetailDataWindow::moveInHaveData()
{
    int x = m_Data.x-this->width()/2;
    trianglePos = this->width()/2;
    if(m_Data.x - this->width() / 2 < 0){
       x = 0;
       trianglePos = trianglePos - qAbs(m_Data.x - this->width() / 2);
    }
    if(m_Data.x + this->width()/ 2 > getScreenSize().width()){
       x = getScreenSize().width() - this->width();
       trianglePos = trianglePos + qAbs(m_Data.x + this->width()/ 2 - getScreenSize().width());
    }
    LOG("trianglePos:%d",trianglePos);
    this->move(x,m_Data.y);
}
//重写绘画事件
void DotDetailDataWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.fillRect(this->rect(),QColor(0,0,0,0));
    if(m_CurrentState == DataState::NoData)
        drawThisInNoData(&painter);
    else if(m_CurrentState == DataState::HaveData)
        drawThisInHaveData(&painter);
}
//绘制没有数据时候的窗体
void DotDetailDataWindow::drawThisInNoData(QPainter * painter)
{
    QRect rect(0,0,450,150);
    drawFrameRect(rect,painter);
    drawTextInNoData(painter,rect.center());
    drawLoweTriangleRect(painter);

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
//绘制下三角当没有数据时
void DotDetailDataWindow::drawLoweTriangleRect(QPainter * painter)
{
    QPoint point(m_Data.x,m_Data.y);
    point = mapTo(this,point);
    const QPoint lowtriangle[3] = {
        QPoint(trianglePos-8,145),
        QPoint(trianglePos+8,145),
        QPoint(trianglePos,200)
    };
    LOG("drawLoweTriangleRect");
    painter->setPen(QPen(QColor(60,60,60)));
    painter->setBrush(QBrush(QColor(60,60,60)));
    painter->save();
    painter->drawConvexPolygon(lowtriangle,3);
    painter->restore();
}
//绘制有数据时候的窗体
void DotDetailDataWindow::drawThisInHaveData(QPainter * painter)
{ 
    QRect titlerect(30,30,390,70);
    drawFrameRect(checkMessageSize(),painter);
    drawTitleRect(titlerect,painter);
    drawTitleText(titlerect.center(),painter);
    drawMessageContent(painter);
}
//检测消息的条数
QRect DotDetailDataWindow::checkMessageSize()
{
    if(m_Data.message.size() == 2)
        return QRect(0,0,450,100+100*2);
    else if(m_Data.message.size() == 1)
        return QRect(0,0,450,100+100*1);
    return QRect(0,0,450,100+100*3);
}
//绘制窗体框架
void DotDetailDataWindow::drawFrameRect(QRect rect,QPainter * painter)
{
    painter->setBrush(QBrush(QColor(60,60,60)));
    painter->save();
    painter->drawRoundedRect(rect,15,15);
    painter->restore();
}
//绘制title Rect （最大三比交易）
void DotDetailDataWindow::drawTitleRect(QRect rect,QPainter * painter)
{
    painter->setPen(QPen(QColor(110,110,110)));
    painter->setBrush(QBrush(QColor(110,110,110)));
    painter->save();
    painter->drawRoundedRect(rect,10,10);
    painter->restore();
}
//绘制title text
void DotDetailDataWindow::drawTitleText(QPoint pos,QPainter * painter)
{
    QFont font;
    font.setPixelSize(30);
    painter->setFont(font);
    pos = QPoint(pos.x()-font.pixelSize()*3,pos.y()+font.pixelSize()/2);
    painter->setPen(QPen(QColor(255,255,255)));
    painter->drawText(pos,"最大3比交易");
}
//绘制最大三比交易
void DotDetailDataWindow::drawMessageContent(QPainter * painter)
{
    int rows = 0;
    for(int i = 0; i < m_Data.message.size() && rows < 3; i++,rows++){
        drawSingleMessageContent(QRect(0,100+60*i,450,80),m_Data.message[i],painter);
    }
}
//绘制单条message content
void DotDetailDataWindow::drawSingleMessageContent(QRect rect,MaxThreeMessage message,QPainter * painter)
{
    QString IconPath;
    QString PayType;
    QPoint pos;
    if(m_Type == InOrOut::Income){
            IconPath = QString(":/BillPage/image/Bill_Income_%1.jpg").arg(message.typeId);
            PayType = GLOBALDATA::incomemenuvector[message.typeId];
    }
    else if (m_Type == InOrOut::Expand){
            IconPath = QString(":/BillPage/image/BillPage_Expand_%1.jpg").arg(message.typeId);
            PayType = GLOBALDATA::expandituremenuvector[message.typeId-1];
    }
    pos.setX(30);
    pos.setY(rect.top()+80);
    drawCirclePic(pos,IconPath,painter);
}
//绘制圆形图片
void DotDetailDataWindow::drawCirclePic(QPoint pos,QString url,QPainter*painter)
{
    LOG("drawCirclePic");
    int weight = 60;
    int height = 60;
    QPixmap pixmap(url);
    QPixmap roundPixmap(weight,height);
    roundPixmap.fill(Qt::transparent);
    QPainterPath path;
    path.addEllipse(pos.x(),pos.y(),weight,height);
    painter->begin(&roundPixmap);
    painter->setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    painter->setClipPath(path);
    painter->drawPixmap(pos.x(),pos.y(),weight,height,pixmap.copy(QRect(45,45,100,100)));
    painter->end();
    painter->begin(this);
}




