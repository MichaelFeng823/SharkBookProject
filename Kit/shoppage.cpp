#include "shoppage.h"
#include "ui_shoppage.h"
ShopPage::ShopPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShopPage)
{
    ui->setupUi(this);
    this->raise();
    this->show();
}

ShopPage::~ShopPage()
{
    delete ui;
}
