#ifndef MENUICON_H
#define MENUICON_H
#pragma once
#include <QWidget>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2022/12/27
/// @file    : MenuIcon.h
/// @brief   : 记账菜单图标类
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
class MenuIcon : public QWidget
{
    Q_OBJECT
public:
    explicit MenuIcon(QString stylestr,QWidget *parent = nullptr);

public:
    void setid(int num){this->m_Id = num;}
    void setname(QString name){this->m_Name = name;}
    int getid(){return m_Id;}
    QString getname(){return m_Name;}

signals:


private:
    int m_Id;
    QString m_Name;





};

#endif // MENUICON_H
