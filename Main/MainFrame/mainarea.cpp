#include "mainarea.h"
#include "ui_mainarea.h"
#include "Kit/LogInfo/clog.h"


MainArea::MainArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainArea)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,false);
    initThis();
    BuildStackWidgetContent();
    onFirstpageButtonClicked();
    connect(ui->pushButton_firstpage,&QPushButton::clicked,this,&MainArea::onFirstpageButtonClicked);
    connect(ui->pushButton_secondpage,&QPushButton::clicked,this,&MainArea::onSecondpageButtonClicked);
    connect(ui->pushButton_thirdpage,&QPushButton::clicked,this,&MainArea::onThridpageButtonClicked);
    connect(ui->pushButton_fourpage,&QPushButton::clicked,this,&MainArea::onFourpageButtonClicked);
    connect(ui->BookkeepButton,&QPushButton::clicked,this,&MainArea::onBookkeepButtonClicked);
}
void MainArea::initThis()
{
    m_ButtomButtonList.clear();
    m_ButtomButtonList.append(ui->pushButton_firstpage);
    m_ButtomButtonList.append(ui->pushButton_secondpage);
    m_ButtomButtonList.append(ui->pushButton_thirdpage);
    m_ButtomButtonList.append(ui->pushButton_fourpage);
}

//构建stackwidget的UI内容布局
void  MainArea::BuildStackWidgetContent()
{
    NEW_OBJECT(detailesonpageinstackwidget,DetailedSonPageInStackwidget);
    //detailesonpageinstackwidget = new DetailedSonPageInStackwidget;

    chartmaininterface = new ChartMainInterface();

    discoverymaininterface = new DiscoveryMainInterface();

    minewidget = new MinewidagetInStackWidget();

    connect(minewidget,&MinewidagetInStackWidget::ExitLoginSignal,this,&MainArea::receiveSignalStr);

    ui->stackedWidget->addWidget(detailesonpageinstackwidget);
    ui->stackedWidget->addWidget(chartmaininterface);
    ui->stackedWidget->addWidget(discoverymaininterface);
    ui->stackedWidget->addWidget(minewidget);

    m_PageVector.append(detailesonpageinstackwidget);
    m_PageVector.append(chartmaininterface);
    m_PageVector.append(discoverymaininterface);
    m_PageVector.append(minewidget);
}
void MainArea::receiveSignalStr(QString str)
{
    LOG("MainArea Receive signalStr is %s",str.toStdString().c_str());
    emit sendSignalToApppagemanager(str);
}
void  MainArea::setcontent(QString str)
{
    //ui->label->setText(str);
}

void MainArea::onFirstpageButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(PageType::DetailPage);
    setButtonClicked(PageType::DetailPage);
    setActiveWindowState(PageType::DetailPage);
}

void MainArea::onSecondpageButtonClicked()
{
    ui->stackedWidget->setCurrentIndex((int)PageType::ChartPage);
    setButtonClicked(PageType::ChartPage);
    setActiveWindowState(PageType::ChartPage);
}
void MainArea::onThridpageButtonClicked()
{
    ui->stackedWidget->setCurrentIndex((int)PageType::Discovery);
    setButtonClicked((int)PageType::Discovery);
    setActiveWindowState(PageType::Discovery);
}
void MainArea::onFourpageButtonClicked()
{
    ui->stackedWidget->setCurrentIndex((int)PageType::MinePage);
    setButtonClicked(PageType::MinePage);
    setActiveWindowState(PageType::MinePage);
}

void MainArea::onBookkeepButtonClicked()
{
    if(bookkeepingselectarea != nullptr)
        delete  bookkeepingselectarea;
    bookkeepingselectarea = new BookkeepingSelectArea();
    connect(bookkeepingselectarea,&BookkeepingSelectArea::sendSignalToUpdateBillArea,this,&MainArea::onReceiveUpdateBillSignal);
}

void MainArea::onReceiveUpdateBillSignal(bool status)
{
    LOG("onReceiveUpdateBillSignal()");
    if(status)
    {
        detailesonpageinstackwidget->UpdateBillContent();
        bookkeepingselectarea->close();
    }
}

void MainArea::onReceiveUpdateDetailBiilPageSignal(bool status)
{
    LOG("onReceiveUpdateDetailBiilPageSignal()");
    if(status){
        detailesonpageinstackwidget->UpdateBillDetailPage();
    }

}
void MainArea::setButtonClicked(int btnindex)
{
    for(int i = 0; i < m_ButtomButtonList.size(); i++){
        if(btnindex == i){
            m_ButtomButtonList[i]->setDisabled(true);
        }
        else{
             m_ButtomButtonList[i]->setEnabled(true);
        }
    }
}

void MainArea::setActiveWindowState(PageType page)
{
    for(int index = 0; index <= PageType::MinePage; index++){
        if(m_PageVector[index] != nullptr)
              m_PageVector[index]->setActiveWindow(false);
    }
    if(m_PageVector[(int)page] != nullptr)
         m_PageVector[(int)page]->setActiveWindow(true);
}

MainArea::~MainArea()
{
    LOG("MainArea::~MainArea()");
    delete ui;
}
