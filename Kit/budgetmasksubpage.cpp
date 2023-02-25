#include "budgetmasksubpage.h"
#include "ui_budgetmasksubpage.h"

BudgetMaskSubpage::BudgetMaskSubpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BudgetMaskSubpage)
{
    ui->setupUi(this);
}

BudgetMaskSubpage::~BudgetMaskSubpage()
{
    delete ui;
}
