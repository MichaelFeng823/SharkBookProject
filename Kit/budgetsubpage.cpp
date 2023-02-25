#include "budgetsubpage.h"
#include "ui_budgetsubpage.h"

BudGetSubPage::BudGetSubPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BudGetSubPage)
{
    ui->setupUi(this);
    this->raise();
    this->show();
}

BudGetSubPage::~BudGetSubPage()
{
    delete ui;
}
