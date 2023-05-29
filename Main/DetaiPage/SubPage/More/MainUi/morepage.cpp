#include "morepage.h"
#include "ui_morepage.h"
#include "Kit/LogInfo/clog.h"

MorePage::MorePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorePage)
{
    ui->setupUi(this);
    this->raise();
    this->show();
    connect(ui->pushButton_return,&QPushButton::clicked,this,&MorePage::onButtonReturnClicked);
    connect(ui->pushButton_GirlPic,&QPushButton::clicked,this,&MorePage::onButtonGirlPicClicked);
}

MorePage::~MorePage()
{
    delete ui;
}
//返回关闭按钮槽函数
void MorePage::onButtonReturnClicked()
{
    LOG("close");
    this->close();
}
//美女图片打开按钮槽函数
void MorePage::onButtonGirlPicClicked()
{
    if(m_GirlPic == nullptr)
         m_GirlPic = new GirlPicWidget();
    m_GirlPic->raise();
    m_GirlPic->show();
    m_GirlPic->setActiveWindow(true);
}
