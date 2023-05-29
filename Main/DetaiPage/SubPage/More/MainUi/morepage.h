#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>
#include <QPointer>
#include <Main/DetaiPage/SubPage/More/SubPage/girlpicwidget.h>
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
    void onButtonGirlPicClicked();        //美女图片打开按钮槽函数

private:
    Ui::MorePage *ui;
    QPointer<GirlPicWidget> m_GirlPic;   //美女图片窗口
};

#endif // MOREPAGE_H
