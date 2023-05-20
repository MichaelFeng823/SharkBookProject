#ifndef CIRCULARDIAGRAM_H
#define CIRCULARDIAGRAM_H
#pragma once
#include <QWidget>
#include <QObject>
#include <QPropertyAnimation>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023-4-11
/// @file    : circulardiagram
/// @brief   : 环形图控件基础类
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
enum CircleDirection
{
    clockwise = 1,          //顺时针
    anticlockwise           //逆时针
};
const int ACYCLE = 360*16;
const int ANIMATIONTIME = 500;
class CircularDiagram : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double m_FreedPercent READ getFreedPercent WRITE setFreedPercent)
public:
    explicit CircularDiagram(QWidget *parent = nullptr);

public:
    void setInnerRadius(int radius){m_InnerRadius = radius;}
    void setOutterRadius(int radius){m_OutterRadius = radius;}
    void setUsedPercent(double percent){m_UsedPercent = percent;}
    void setFreedPercent(double percent){m_FreedPercent = percent;m_UsedPercent = 1 - m_FreedPercent;}
    void setCircleDirection(CircleDirection Direction){m_Direction = Direction;}

    int getInnerRadius(){return m_InnerRadius;}
    int getOutterRadius(){return m_OutterRadius;}
    double getUsedPercent(){return m_UsedPercent;}
    double getFreedPercent(){return m_FreedPercent;}
    CircleDirection getCircleDirection(){return m_Direction;}
    int getRingWidth(){if(m_OutterRadius > m_InnerRadius){return m_OutterRadius - m_InnerRadius;}else{return -1;}}

    void setBackgroundColor(QColor color){m_backgroundColor = color;}
    void setUsedRingColor(QColor color){m_usedRingColor = color;}
    void setFreedRingColor(QColor color){m_freedRingColor = color;}
    void setTextColor(QColor color){m_textColor = color;}
    void setNumColor(QColor color){m_numColor = color;}

    void setTextFont(QFont font){m_TextFont = font;}
    void setNumFont(QFont font){m_NumFont = font;}

    void setCenterText(QString text){m_Text = text;}
    void setNum(QString num){m_Num = num;}

    void setStartAnglePosition(int angle){m_StartAnglePosition = angle * 16;}
    void setAnimationEndValue(double);    //设置动画终点值

    void initFontStyle();           //初始化字体样式
    void initAnimation();           //初始化动画

private:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void drawCircle(QPainter * painter);
    void drawInnerCircle(QPainter * painter);
    void drawFreesRing(QPainter * painter);
    void drawUsedRing(QPainter * painter);
    void drawText(QPainter * painter);
    void drawNum(QPainter * painter);
    void checkDirection();
private:
    QColor m_backgroundColor;        //背景色
    QColor m_usedRingColor;          //占用的环的颜色
    QColor m_freedRingColor;         //未占用的环的颜色
    QColor m_textColor;              //文本颜色
    QColor m_numColor;               //字体颜色

    QFont m_TextFont;                //文本字体
    QFont m_NumFont;                 //数字字体

    QString m_Text;                  //文本内容
    QString m_Num;                   //数字内容

    QPropertyAnimation * animation = nullptr;  //动画对象指针


private:
    int m_InnerRadius;                //内圆半径
    int m_OutterRadius;               //外圆半径
    int m_ringWidth;                  //环厚
    double m_UsedPercent;             //使用占比
    double m_FreedPercent;            //剩余占比
    int m_StartAnglePosition = 90*16; //开始的位置
    int m_EndAnglePosition;           //停止的位置

    CircleDirection m_Direction = CircleDirection::clockwise;      //方向
};

#endif // CIRCULARDIAGRAM_H
