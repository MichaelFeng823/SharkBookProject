#include "picitem.h"
#include <QEvent>
#include <QWheelEvent>
#include <Kit/LogInfo/clog.h>
#include <Controler/PublicApi/PublicDbFunc.h>

using namespace ScreenFunc;

PicItem::PicItem(QWidget * parent)
{
    m_scene = new QGraphicsScene();
    m_imageItem = new QGraphicsPixmapItem();
    m_scene->addItem(m_imageItem);
    setScene(m_scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);

    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    centerOn(0, 0);

    //this->resize(defaultSize);
    this->installEventFilter(this);
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    //SetImage(QImage(":/girls/image/glsbgpmpvq2tngtg.jpg"));
}

PicItem::~PicItem()
{
    m_scene->deleteLater();
    delete m_imageItem;
}

void PicItem::SetImage(const QImage &image)
{
    m_imageItem->setPixmap(QPixmap::fromImage(image));
    pix = QPixmap::fromImage(image);
    QPoint newCenter(image.width() / 2 ,image.height()/2);
    //设置scene中心到图像中点
    centerOn(newCenter);
    show();
}
void PicItem::ZoomIn()
{
    Zoom(1.1);
}

void PicItem::ZoomOut()
{
    Zoom(0.9);
}
void PicItem::Zoom(float scaleFactor)
{
    // 防止过小或过大
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    mFactor = factor;
    if (factor < minscaleFactor)
    {
        //emit SendStateToPhotoView(false);
        return;

    }
    if(factor > 10){
         return;
    }
    scale(scaleFactor, scaleFactor);
}
void PicItem::Translate(QPointF delta)
{
    int w = viewport()->rect().width();
    int h = viewport()->rect().height();
    QPoint newCenter(w / 2. - delta.x()+0.5,  h / 2. - delta.y()+0.5);
    centerOn(mapToScene(newCenter));
}

void PicItem::CheckDoubleClicked()
{
    ++mPressedCounts;
    LOG("mPressedCounts:%d",mPressedCounts);
    mFactor = minscaleFactor;
    if(mPressedCounts == 2){
        emit SendStateToPhotoView(false);
        mPressedCounts = 0;
        mIsZoomOut = false;
    }
}

bool PicItem::eventFilter(QObject *obj, QEvent *event)
{
    if(this == obj)
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        if(event->type() == QEvent::TouchBegin)
        {
            event->accept();
            if(!mIsPinchToZoom){
                m_isTranslate = true;
                CheckDoubleClicked();
                m_lastMousePos  = touchEvent->touchPoints().first().pos().toPoint();
            }
            return true;
        }
        if(event->type() == QEvent::TouchUpdate)
        {
            event->accept();
            QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
             //判断单点触控
            if (touchPoints.count() == 1) {
                if(!mIsPinchToZoom && mIsZoomOut){
                    if(m_isTranslate)
                    {
                        m_isMoveing = true;
                        mPressedCounts = 0;
                        //获取
                        QPointF mouseDelta = touchPoints.last().pos().toPoint()-m_lastMousePos;
                        Translate(mouseDelta);
                    }
                    m_lastMousePos = touchPoints.last().pos().toPoint();
                }
            }
            else if(touchPoints.count() == 2) {
                mPressedCounts = 0;
                mIsZoomOut = true;
                scalingProcess(touchPoints);
            }
            return true;
        }
        if(event->type() == QEvent::TouchCancel)
        {
            event->accept();
            if(mIsZoomOut)
                emit SendStateToPhotoView(true);
            mIsPinchToZoom = false;
            m_isTranslate = false;
            m_isPressed = false;
            m_isMoveing = false;
            return false;
        }
        if(event->type() == QEvent::TouchEnd)
        {
           event->accept();
           if(mIsZoomOut)
               emit SendStateToPhotoView(true);
           mIsPinchToZoom = false;
           m_isTranslate = false;
           m_isPressed = false;
           m_isMoveing = false;
           return false;
        }
        //return false;
    }
    return QGraphicsView::eventFilter(obj, event);
}

void PicItem::scalingProcess(QList<QTouchEvent::TouchPoint> touchPoints)
{
    //判断两点也就是双指缩放动作
    QPointF p1 = touchPoints.at(0).startPos();
    QPointF p2 = touchPoints.at(0).lastPos();
    QPointF p3 = touchPoints.at(1).startPos();
    QPointF p4 = touchPoints.at(1).lastPos();
    if (p1 == p2 || p3 == p4) {

    } else {
        double delta1 = QLineF(p1, p2).length();
        double delta2 = QLineF(p3, p4).length();
        double delta = delta1;
        if (delta1 < delta2) {
            delta = delta1;
        } else {
            delta = delta2;
        }
        double x1 = p2.x() - p1.x();
        double y1 = p2.y() - p1.y();
        double x2 = p4.x() - p3.x();
        double y2 = p4.y() - p3.y();
        double aaa = x1 * x2 + y1 * y2;
        double bbb = sqrt((pow(x1, 2) + pow(y1, 2)) * (pow(x2, 2) + pow(y2, 2)));
        double cosA = aaa / bbb;
        if (cosA < 0) {
             mIsPinchToZoom = true;
            //通过向量算出两者之间的夹角的余弦值，因为范围是（0，180，所以大于90度，余弦值必定小于0
            double endL = QLineF(p2, p4).length();
            double startL = QLineF(p1, p3).length();
            //然后来判断是放大还是缩小操作算出两指起始点以及终止点之间的距离，如果起始点间的距离大于
            //终止点间的距离则是缩小，反正放大
            if (endL > startL){
                 //放大操作
                    ZoomIn();
                }else {
                 //缩小操作
                 ZoomOut();
            }
            update();//此处更新控件使之放大缩小
        }
    }
}

void PicItem::setPosAndSize(int x,int y,int width,int height)
{
    //this->setGeometry(x,y,width,height);
    minscaleFactor = width / (float)pix.width();
    Zoom(minscaleFactor);
    //centerOn(QPoint(pix.width()*minscaleFactor/2,pix.height()*minscaleFactor/2));
    //m_imageItem->moveBy(this->rect().center().x(),this->rect().center().y());
    //centerOn(m_imageItem);
}

