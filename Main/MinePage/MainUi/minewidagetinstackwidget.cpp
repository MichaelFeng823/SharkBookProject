#include "minewidagetinstackwidget.h"
#include "ui_minewidagetinstackwidget.h"
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include "Controler/GlobalInfo/GlobalDocumentPath.h"
#include "Controler/PublicApi/PublicDbFunc.h"

using namespace ReadWriteIni;
using namespace DocPath;

MinewidagetInStackWidget::MinewidagetInStackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MinewidagetInStackWidget)
{
    ui->setupUi(this);
    LOG("UserName:%s",UserInfo::UserName.toStdString().c_str());
    ui->label_UserName->setText(UserInfo::UserName);
    connect(ui->ChangePasswordButton,&QPushButton::clicked,this,&MinewidagetInStackWidget::onChangePasswordButtonClicked);\
    connect(ui->ExitLoginButton,&QPushButton::clicked,this,&MinewidagetInStackWidget::onExitLoginButtonClicked);
}
void MinewidagetInStackWidget::onChangePasswordButtonClicked()
{
    form = new ChangePassWordForm(this->nativeParentWidget());
}

void MinewidagetInStackWidget::onExitLoginButtonClicked()
{
    ExitLogin();
    QString strSignal = "MainArea->LoginForm";
    emit ExitLoginSignal(strSignal);
}
 void MinewidagetInStackWidget::ExitLogin()     //退出登录函数
 {
        bool is_username = writeIni(Android_Local_Userini_dir,"config","username",UserInfo::UserName);
        bool is_pass = writeIni(Android_Local_Userini_dir,"config","password", UserInfo::UserPassWord);
        LOG("Is_UserName:%d,Is_Password:%d,Username:%s,UserPass:%s",is_username,is_pass,UserInfo::UserName.toStdString().c_str(),UserInfo::UserPassWord.toStdString().c_str());
        writeIni(Android_Local_Userini_dir,"config","remember_states",UserInfo::RemmberPassState);
        writeIni(Android_Local_Userini_dir,"config","auto_states","0");
 }

MinewidagetInStackWidget::~MinewidagetInStackWidget()
{
    delete ui;
}
