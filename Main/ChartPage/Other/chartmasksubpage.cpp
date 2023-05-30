#include "chartmasksubpage.h"
#include "ui_chartmasksubpage.h"

ChartMaskSubPage::ChartMaskSubPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartMaskSubPage)
{
    ui->setupUi(this);
}

ChartMaskSubPage::~ChartMaskSubPage()
{
    delete ui;
}
