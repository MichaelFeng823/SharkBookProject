#include "ui_loginform.h"
#include "loginform.h"
#include "Kit/LogInfo/clog.h"
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include "Controler/GlobalInfo/GlobalDocumentPath.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include "LoginAbout/Register/registerwindow.h"
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <QFile>
using namespace SqlFunc;
using namespace ReadWriteIni;
using namespace DocPath;

LoginForm::LoginForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    hasConnected = DataBaseObj::getConnecttion(coon);
    if(hasConnected){
        QFile file(Android_Local_Userini_dir);

        if(!file.exists()||file.size() == 0){
            QString infostr = QString::fromLocal8Bit("File is not exsit,bulit it! and path:%1").arg(Android_Local_Userini_dir);
            LOG("%s",infostr.toStdString().c_str());
            file.copy("assets:/user.ini",Android_Local_Userini_dir);      //拷到安卓本地目录
            file.setPermissions(QFile::WriteOwner | QFile::ReadOwner); //以读写的方式进行拷贝
            file.close();
        }
        bool is_un = readIni(Android_Local_Userini_dir,"config","username",m_UserName);
        bool is_pass = readIni(Android_Local_Userini_dir, "config","password",m_UserPassword);
        readIni(Android_Local_Userini_dir,"config", "remember_states",m_RememberPassword);
        readIni(Android_Local_Userini_dir, "config","auto_states",m_AutoLogin);
        ui->UserNamel_ineEdit->setText(m_UserName);
        ui->UserPassword_lineEdit->setText(m_UserPassword);
        ui->checkBox_Remmber_Password->setChecked(m_RememberPassword=="1");
        ui->checkBox_AutoLogin->setChecked(m_AutoLogin=="1");
        if(ui->checkBox_AutoLogin->isChecked()){
           connect(&timer,&QTimer::timeout,this,&LoginForm::onLoginButtonPressed);
           timer.setSingleShot(true);
           timer.start(300);
           LOG("arrive 43line");
        }
        connect(ui->Pushbutton_Login,&QPushButton::clicked,this,&LoginForm::onLoginButtonPressed);
    }else{
        QMessageBox::warning(0,"error","DataBase Is Fail to Open!");
        LOG("数据库连接失败");
    }
    connect(ui->pushButton_Register,&QPushButton::clicked,this,&LoginForm::onRegisterButtonOPressed);
}
 //登录按钮槽函数
void LoginForm::onLoginButtonPressed()
{
    QString signalStr = "LoginForm->MainArea";
    QSqlQuery query(coon);
    QString u_name = ui->UserNamel_ineEdit->text();
    QString u_psd = ui->UserPassword_lineEdit->text();

    if(u_name == NULL){
        QMessageBox::warning(0,"Tips","请先将用户名填写完整");
        return;
    }
    if(u_psd == NULL){
        QMessageBox::warning(0,"Tips","请先将密码填写完整");
        return;
    }
    LOG("arrive 66!");
    int u_Id = loginCheck(query,u_name,u_psd);
    LOG("arrive 68! and u_Id:%d",u_Id);
    if(u_Id >= 0)
    {
        LOG("Success Login! and UserID:%d,UserName:%s,UserPassword:%s",u_Id,u_name.toStdString().c_str(),u_psd.toStdString().c_str());
        this->m_UserId = u_Id;
        this->m_UserName = u_name;
        this->m_UserPassword = u_psd;
        UserInfo::UserID = this->getUerID();
        UserInfo::UserName = this->getUserName();
        UserInfo::UserPassWord = this->getUserPassword();
        UserInfo::RemmberPassState = this->getRemeberPassword();
        UserInfo::AutoLogin = this->getAutoLogin();
        LOG("UserID:%d",UserInfo::UserID);
        LOG("UserName:%s",UserInfo::UserName.toStdString().c_str());
        LOG("UserPassWord:%s",UserInfo::UserPassWord.toStdString().c_str());
        LOG("RemmberPassState:%s",UserInfo::RemmberPassState.toStdString().c_str());
        LOG("AutoLogin:%s",UserInfo::AutoLogin.toStdString().c_str());
        rememberPass();
        emit sendSignalToAppPageManager(signalStr);
        LOG("emit signalStr: %s",signalStr.toStdString().c_str());
    }
    else
    {
        LOG("Error:%s",this->coon.lastError().text().toStdString().c_str());
        LOG("用户名或者密码错误！");
        QMessageBox::warning(0,QString("%1").arg(u_Id),"用户名或者密码错误！");
    }

}
bool LoginForm::rememberPass()
{
     if(ui->checkBox_Remmber_Password->isChecked()){
            QString username = ui->UserNamel_ineEdit->text();  //账号
            QString password = ui->UserPassword_lineEdit->text();  //密码

            bool is_username = writeIni(Android_Local_Userini_dir,"config","username", username);
            bool is_pass = writeIni(Android_Local_Userini_dir,"config","password", password);
            LOG("Is_UserName:%d,Is_Password:%d,Username:%s,UserPass:%s",is_username,is_pass,username.toStdString().c_str(),password.toStdString().c_str());
        }
        else{
            bool is_username = writeIni(Android_Local_Userini_dir,"config","username", "");
            bool is_pass = writeIni(Android_Local_Userini_dir,"config","password", "");
            LOG("Is_UserName:%d,Is_Password:%d,Username:%s,UserPass:%s",is_username,is_pass,"","");
        }
     m_RememberPassword = QString::number(ui->checkBox_Remmber_Password->isChecked());
     m_AutoLogin = QString::number(ui->checkBox_AutoLogin->isChecked());

     writeIni(Android_Local_Userini_dir,"config","remember_states", m_RememberPassword);
     writeIni(Android_Local_Userini_dir,"config","auto_states", m_AutoLogin);
     return true;
}
//注册按钮槽函数
void LoginForm::onRegisterButtonOPressed()
{
    RegisterWindow registerwindow;
    registerwindow.raise();
    if(registerwindow.exec() == QDialog::Accepted)
    {
        LOG("Success rgeister!");
    }
}
LoginForm::~LoginForm()
{
    LOG("LoginForm::~LoginForm()");
    delete ui;
}
