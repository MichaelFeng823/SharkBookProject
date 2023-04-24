#ifndef MINEWIDAGETINSTACKWIDGET_H
#define MINEWIDAGETINSTACKWIDGET_H

#include <QWidget>
#include "LoginAbout/ChangePassWord/changepasswordform.h"
#include "BaseClass/basecustomwidget.h"

namespace Ui {
class MinewidagetInStackWidget;
}

class MinewidagetInStackWidget : public BaseCustomWidget
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
