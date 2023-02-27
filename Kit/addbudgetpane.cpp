#include "addbudgetpane.h"
#include "ui_addbudgetpane.h"

AddBudgetPane::AddBudgetPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddBudgetPane)
{
    ui->setupUi(this);
}

AddBudgetPane::~AddBudgetPane()
{
    delete ui;
}
