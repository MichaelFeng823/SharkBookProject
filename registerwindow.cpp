#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "Controler/PublicDbFunc.h"
#include "Controler/databaseobj.h"
#include <ctime>
#include <QMessageBox>
using namespace  ScreenFunc;
using namespace  SqlFunc;
RegisterWindow::RegisterWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    QSize screensize = getScreenSize();
    this->resize(screensize.width(),screensize.height()/2);
    this->move(0,screensize.height()/2);
    ui->lineEdit_RegUserName->installEventFilter(this);
    ui->lineEdit_RegUserPass->installEventFilter(this);
    ui->lineEdit_RegUserPassRepeat->installEventFilter(this);
    ui->lineEdit_RegVerification->installEventFilter(this);
    generate_Expression();
    connect(ui->pushButton_Register,&QPushButton::clicked,this,&RegisterWindow::onRegisterButtonClicked);
    connect(ui->pushButton_Cancle,&QPushButton::clicked,this,&RegisterWindow::onCancelButtonClicked);
}

//注册按钮槽函数
void RegisterWindow::onRegisterButtonClicked()
{
    QString username = ui->lineEdit_RegUserName->text();
    QString userpass = ui->lineEdit_RegUserPass->text();
    QString userpassrepeat = ui->lineEdit_RegUserPassRepeat->text();
    int resultnum = ui->lineEdit_RegVerification->text().toInt();
    if(userpass != userpassrepeat){
         LOG("Arrive_REG_Line_36");
         ui->label_title->setText("两次密码不一致");
         ui->lineEdit_RegUserPass->clear();
         ui->lineEdit_RegUserPassRepeat->clear();
         this->exec();
         return;
    }

    if(resultnum != verificationResult)
    {
         ui->label_title->setText("验证码错误");
         generate_Expression();
         this->exec();
         return;
    }
    QSqlDatabase coon;
    coon.transaction();
    bool hasConnected = DataBaseObj::getConnecttion(coon);
    if(hasConnected){
        QSqlQuery sqlquery(coon);
        int ret = registerUser(coon,sqlquery,username,userpass);
        LOG("ret:%d",ret);
        if(ret == -2)
            QMessageBox::warning(0,"error","用户已存在，请不要重复注册！");
        else if(ret == -1){
            QMessageBox::question(0,"error","注册失败！");
        }
        else if(ret == 1){
            QMessageBox::information(0,"Success","注册成功！");
            this->accept();
        }
    }
    else{
        LOG("数据库连接失败 IN RegisterWindow!");
        this->reject();
    }
}
//取消按钮槽函数
void RegisterWindow::onCancelButtonClicked()
{
    this->close();
}

void RegisterWindow::generate_Expression()       //随机生成验证表达式
{
    qsrand(time(NULL));

    int num1 = qrand() % 50;
    int num2 = qrand() % 50;
    int operate_name = qrand() % 50;
    QString operate_Numbol;
    if(operate_name % 2 == 0)
    {
        operate_Numbol = "+";
        verificationResult = num1 + num2;

    }
    else if(operate_name % 2 == 1)
    {
        operate_Numbol = "-";
        verificationResult = num1 - num2;
    }
    QString express_Str = QString("%1 %2 %3 = ?").arg(num1).arg(operate_Numbol).arg(num2);
    ui->label_Express->setText(express_Str);
}

bool RegisterWindow::eventFilter(QObject *obj, QEvent *event)
{

      if(ui->lineEdit_RegUserName->text().isEmpty() || ui->lineEdit_RegUserPass->text().isEmpty() || ui->lineEdit_RegUserPassRepeat->text().isEmpty() || ui->lineEdit_RegVerification->text().isEmpty())
      {
          ui->pushButton_Register->setDisabled(true);
      }
      else
      {
          ui->pushButton_Register->setEnabled(true);
      }
     return QWidget::eventFilter(obj, event);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}
