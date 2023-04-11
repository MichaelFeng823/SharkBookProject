#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H
#include <QDialog>


namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();
public:
    void generate_Expression();        //随机生成验证表达式

public slots:
    void onRegisterButtonClicked();     //注册按钮槽函数
    void onCancelButtonClicked();       //取消按钮槽函数

private:
      bool eventFilter(QObject *obj, QEvent *event);
private:
    int verificationResult = -1;             //验证码结果

    Ui::RegisterWindow *ui;
};

#endif // REGISTERWINDOW_H
