#ifndef PHOTOFRAME_H
#define PHOTOFRAME_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTimer>
#include <BaseClass/basecustomwidget.h>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/05/16
/// @file    : photoframe.h
/// @brief   : 相框类
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************

namespace Ui {
class PhotoFrame;
}

class PhotoFrame : public  BaseCustomWidget
{
    Q_OBJECT

public:
    explicit PhotoFrame(QWidget *parent = nullptr);
    ~PhotoFrame();
public:
    void addPic();                       //添加图片
    void setPicVector(QVector<QByteArray> * vector);      //设置图片容器引用
private:
    void initOther();                                   //初始化其他
    void initTableviewPreviewList();                    //初始化图片预览列表
    void addPicItem(QByteArray data);                   //添加item
    void popPic();                                      //弹出图片
public slots:
    void onAutoDisplayClicked();                        //自动放映槽函数
    void onManualDisplayClicked();                      //手动放映槽函数
    void onAutoDisplayStart();                          //当定时轮播启动时
    void onReceiveChangePicRequest();                   //当收到改变图片请求
    void onReceivePicItemState(bool);                       //当收到PicItemState时

signals:
    void requestOnePic();                               //请求一张图片
    void requestClose();                                //请求关闭

private:
    int colcounts = 10;                                  //列数
private:
    Ui::PhotoFrame *ui;
    QStandardItemModel * model;
    QVector<QByteArray> * PicVector;
    QTimer * autoDisplayTimer;                           //自动轮播定时器
    int m_CurrentIndex;                          //当前图片index
    bool m_CurrentIsAuto = false;                //当前状态
};

#endif // PHOTOFRAME_H
