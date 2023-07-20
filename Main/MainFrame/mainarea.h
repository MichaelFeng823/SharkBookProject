#ifndef MAINAREA_H
#define MAINAREA_H
#include <QWidget>
#include <QMap>
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
    enum PageType{DetailPage = 0,ChartPage,Discovery,MinePage};

private:
    void  setcontent(QString str);

    void  BuildStackWidgetContent();

    void initThis();

    void setButtonClicked(int btnindex);

    void setActiveWindowState(PageType page);

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
     DetailedSonPageInStackwidget * detailesonpageinstackwidget = nullptr;
     ChartMainInterface * chartmaininterface = nullptr;
     DiscoveryMainInterface * discoverymaininterface = nullptr;
     MinewidagetInStackWidget * minewidget = nullptr;
     BookkeepingSelectArea * bookkeepingselectarea = nullptr;
     QVector<QPushButton*> m_ButtomButtonList;
     QVector<BaseCustomWidget*> m_PageVector;
     Ui::MainArea *ui;
};

#endif // MAINAREA_H
