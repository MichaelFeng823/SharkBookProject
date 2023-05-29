#include <QApplication>
#include "Controler/APPControl/apppagemanager.h"
#include <QGridLayout>
#include "Kit/Chinese2PinYin/chinese2pinyin.h"
#include "BaseClass/circulardiagram.h"
#include "Main/DetaiPage/SubPage/Budget/SubCtrls/budgetsubitem.h"
#include "Controler/DataStruct/PublicDataStruct.h"
#include "BaseClass/picitem.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 1
    return AppPageManager::instance()->initPage();
#else
   //实例化一些子部件自定义控件
   //QString input("太阳当空照");
   //LOG("%s",Chinese2Pinyin::getFullChars(input).toStdString().c_str());
   //多音字,暂不支持
   //LOG("%s",Chinese2Pinyin::getCamelChars(input).at(0).toStdString().c_str());
    //QWidget * w = new QWidget();
    //w->setStyleSheet("background-color:rgb(0,0,0);");
    //PicItem * demo = new PicItem();
    //demo->setStyleSheet("background-color:rgb(0,0,0);");
    //demo->setParent(w);
    //demo->move(0,0);
    //w->show();
    PhotoFrame * w = new PhotoFrame();
    w->show();



    //CircularDiagram * demo = new CircularDiagram();
    //demo->setGeometry(0,0,1920,1080);
    //demo->show();

    return QApplication::exec();
#endif

}
