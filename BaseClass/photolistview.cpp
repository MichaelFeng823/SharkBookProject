#include "photolistview.h"
#include <QScrollBar>
#include <QScroller>
#include <QHeaderView>
#include <QEasingCurve>

PhotoListView::PhotoListView(QWidget *parent)
{
    initProperty();
    initScroller();
}

PhotoListView::~PhotoListView()
{

}
 //初始化各种属性
 void PhotoListView::initProperty()
 {
     QHeaderView * hor = this->horizontalHeader();
     hor->setVisible(false);
     QHeaderView * ver = this->verticalHeader();
     ver->setVisible(false);
     this->setShowGrid(false);
     this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     this->setFocusPolicy(Qt::NoFocus); // 取消Item选中后的虚线边框
     this->setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置不可编辑
     this->setMouseTracking(true);
 }

 void PhotoListView::initScroller()
 {
     mScroller = QScroller::scroller(this);
     startScroller();
     initScrollerPerproty();
 }
 //开启滚动控件
 void PhotoListView::startScroller()
 {
     mScroller->grabbedGesture(this);
     mScroller->grabGesture(this,QScroller::TouchGesture);
     this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
 }
 //初始化滚动条属性
 void PhotoListView::initScrollerPerproty()
 {
     QScrollerProperties properties;
    //设置过冲值，避免过冲导致的BUG
    #if 1
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor,0.05);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor,0.4);
    properties.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor,0.5);//设置平滑滑动速度，当滑动后手离开屏幕后,进行平滑滑动的速度,此值应介于0和1之间。值越小，速度越慢。??实际没区别
    properties.setScrollMetric(QScrollerProperties::FrameRate,QScrollerProperties::Fps60);//设置滚动过程中画面的刷新率，60帧看着舒服，帧率越低画面越跳动
//    properties.setScrollMetric(QScrollerProperties::DragStartDistance,0.002);//设置移动阀值(按下后需要移动最少多少米距离后,触发滑动),用来避免误操作
//    properties.setScrollMetric(QScrollerProperties::DecelerationFactor,0.5);//设置减速因子,值越大，减速越快，进而会影响点击释放后滚动的距离。对于大多数类型，该值应在0.1到2.0的范围内
//    properties.setScrollMetric(QScrollerProperties::AxisLockThreshold,0.5);//设置当运动方向与某一个轴的角度小于该设定值（0~1）时，则限定只有该轴方向的滚动
//    properties.setScrollMetric(QScrollerProperties::ScrollingCurve,QEasingCurve::OutQuad);//设置当鼠标释放后自动滚动到停止时的运动曲线，参数为QEasingCurve类型，不能设置为QEasingCurve::Type类型，不会隐式转换
//    properties.setScrollMetric(QScrollerProperties::MaximumClickThroughVelocity,0.5);//自动滚动过程中，鼠标点击操作会停止当前滚动，当速度大于该设定（m/s）时，鼠标事件不会传递给目标即不会停止滚动
//    properties.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime ,2);//与AcceleratingFlickSpeedupFactor配合使用。设置一个时间和加速因子，开始自动滚动后，如果在该时间（s）内检测到轻滑加速手势，则滚动速度加速到：当前速度x加速因子，加速因子必须大于等于1.0。加速后的速度也不能超过QScrollerProperties::MaximumVelocity的设定值
//    properties.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor,1);//与AcceleratingFlickMaximumTime配合使用，应>=1
//    properties.setScrollMetric(QScrollerProperties::MinimumVelocity,0.1);//设置自动滑动的最小速度，m/s，如果手势的速度小于该速度，则不会触发自动滚动，所以可以设置得小一些
//    properties.setScrollMetric(QScrollerProperties::MaximumVelocity,0.1);//设置自动滚动能达到得最大速度，m/s
//    properties.setScrollMetric(QScrollerProperties::SnapTime,0.1);//设置滚动曲线的时间因子。设置滚动的时长,值越小，滚动时间越长
//    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy,QScrollerProperties::OvershootWhenScrollable);//设置垂直向允许过量拖拽的策略，可以设置滚动条出现时开启、始终关闭、始终开启三种策略
//    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor,0.1);//设置过量拖拽的移动距离与鼠标移动距离的比例，0~1，值越小表现出的阻塞感越强
//    properties.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor,0.1);//设置过量拖拽的距离占页面的比例，0~1，比如设置0.5，过量拖拽垂直最多移动高度的一半
//    properties.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor,0.1);//设置自动滚动时允许的过量滚动距离比例，类似于OvershootDragDistanceFactor
//    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime,0.1);//设置当拖拽过量时，释放后位置恢复所用时间（s）
//    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay,1);//设置鼠标事件延迟时间，单位s。当鼠标按下后，开始手势识别，如果在该时间内开始了拖拽手势，窗口不会收到鼠标事件，相当于取消了点击事件，和滑屏关系不大*/
    #endif
    mScroller->setScrollerProperties(properties);
 }
 //关闭滚动控件
 void PhotoListView::stopScroller()
 {
     mScroller->ungrabGesture(this);
 }
 //当收到PicItem的信号时
 void PhotoListView::onReceivePicitemSignal(bool state)
 {
     if(state)
         stopScroller();
     else
         startScroller();
 }
