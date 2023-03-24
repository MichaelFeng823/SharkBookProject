#include "morepage.h"
#include "ui_morepage.h"
#include "Util/clog.h"

MorePage::MorePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorePage)
{
    ui->setupUi(this);
    this->raise();
    this->show();
    connect(ui->pushButton_return,&QPushButton::clicked,this,&MorePage::onButtonReturnClicked);
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
