#ifndef APPPAGEMANAGER_H
#define APPPAGEMANAGER_H
#pragma once
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2022/12/13
/// @file    : apppagemanager.h
/// @brief   : app页面管理类 做一些页面之间的切换与回收
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
#include <QObject>
#include <QApplication>
#include <QMutex>
#include "LoginAbout/Login/loginform.h"
#include "Main/MainFrame/mainarea.h"

class AppPageManager : public QObject
{
    Q_OBJECT

public:
     static AppPageManager * instance();      //获取页面管理类单例对象

     int initPage();                   //初始化界面

     bool getUserInfoAfterSuccessLogin();   //获取用户信息在成功登录后

public slots:
     //页面管理界面收到信号 字符串格式 "open *** page-close *** page"
      void receiveSignal(QString str);
signals:

private:
    LoginForm * login = nullptr;
    MainArea * mainarea = nullptr;
private:
     AppPageManager(QObject *parent = nullptr);
     bool requestPermission(QString permission);              //请求系统权限


private:
     static AppPageManager * self;
     static QMutex my_Mutex;
};

#endif // APPPAGEMANAGER_H
