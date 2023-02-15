#ifndef MINEWIDAGETINSTACKWIDGET_H
#define MINEWIDAGETINSTACKWIDGET_H

#include <QWidget>
#include "changepasswordform.h"

namespace Ui {
class MinewidagetInStackWidget;
}

class MinewidagetInStackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MinewidagetInStackWidget(QWidget *parent = nullptr);
    ~MinewidagetInStackWidget();
    void ExitLogin();     //退出登录函数


public slots:
    void onChangePasswordButtonClicked();

    void onExitLoginButtonClicked();

signals:
    void ExitLoginSignal(QString);

private:
    ChangePassWordForm * form;
    Ui::MinewidagetInStackWidget *ui;

};

#endif // MINEWIDAGETINSTACKWIDGET_H
