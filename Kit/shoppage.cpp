#include "shoppage.h"
#include "ui_shoppage.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include "Net/msgqueue.h"
#include "Controler/PublicUerInfo.h"
#include "Util/clog.h"
ShopPage::ShopPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShopPage)
{
    ui->setupUi(this);
    connect(ui->pushButton_query,&QPushButton::clicked,this,&ShopPage::onQueryButtonClicked);
    connect(MSGQUEUE, SIGNAL(SignalRecvMsg(QByteArray,QObject *)), this, SLOT(onRecvMsg(QByteArray, QObject *)));
    this->raise();
    this->show();
}
ShopPage::~ShopPage()
{
    delete ui;
}
//当查询按钮被点击是的槽函数
void ShopPage::onQueryButtonClicked()
{
    if(SYSTEMSTATE::NETWORKSTATE){
        TMsgItem item;
        item.strUrl = QString("http://v.juhe.cn/calendar/day?date=%1&key=196baf2af41fd38cc2c3674c23d4cee6").arg(ui->lineEdit->text());
        item.pObj = this;
        MSGQUEUE->Push(item);
    }
    else{
        QMessageBox::warning(0,"警告","无网络使用权限");
    }
}
//当收到消息时的槽函数
void ShopPage::onRecvMsg(QByteArray data, QObject * obj)
{
    if(obj != this)
        return;
    //请求返回的结果
    QString content = parsingData(&data);
    ui->label->setText(content);
}
//解析数据
QString ShopPage::parsingData(QByteArray * data)
{
    QString content;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(*data, &jsonError);
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if (doucment.isObject()){
            QJsonObject object = doucment.object();
            QString reason = object.value("reason").toString();
            if (object.contains("result")){
                QJsonObject object_result = object.value("result").toObject();
                if(object_result.contains("data")){
                   content = mergeData(object_result.value("data").toObject());
                }
            }
        }
    }
     LOG("content:%s",content.toStdString().c_str());
     if(content.isEmpty())
         content = QString("查询结果为空");
     return content;
}
//合并数据
QString ShopPage::mergeData(QJsonObject object_data)
{
    QString content;
    if(object_data.contains("holiday")){
        LOG("contains holiday");
        QString holiday = object_data.value("holiday").toString();
        QString avoid = object_data.value("avoid").toString();
        QString animalsYear = object_data.value("animalsYear").toString();
        QString desc = object_data.value("desc").toString();
        QString weekday = object_data.value("weekday").toString();
        QString suit = object_data.value("suit").toString();
        QString lunarYear = object_data.value("lunarYear").toString();
        QString lunar = object_data.value("lunar").toString();
        QString yearmonth = object_data.value("year-month").toString();
        QString date = object_data.value("date").toString();

        content.append(QString("假日:")+holiday + "\n");
        content.append(QString("忌:")+avoid + "\n");
        content.append(QString("属相:"+animalsYear + "\n"));
        content.append(QString("假日描述:"+desc + "\n"));
        content.append(QString("周几:")+weekday + "\n");
        content.append(QString("宜:")+suit + "\n");
        content.append(QString("纪年")+lunarYear + "\n");
        content.append(QString("农历")+lunar + "\n");
        content.append(QString("年份和月份:")+yearmonth + "\n");
        content.append(QString("具体日期:")+date + "\n");
    }
    return content;
}
