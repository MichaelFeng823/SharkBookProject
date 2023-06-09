#include "dotdetaildatawindow.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include "Controler/GlobalInfo/PublicUerInfo.h"
using namespace ScreenFunc;
DotDetailDataWindow::DotDetailDataWindow(InOrOut type,ChartSelectType charttype,QWidget * parent):m_Type(type),m_ChartSelectType(charttype)
{
    Q_UNUSED(parent);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

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
    LOG("m_Data.message.size:%d",m_Data.message.size());
    if(m_Data.message.isEmpty()){
        m_CurrentState = DataState::NoData;
        this->setFixedSize(450,435);
        moveInNoData();
    }
    else{
        m_CurrentState = DataState::HaveData;
        this->setFixedSize(520,435);
        moveInHaveData();
    }
}
//在没有数据时移动
void DotDetailDataWindow::moveInNoData()
{
    int x = m_Data.x-this->width()/2;
    trianglePos.setX(this->width()/2);
    trianglePos.setY(145);
    if(m_Data.x - this->width() / 2 < 0){
       x = 0;
       trianglePos.setX(trianglePos.x() - qAbs(m_Data.x - this->width() / 2));
    }
    if(m_Data.x + this->width()/ 2 > getScreenSize().width()){
       x = getScreenSize().width() - this->width();
       trianglePos.setX(trianglePos.x() + qAbs(m_Data.x + this->width()/ 2 - getScreenSize().width()));
    }
    this->move(x,m_Data.y-200);
}
//在有数据时移动
void DotDetailDataWindow::moveInHaveData()
{
    mainRect = checkMessageSize();
    LOG("m_Data.x:%d",m_Data.x);
    LOG("m_Data.y:%d",m_Data.y);
    int x = m_Data.x;
    m_triangleEndPosx = 35;
    trianglePos.setX(0);
    trianglePos.setY(qAbs(m_Data.y -120));
    m_triangleEndPosy = qAbs(m_Data.y -120);
    if(m_Data.x + this->width() > getScreenSize().width()){
        LOG("1");
       x = m_Data.x - this->width();
       trianglePos.setX(520);
       m_triangleEndPosx = this->width()-35;
    }
    if(m_Data.y - 120 <= 0){
        LOG("2");
        m_triangleEndPosy = this->rect().topLeft().y();
        trianglePos.setY(0);
        this->move(x,120 - qAbs(m_Data.y -120));
        return;
    }
    if(m_Data.y >= mainRect.height()+120){
        LOG("3");
        LOG("mainRect height:%d",mainRect.height());
        trianglePos.setY(mainRect.height());
        m_triangleEndPosy = mainRect.height()-20;
        this->move(x,m_Data.y - mainRect.height());
        return;
    }
    this->move(x,120);
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
        QPoint(trianglePos.x()-8,trianglePos.y()),
        QPoint(trianglePos.x()+8,trianglePos.y()),
        QPoint(trianglePos.x(),trianglePos.y()+55)
    };
    painter->setPen(QPen(QColor(60,60,60)));
    painter->setBrush(QBrush(QColor(60,60,60)));
    painter->save();
    painter->drawConvexPolygon(lowtriangle,3);
    painter->restore();
}
//绘制左侧或者右侧三角当有数据时
void DotDetailDataWindow::drawLowTriangleRectInHaveData(QPainter * painter)
{
    const QPoint lowtriangle[3] = {
        QPoint(m_triangleEndPosx,m_triangleEndPosy+8),
        QPoint(m_triangleEndPosx,m_triangleEndPosy-8),
        QPoint(trianglePos.x(),trianglePos.y())
    };
    painter->setPen(QPen(QColor(60,60,60)));
    painter->setBrush(QBrush(QColor(60,60,60)));
    painter->save();
    painter->drawConvexPolygon(lowtriangle,3);
    painter->restore();
}
//绘制有数据时候的窗体
void DotDetailDataWindow::drawThisInHaveData(QPainter * painter)
{ 
    QRect titlerect(65,30,390,70);
    drawFrameRect(mainRect,painter);
    drawTitleRect(titlerect,painter);
    drawTitleText(titlerect.center(),painter);
    drawMessageContent(painter);
    drawLowTriangleRectInHaveData(painter);
}
//检测消息的条数
QRect DotDetailDataWindow::checkMessageSize()
{
    if(m_Data.message.size() == 2)
        return QRect(35,0,450,130+100*2);
    else if(m_Data.message.size() == 1)
        return QRect(35,0,450,150+100*1);
    return QRect(35,0,450,105+100*3);
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
    QRect rect;
    for(int i = 0; i < m_Data.message.size() && rows < 3; i++,rows++){
        rect.setLeft(35);
        rect.setTop(125+75*i);
        rect.setWidth(450);
        rect.setHeight(80);
        drawSingleMessageContent(rect,m_Data.message[i],painter);
    }
    drawTextInBottom(rect.bottomLeft(),m_Data,painter);
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
    message.PayType = PayType;
    pos.setX(65);
    pos.setY(rect.top());
    drawCirclePic(pos,IconPath,painter);
    painter->begin(this);
    drawDatePayTypeAmountText(pos,message,painter);
}
//绘制圆形图片
void DotDetailDataWindow::drawCirclePic(QPoint pos,QString url,QPainter*painter)
{
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
}
//绘制日期支付类型金额文本
void DotDetailDataWindow::drawDatePayTypeAmountText(QPoint pos,MaxThreeMessage message, QPainter*painter)
{
    QFont font;
    font.setPixelSize(24);
    painter->setFont(font);
    painter->setPen(QPen(QColor(255,255,255)));
    painter->save();
    //LOG("message.PayType:%s",message.PayType.toStdString().c_str());
    painter->drawText(pos.x()+80,pos.y()+40,message.PayType);
    painter->drawText(pos.x()+150,pos.y()+40,message.date.toString("yyyy/MM/dd"));
    painter->drawText(pos.x()+300,pos.y()+40,QString::number(message.Amount,'f',2));
    painter->restore();
}
//绘制底部的文本
void DotDetailDataWindow::drawTextInBottom(QPoint pos,DotData data,QPainter * painter)
{
    QString str;
    float amount = 0;
    for(int i = 0; i < data.message.size(); i++){
        amount += data.message[i].Amount;
    }
    switch (m_ChartSelectType) {
        case ChartSelectType::week:{
            str = "当日总支出:%1";
            break;
        }
        case ChartSelectType::month:{
            str = "当日总支出:%1";
            break;
        }
        case ChartSelectType::year:{
            str = "当月总支出:%1";
            break;
        }
    }
    str = str.arg(QString::number(amount,'f',2));

    QFont font;
    font.setPixelSize(24);
    painter->setFont(font);
    painter->setPen(QPen(QColor(255,255,255)));
    painter->save();
    painter->drawText(pos.x()+30,pos.y()+20,str);
    painter->restore();
}




