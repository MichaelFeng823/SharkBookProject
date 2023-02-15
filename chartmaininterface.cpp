#include "chartmaininterface.h"
#include "ui_chartmaininterface.h"

ChartMainInterface::ChartMainInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartMainInterface)
{
    ui->setupUi(this);
}

ChartMainInterface::~ChartMainInterface()
{
    delete ui;
}
