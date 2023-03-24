#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/3/24
/// @file    : morepage.h
/// @brief   : 更多功能子界面
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
namespace Ui {
class MorePage;
}

class MorePage : public QWidget
{
    Q_OBJECT

public:
    explicit MorePage(QWidget *parent = nullptr);
    ~MorePage();


public slots:
    void onButtonReturnClicked();         //返回关闭按钮槽函数

private:
    Ui::MorePage *ui;
};

#endif // MOREPAGE_H
