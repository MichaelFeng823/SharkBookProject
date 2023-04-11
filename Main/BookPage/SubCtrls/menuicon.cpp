#include "menuicon.h"
#include "Kit/LogInfo/clog.h"

MenuIcon::MenuIcon(QString str,QWidget *parent) : QWidget(parent)
{
    //this->setStyleSheet(str);
    this->setStyleSheet("{background-color: rgb(0, 255, 255);}");
    LOG("MwnuIcon build!");
}
