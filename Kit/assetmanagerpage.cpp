#include "assetmanagerpage.h"
#include "ui_assetmanagerpage.h"

AssetManagerPage::AssetManagerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AssetManagerPage)
{
    ui->setupUi(this);
    QGridLayout * layout = new QGridLayout;
    layout->addWidget(ui->widget);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->page_2->setLayout(layout);
    this->raise();
    this->show();
}

AssetManagerPage::~AssetManagerPage()
{
    delete ui;
}
