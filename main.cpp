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
    //BillDataItemCopy * demo = new BillDataItemCopy();
    //delete  demo;
    //BillDataItem * demo = new BillDataItem();
    //delete  demo;
    QString input("太阳当空照");
    LOG("%s",Chinese2Pinyin::getFullChars(input).toStdString().c_str());
    //多音字,暂不支持
    LOG("%s",Chinese2Pinyin::getCamelChars(input).at(0).toStdString().c_str());

    return AppPageManager::instance()->initPage();
#else
   QWidget w;
   //CalendarKit kit;
   // kit.setParent(&w);
   // kit.show();
   // InAndOutKit kitIn(InAndOutType::InType,&w);
   // kitIn.move(200,0);
   // InAndOutKit kitout(InAndOutType::OutType,&w);
   // kitout.move(600,0);
   // BillDataItem kititem(InAndOutType::OutType,&w);
   //kititem.move(0,120);
   // DetailedSonPageInStackwidget demo;
   // BookkeepingSelectArea demo;

   //Calendar demo(CalendarType::YearMonthDay);
   BookkeepingInputKit demo;
   //QGridLayout layout;
   //layout.addWidget(&demo);
   //layout.setSpacing(0);
   //layout.setMargin(0);
   //w.setLayout(&layout);
   demo.setParent(&w);
   demo.setFixedSize(1080,850);
   demo.move(0,2400-1040);
   demo.show();
   w.show();

    return a.exec();
#endif

}
