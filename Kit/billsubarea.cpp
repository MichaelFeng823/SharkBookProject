#include "billsubarea.h"
#include "ui_billsubarea.h"

BillSubArea::BillSubArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BillSubArea)
{
    ui->setupUi(this);
}

BillSubArea::~BillSubArea()
{
    delete ui;
}
