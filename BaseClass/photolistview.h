#ifndef PHOTOLISTVIEW_H
#define PHOTOLISTVIEW_H
#include <QObject>
#include <QTableView>
#include <QScroller>
class PhotoListView:public QTableView
{
    Q_OBJECT
public:
    PhotoListView(QWidget *parent = nullptr);
    ~PhotoListView();

private:
    void initProperty();                     //初始化各种属性
    void initScroller();                     //初始化滚动控件
    void initScrollerPerproty();             //初始化滚动条属性
    void startScroller();                    //开启滚动控件
    void stopScroller();                     //关闭滚动控件

public slots:
    void onReceivePicitemSignal(bool);       //当收到PicItem的信号时
private:
    QScroller * mScroller = nullptr;



};

#endif // PHOTOLISTVIEW_H
