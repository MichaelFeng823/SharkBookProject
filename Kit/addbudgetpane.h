#ifndef ADDBUDGETPANE_H
#define ADDBUDGETPANE_H
#pragma once
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/02/28
/// @file    : AddBudgetPane.h
/// @brief   : 添加预算窗口
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
#include <QWidget>
#include "Controler/PublicDataStruct.h"

namespace Ui {
class AddBudgetPane;
}

class AddBudgetPane : public QWidget
{
    Q_OBJECT

public:
    explicit AddBudgetPane(TypeBudget type,QWidget *parent = nullptr);
    ~AddBudgetPane();

public slots:
    void onButtonOkClicked();       //点击确认按钮槽函数
    void onOperatorNumButtonClicked(); //数字输入操作区按钮点击槽函数
    void onButtonDeleteClicked();   //点击删除按钮槽函数
protected:
    void checkTypeBudget();         //检验当前预算类型
    void initSlots();               //初始化槽函数

private:
    bool eventFilter(QObject *obj, QEvent *event); //事件过滤器

private:
    Ui::AddBudgetPane *ui;
    TypeBudget m_TypeBudget;        //当前类型
    bool IsExsitPoint = false;      //当前是否存在.
    QStringList m_strList;          //当前字符串容器
};

#endif // ADDBUDGETPANE_H
