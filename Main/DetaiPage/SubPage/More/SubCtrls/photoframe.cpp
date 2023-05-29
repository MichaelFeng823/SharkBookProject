#include "photoframe.h"
#include "ui_photoframe.h"
#include <QPointer>
#include <BaseClass/picitem.h>
#include <Controler/PublicApi/PublicDbFunc.h>

using namespace ScreenFunc;

PhotoFrame::PhotoFrame(QWidget *parent) :
    BaseCustomWidget(parent),
    ui(new Ui::PhotoFrame)
{
    ui->setupUi(this);
    connect(ui->PicGraphicsView,&PicItem::SendStateToPhotoView,this,&PhotoFrame::onReceivePicItemState);
    connect(ui->pushButton_Close,&QPushButton::clicked,this,&PhotoFrame::requestClose);
    initOther();
    initTableviewPreviewList();
    m_CurrentIndex = 0;
}

PhotoFrame::~PhotoFrame()
{
    delete ui;
}
//初始化其他
void PhotoFrame::initOther()
{
    model = new QStandardItemModel();
    ui->tableViewPreview->setModel(model);
    autoDisplayTimer = new QTimer(this);
    connect(autoDisplayTimer,&QTimer::timeout,this,&PhotoFrame::onAutoDisplayStart);
    connect(ui->autoDisplayButton,&QPushButton::clicked,this,&PhotoFrame::onAutoDisplayClicked);
    connect(ui->ManualButton,&QPushButton::clicked,this,&PhotoFrame::onManualDisplayClicked);
}
//初始化图片预览列表
void PhotoFrame::initTableviewPreviewList()
{
    model->setColumnCount(colcounts);
    model->setRowCount(1);
    ui->tableViewPreview->setRowHeight(0,getScreenSize().height()/13*2);

    for(int i = 0; i < 10; i++){
       ui->tableViewPreview->setColumnWidth(i,getScreenSize().height()/13*2*1.7*0.8);
       QModelIndex index = model->index(0,i);
       QPointer<QPushButton> item = new QPushButton();
       item->resize(getScreenSize().height()/13*2*1.7*0.7,getScreenSize().height()/13*2*0.7);
       QImage pic;
       pic.load(QString(":/girls/image/girls/%1.jpg").arg(i+1));
       connect(item,&QPushButton::clicked,this,&PhotoFrame::onReceiveChangePicRequest);
       item->setIcon(QIcon(QPixmap::fromImage(pic)));
       item->setProperty("PicData",QVariant::fromValue(pic));
       item->setIconSize(QSize(item->size()));
       ui->tableViewPreview->setIndexWidget(index,item);
    }
     //model->setColumnCount(colcounts+1);

}
//设置图片容器引用
void PhotoFrame::setPicVector(QVector<QByteArray> * vector)
{
    PicVector = vector;
}
 //添加图片
void PhotoFrame::addPic()
{
    LOG("添加图片");
    if(!PicVector->isEmpty())
    {
        LOG("当前容器中还有%d张新图片",PicVector->count());
        addPicItem(PicVector->front());
        PicVector->pop_front();
    }
    if(colcounts > 50){
        LOG("popPic");
        popPic();
    }
}
//添加item
void PhotoFrame::addPicItem(QByteArray data)
{
    LOG("colcounts:%d",colcounts);
    model->insertColumns(colcounts,1);
    ui->tableViewPreview->setColumnWidth(colcounts,getScreenSize().height()/13*2*1.7*0.8);
    ui->tableViewPreview->setModel(model);
    QModelIndex index = model->index(0,colcounts);
    QPointer<QPushButton> item = new QPushButton();
    item->resize(getScreenSize().height()/13*2*1.7*0.7,getScreenSize().height()/13*2*0.7);
    QImage pic;
    pic.loadFromData(data);
    connect(item,&QPushButton::clicked,this,&PhotoFrame::onReceiveChangePicRequest);
    item->setIcon(QIcon(QPixmap::fromImage(pic)));
    item->setProperty("PicData",QVariant::fromValue(pic));
    item->setIconSize(QSize(item->size()));
    ui->tableViewPreview->setIndexWidget(index,item);
    colcounts += 1;
}

//弹出图片
void PhotoFrame::popPic()
{
    if(model->columnCount() > 1){
        model->removeColumns(0,1);
        colcounts-=1;
    }
    //QModelIndex index = model->index(0,0);
    //QWidget * indexwidget = ui->tableView->indexWidget(index);
}
//自动放映槽函数
void PhotoFrame::onAutoDisplayClicked()
{
    if(!autoDisplayTimer->isActive()){
        autoDisplayTimer->start();
        autoDisplayTimer->setInterval(3000);
        ui->buttomwidget->setVisible(false);
        m_CurrentIsAuto = true;
    }
}
//手动放映槽函数
void PhotoFrame::onManualDisplayClicked()
{
    if(autoDisplayTimer->isActive())
        autoDisplayTimer->stop();
    ui->buttomwidget->setVisible(true);
    m_CurrentIsAuto = false;
}
void PhotoFrame::onAutoDisplayStart()
{
    LOG("isActiveWindow = %d",isActiveWindow());
    if(!isActiveWindow())
        return;
    m_CurrentIndex = ui->tableViewPreview->currentIndex().column();
    ++m_CurrentIndex;
    QModelIndex index = model->index(0,m_CurrentIndex);
    if(index.isValid()){
        ui->tableViewPreview->setCurrentIndex(index);
        static_cast<QPushButton*>(ui->tableViewPreview->indexWidget(index))->clicked();
    }
    if(colcounts - m_CurrentIndex < 3)
    {
        LOG("请求图片");
        addPic();
    }
    if(m_CurrentIndex == colcounts){
        popPic();
        LOG("m_CurrentIndex:%d",m_CurrentIndex);
        LOG("colcounts:%d",colcounts);
        m_CurrentIndex = 0;
        index = model->index(0,m_CurrentIndex);
        ui->tableViewPreview->setCurrentIndex(index);
        static_cast<QPushButton*>(ui->tableViewPreview->indexWidget(index))->clicked();
    }
}
//当收到改变图片请求
void PhotoFrame::onReceiveChangePicRequest()
{
    m_CurrentIndex = ui->tableViewPreview->currentIndex().column();
    QPushButton * button = static_cast<QPushButton*>(sender());
    QVariant var = button->property("PicData");
    ui->PicGraphicsView->SetImage(var.value<QImage>());
    ui->PicGraphicsView->setPosAndSize(0,0,getScreenSize().width(),ui->PicGraphicsView->size().height());
    if(colcounts - m_CurrentIndex < 10)
        addPic();
}
//当收到PicItemState时
void PhotoFrame::onReceivePicItemState(bool state)
{
    if(state && m_CurrentIsAuto){
        if(autoDisplayTimer->isActive()){
            autoDisplayTimer->stop();
        }
    }
    else if(!state && m_CurrentIsAuto)
    {
        if(!autoDisplayTimer->isActive()){
            autoDisplayTimer->start(3000);
            m_CurrentIsAuto = true;
        }
    }
}
