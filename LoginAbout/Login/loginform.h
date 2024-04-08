#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include"Controler/DataControl/databaseobj.h"
#include <QTimer>
namespace Ui {
class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

public:
    int getUerID(){return m_UserId;}
    QString getUserName(){return m_UserName;}
    QString getUserPassword(){return m_UserPassword;}
    QString getRemeberPassword(){return m_RememberPassword;}
    QString getAutoLogin(){return m_AutoLogin;}

protected:
    bool rememberPass();
signals:
    void sendSignalToAppPageManager(QString);

public slots:
    void onLoginButtonPressed();      //登录按钮槽函数
    void onRegisterButtonOPressed();      //注册按钮槽函数
private:
    Ui::LoginForm *ui;
    QTimer timer;                          //定时器
    QString m_RememberPassword = "-1";      //记住密码
    QString m_AutoLogin = "-1";             //自动登录
    int m_UserId = -1;                     //用户id
    QString m_UserName;               //用户名
    QString m_UserPassword;           //用户密码
    QSqlDatabase coon;
    bool hasConnected = false;                //数据库是否已连接
};

#endif // LOGINFORM_H
