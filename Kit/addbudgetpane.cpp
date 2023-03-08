#include "addbudgetpane.h"
#include "ui_addbudgetpane.h"
#include "Controler/PublicDbFunc.h"

using namespace ScreenFunc;
AddBudgetPane::AddBudgetPane(TypeBudget type,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddBudgetPane),m_TypeBudget(type)
{
    ui->setupUi(this);
    ui->lineEdit->installEventFilter(this);
    ui->label_close->installEventFilter(this);
    ui->pushButton_Ok->setEnabled(false);
    this->setWindowFlag(Qt::Popup);
    this->setFixedSize(parent->width(),parent->height() * 0.5);
    this->move(0,parent->height() * 0.5);
    checkTypeBudget();
    initSlots();
    this->raise();
    this->show();
}

AddBudgetPane::~AddBudgetPane()
{
    delete ui;
}
//检验当前预算类型
void AddBudgetPane::checkTypeBudget()
{
    if(m_TypeBudget == TypeBudget::MonthlyBudget){
        ui->label_title->setText(QString("每月总预算"));
    }
    else if(m_TypeBudget == TypeBudget::YearBudget){
        ui->label_title->setText(QString("年度总预算"));
    }
}
//初始化槽函数
void AddBudgetPane::initSlots()
{
    connect(ui->pushButton_Ok,&QPushButton::clicked,this,&AddBudgetPane::onButtonOkClicked);

    QList<QPushButton*> list = ui->Keywidget->findChildren<QPushButton*>();
    LOG("子按钮控件数量:%d",list.size());
    for(int i = 0; i < list.size(); i++){
        if(list[i]->objectName() != QString("pushButton_Delete"))
         connect(list[i],&QPushButton::clicked,this,&AddBudgetPane::onOperatorNumButtonClicked);
    }
    connect(ui->pushButton_Delete,&QPushButton::clicked,this,&AddBudgetPane::onButtonDeleteClicked);
}
//点击确认按钮槽函数
void AddBudgetPane::onButtonOkClicked()
{
    QString str = ui->lineEdit->text();
    double resultNum = str.toDouble();
    LOG("最后结果为:%lf",resultNum);
}
//数字输入操作区按钮点击槽函数
void AddBudgetPane::onOperatorNumButtonClicked()
{
    QPushButton * button = static_cast<QPushButton*>(sender());
    QString btn_Text = button->text();
    if("." == btn_Text){
        if(!IsExsitPoint){
            if(m_strList.isEmpty())
                m_strList.append(QString("0"));
            IsExsitPoint = true;
        }
        else
            return;
    }
    LOG("%s",btn_Text.toStdString().c_str());
    m_strList.append(btn_Text);
    this->update();
}
//点击删除按钮槽函数
void AddBudgetPane::onButtonDeleteClicked()
{
    if(!m_strList.isEmpty()){
        if(m_strList.last() == QString("."))
            IsExsitPoint = false;
         m_strList.pop_back();
    }
    this->update();
}
//事件过滤器
bool AddBudgetPane::eventFilter(QObject *obj, QEvent *event)
{
    if(ui->lineEdit == obj)
    {
        if(ui->lineEdit->text().isEmpty() && m_strList.isEmpty())
        {
            ui->pushButton_Ok->setEnabled(false);
        }
        else
        {
            ui->pushButton_Ok->setEnabled(true);
            QString stroutput;
            foreach(QString strdemo,m_strList)
                stroutput+=strdemo;
            ui->lineEdit->setText(stroutput);
        }
    }

    if(event->type() == QEvent::MouseButtonPress){
        if(obj == ui->label_close)
            this->close();
    }

    return QWidget::eventFilter(obj, event);
}
