#include "changepasswordform.h"
#include "ui_changepasswordform.h"
#include "Controler/PublicUerInfo.h"
#include "Controler/PublicDbFunc.h"
#include "Controler/databaseobj.h"
#include "Controler/GlobalDocumentPath.h"
#include <QMessageBox>
using namespace  ScreenFunc;
using namespace SqlFunc;
using namespace ReadWriteIni;
using namespace DocPath;

ChangePassWordForm::ChangePassWordForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangePassWordForm)
{
    ui->setupUi(this);
    QSize screensize = getScreenSize();
    this->setFixedSize(screensize.width(),screensize.height()/2);
    this->move(0,screensize.height()/2);
    ui->lineEdit_Username->installEventFilter(this);
    ui->lineEdit_UserPassword->installEventFilter(this);
    ui->lineEdit_UserPasswordRepeat->installEventFilter(this);
    ui->lineEdit_Username->setText(UserInfo::UserName);
    connect(ui->pushButton_Sure,&QPushButton::clicked,this,&ChangePassWordForm::onButtonSureClicked);
    connect(ui->pushButton_Cancel,&QPushButton::clicked,this,&ChangePassWordForm::onButtonCancelClicked);
    this->raise();
    this->show();
    hasConnected = DataBaseObj::getConnecttion(coon);
}

void ChangePassWordForm::onButtonSureClicked()
{
     QSqlQuery query(coon);
     QString u_name = ui->lineEdit_Username->text();
     QString u_psd = ui->lineEdit_UserPassword->text();
     QString u_psd_rpeat = ui->lineEdit_UserPasswordRepeat->text();

     if(u_psd != u_psd_rpeat){
          QMessageBox::warning(0,"error","两次密码不一致");
          ui->lineEdit_UserPassword->clear();
          ui->lineEdit_UserPasswordRepeat->clear();
          return;
     }
     if(u_psd == UserInfo::UserPassWord){
         QMessageBox::warning(0,"error","修改密码与原密码一致");
         return;
     }
     if(hasConnected){
         bool ret = changeUserPassword(coon,query,u_name,u_psd);
         LOG("ret:%d",ret);
         if(ret){
             UserInfo::UserPassWord = u_psd;
             bool is_username = writeIni(Android_Local_Userini_dir,"config","username",UserInfo::UserName);
             bool is_pass = writeIni(Android_Local_Userini_dir, "config","password",UserInfo::UserPassWord);

             writeIni(Android_Local_Userini_dir,"config", "remember_states",UserInfo::RemmberPassState);
             writeIni(Android_Local_Userini_dir, "config","auto_states",UserInfo::AutoLogin);
             QMessageBox::warning(0,"success","修改成功");
             this->close();
         }
         else
         {
             QMessageBox::warning(0,"fail","修改失败");
             this->close();
         }
     }
     else
     {
         QMessageBox::warning(0,"fail","数据库连接失败！");
         this->close();
     }

}
void ChangePassWordForm::onButtonCancelClicked()
{
    this->close();
    delete this;
}
bool ChangePassWordForm::eventFilter(QObject *obj, QEvent *event)
{

      if(ui->lineEdit_Username->text().isEmpty() || ui->lineEdit_UserPassword->text().isEmpty() || ui->lineEdit_UserPasswordRepeat->text().isEmpty())
      {
          ui->pushButton_Sure->setDisabled(true);
      }
      else
      {
          ui->pushButton_Sure->setEnabled(true);
      }
     return QWidget::eventFilter(obj, event);
}

ChangePassWordForm::~ChangePassWordForm()
{
    delete ui;
}
