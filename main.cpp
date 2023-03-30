#include <QApplication>
#include "Controler/apppagemanager.h"
#include <QGridLayout>
#include "Kit/billdataitem.h"
#include "Kit/chinese2pinyin.h"
#include "Util/clog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 1
    //QString input("太阳当空照");
    //LOG("%s",Chinese2Pinyin::getFullChars(input).toStdString().c_str());
    //多音字,暂不支持
    //LOG("%s",Chinese2Pinyin::getCamelChars(input).at(0).toStdString().c_str());
    return AppPageManager::instance()->initPage();
#else
   //实例化一些子部件自定义控件
#endif

}
