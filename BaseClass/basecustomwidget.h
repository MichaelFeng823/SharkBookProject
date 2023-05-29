#ifndef BASECUSTOMWIDGET_H
#define BASECUSTOMWIDGET_H

#include <QWidget>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/4/24
/// @file    : basecustomwidget.h
/// @brief   : 自定义widget基类
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
class BaseCustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseCustomWidget(QWidget *parent = nullptr);
public:
     virtual void setActiveWindow(bool state){m_IsActiveWindow = state;}  //设置当前Active状态
     bool isActiveWindow(){return m_IsActiveWindow;}
protected:
     bool m_IsActiveWindow = false;                   //当前是否是Active状态
     signals:
};

#endif // BASECUSTOMWIDGET_H
