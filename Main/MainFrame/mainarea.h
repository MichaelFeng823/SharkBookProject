#ifndef MAINAREA_H
#define MAINAREA_H
#include <QWidget>
#include "Main/MinePage/MainUi/minewidagetinstackwidget.h"
#include "Main/DetaiPage/MainUi/detailedsonpageinstackwidget.h"
#include "Main/BookPage/MainUi/bookkeepingselectarea.h"
#include "Controler/DataStruct/PublicDataStruct.h"
#include "Main/ChartPage/MainUi/chartmaininterface.h"
#include "Main/DiscoveryPage/MainUi/discoverymaininterface.h"

namespace Ui {
class MainArea;
}

class MainArea : public QWidget
{
    Q_OBJECT

public:
    explicit MainArea(QWidget *parent = nullptr);
    ~MainArea();

private:
    void  setcontent(QString str);

    void  BuildStackWidgetContent();

    void initThis();

    void setButtonClicked(int btnindex);

public slots:
    void onFirstpageButtonClicked();

    void onSecondpageButtonClicked();

    void onThridpageButtonClicked();

    void onFourpageButtonClicked();

    void onBookkeepButtonClicked();

    void onReceiveUpdateBillSignal(bool status);

    void onReceiveUpdateDetailBiilPageSignal(bool status);

    void receiveSignalStr(QString str);

signals:
    void sendSignalToApppagemanager(QString);

private:
     DetailedSonPageInStackwidget * detailesonpageinstackwidget;
     ChartMainInterface * chartmaininterface;
     DiscoveryMainInterface * discoverymaininterface;
     MinewidagetInStackWidget * minewidget;
     BookkeepingSelectArea * bookkeepingselectarea = nullptr;
     QVector<QPushButton*> m_ButtomButtonList;

    Ui::MainArea *ui;
};

#endif // MAINAREA_H
