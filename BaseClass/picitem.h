#ifndef PICITEM_H
#define PICITEM_H
#pragma once
#include <QLabel>
#include <QList>
#include <QTouchEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/5/11
/// @file    :
/// @brief   : 图片加载item基类
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************

class PicItem:public QGraphicsView
{
     Q_OBJECT
public:
    PicItem(QWidget * parent = nullptr);
    ~ PicItem();
    void setPosAndSize(int x,int y,int width,int height);
    void SetImage(const QImage & image);

private:
    bool eventFilter(QObject *obj, QEvent *event); //事件过滤器
    void scalingProcess(QList<QTouchEvent::TouchPoint> touchPoints);

private:
    void ZoomIn();
    void ZoomOut();
    void Zoom(float scaleFactor);
    void Translate(QPointF delta);
    void CheckDoubleClicked();

signals:
    void SendStateToPhotoView(bool);       //发送信号给photoview

private:               
    QPoint m_lastMousePos;
    QGraphicsScene * m_scene;
    QGraphicsPixmapItem * m_imageItem;
    QPixmap pix;
    float minscaleFactor;
private:
    bool m_isTranslate;
    bool m_isMoveing = false;                //是否单点正在移动
    bool m_isPressed = false;                //是否已经被按下
    bool mIsPinchToZoom = false;             //是否已经双指放大
    bool mIsZoomOut = false;                 //是否放大显示
    int mPressedCounts = 0;                  //按下次数
    int mFactor = 0;                         //
};

#endif // PICITEM_H
