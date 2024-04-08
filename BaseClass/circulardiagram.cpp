#include "circulardiagram.h"
#include <QPainter>
#include <QResizeEvent>
#include "Kit/LogInfo/clog.h"


CircularDiagram::CircularDiagram(QWidget *parent) : QWidget(parent)
{
    this->setFreedRingColor(QColor(250,217,86));
    this->setBackgroundColor(QColor(255,255,255));
    this->setUsedRingColor(QColor(241,241,241));
    this->setTextColor(QColor(149,149,151));
    this->setNumColor(QColor(52,52,52));
    setAnimationEndValue(0.9);
    this->setCenterText(QString("剩余"));

    initFontStyle();
    initAnimation();
}
//初始化字体样式
void CircularDiagram::initFontStyle()
{
     m_TextFont.setFamily("Microsoft YaHei");                //文本字体
     m_TextFont.setPixelSize(getInnerRadius()/15*6);
     m_NumFont.setFamily("Microsoft YaHei");                 //数字字体
     m_NumFont.setPixelSize(getInnerRadius()/15*6);
}
//初始化动画
void CircularDiagram::initAnimation()
{
    animation = new QPropertyAnimation(this,"m_FreedPercent");
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->setDuration(ANIMATIONTIME);
    animation->setStartValue(0.0);
    connect(animation, SIGNAL(valueChanged(const QVariant&)), this, SLOT(update()));
}
//设置动画终点值
void CircularDiagram::setAnimationEndValue(double value)
{
    double realValue = 0;
    if(value >= 0){
        m_IsSurplus = true;
        realValue = value;
    }else{
        m_IsSurplus = false;
    }
    if(animation == nullptr)
        initAnimation();
    animation->setEndValue(realValue);
    animation->start(QAbstractAnimation::KeepWhenStopped);
}
void CircularDiagram::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    this->setNum(QString::number(getFreedPercent()*100,'f',1)+"%");
    painter.setRenderHint(QPainter::HighQualityAntialiasing,true);
    int side = qMin(width(),height());
    painter.translate(width()/2,height()/2);
    painter.scale(side/2/getOutterRadius(),side/2/getOutterRadius());
    drawCircle(painter);
    drawNum(painter);
    drawText(painter);

}
void CircularDiagram::resizeEvent(QResizeEvent *event)
{
    this->resize(event->size());
    int outterRadius = qMin(width(),height())/2;
    this->setOutterRadius(outterRadius);
    this->setInnerRadius(outterRadius-30);
}
void CircularDiagram::drawCircle(QPainter & painter)
{
    drawUsedRing(painter);        //1
    drawFreesRing(painter);       //2
    drawInnerCircle(painter);     //3
}                                 //绘制顺序不可随便变 123 或者 132 皆可
void CircularDiagram::drawInnerCircle(QPainter & painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_backgroundColor));
    painter.drawEllipse(QPoint(0,0),getInnerRadius(),getInnerRadius());
    painter.save();
    painter.restore();
}
void CircularDiagram::drawFreesRing(QPainter & painter)
{
    QPen pen;
    pen.setColor(m_freedRingColor);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(getRingWidth());
    painter.setPen(pen);
    checkDirection();
    painter.drawArc(QRect(-getInnerRadius()-getRingWidth()/2,-getInnerRadius()-getRingWidth()/2,getInnerRadius()*2+getRingWidth(),getInnerRadius()*2+getRingWidth()),m_StartAnglePosition,m_EndAnglePosition);
    painter.save();
    painter.restore();
    //LOG("画剩余圆");
}
void CircularDiagram::drawUsedRing(QPainter & painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_usedRingColor));
    painter.save();
    painter.drawEllipse(QPoint(0,0),getOutterRadius(),getOutterRadius());
    painter.restore();
    //LOG("画已用圆环");
}
void CircularDiagram::drawText(QPainter & painter)
{
    QRect rect = QRect(-getInnerRadius(),-15-getInnerRadius()/4,getInnerRadius()*2,m_TextFont.pixelSize());
    if(!m_IsSurplus){
        setTextColor(QColor(255,100,100));
        setCenterText("已超支");
        rect = QRect(-getInnerRadius(),-10-m_TextFont.pixelSize()/2,getInnerRadius()*2,getInnerRadius()/2);
    }
    initFontStyle();
    painter.setFont(m_TextFont);
    painter.setPen(m_textColor);
    painter.save();
    painter.drawText(rect,Qt::AlignCenter,m_Text);
    painter.restore();
    LOG("Draw test -----------------");
}
void CircularDiagram::drawNum(QPainter & painter)
{
    painter.setFont(m_NumFont);
    painter.setPen(m_numColor);
    painter.save();
    if(m_IsSurplus)
         painter.drawText(QRect(-getInnerRadius(),15,getInnerRadius()*2,getInnerRadius()/3),Qt::AlignCenter,m_Num);
    painter.restore();
}

void CircularDiagram::checkDirection()
{
    if(m_Direction == CircleDirection::clockwise){
        m_EndAnglePosition =-1*getFreedPercent()*ACYCLE;
    }
    else if(m_Direction == CircleDirection::anticlockwise){
        m_EndAnglePosition =getFreedPercent()*ACYCLE;
    }
}
