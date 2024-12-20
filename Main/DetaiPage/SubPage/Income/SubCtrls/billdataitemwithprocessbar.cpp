#include "billdataitemwithprocessbar.h"
#include "ui_billdataitemwithprocessbar.h"
#include "Kit/LogInfo/clog.h"
#include <QDate>
#include <QTouchEvent>
#include <QImageReader>

BillDataItemWithProcessBar::BillDataItemWithProcessBar(BillTableStruct billinfo,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BillDataItemWithProcessBar),m_BillItemInfo(billinfo)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    this->installEventFilter(this);
    this->setTypeText(m_BillItemInfo.PayType);
    this->setMoneynum(m_BillItemInfo.moneyAmount);
    this->setDate(m_BillItemInfo.date);
    this->setTypeIcon(m_BillItemInfo.IconPath);
    this->show();
}
bool BillDataItemWithProcessBar::eventFilter(QObject *obj, QEvent *event)
{

      if(obj == this){
          if(event->type() == QEvent::MouseButtonPress){
              //设置Title部分样式
              ui->label_TypeText->setStyleSheet("background-color: rgba(140,140,140,50);color: rgb(0, 0, 0);font-size:14pt;border:none;");
              ui->label_Percent->setStyleSheet("background-color: rgba(140,140,140,50);color: rgb(0, 0, 0);font-size:13pt;border:none;");
              ui->label_Moneynum->setStyleSheet("background-color: rgba(140,140,140,50);color: rgb(0, 0, 0);font-size:15pt;border:none;");
              ui->progressBar->setStyleSheet("QProgressBar::chunk\n"
              "{border-radius:7px;background-color: rgb(255, 207, 11);}\n"
              "QProgressBar\n"
              "{color:white;background:rgba(141,141,141,50);border:none;}");
              ui->label_Date->setStyleSheet("background-color: rgba(140,140,140,50);color: rgb(170, 170, 170);font-size:14pt;");
              ui->subwidget->setStyleSheet("background-color: rgba(140,140,140,50);border-bottom:1px ;border-color: rgba(140,140,140,120);border-style: solid;");
              ui->backgroundwidget->setStyleSheet("background-color:white");
              this->setStyleSheet("background-color:white");
              LOG("press!");
              LOG("m_BillItemInfo.billNo:%d",m_BillItemInfo.billNo);
              LOG("m_BillItemInfo.moneyAmount:%d",(int)m_BillItemInfo.moneyAmount);
              LOG("m_BillItemInfo.:%s",m_BillItemInfo.remarks.toStdString().c_str());
              LOG("m_BillItemInfo.date year:%d month:%d day:%d",m_BillItemInfo.date.year(),m_BillItemInfo.date.month(),m_BillItemInfo.date.day());
          }

          if(event->type() == QEvent::MouseMove){
              is_TouchScroll = true;
          }

          if(event->type() == QEvent::MouseButtonRelease){
              ui->label_TypeText->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);font-size:14pt;border:none;");
              ui->label_Percent->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);font-size:13pt;border:none;");
              ui->label_Moneynum->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);font-size:15pt;border:none;");
              ui->progressBar->setStyleSheet("QProgressBar::chunk\n"
              "{border-radius:7px;background-color: rgb(255, 207, 11);}\n"
              "QProgressBar\n"
              "{color:white;background:white;border:none;}");
              ui->label_Date->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(170, 170, 170);font-size:14pt;");
              ui->subwidget->setStyleSheet("background-color: rgb(255, 255, 255);border-bottom:1px ;border-color: rgba(140,140,140,120);border-style: solid;");
              ui->backgroundwidget->setStyleSheet("background-color: rgb(255, 255, 255);");
              this->setStyleSheet("background-color: rgb(255, 255, 255);");
              if(!is_TouchScroll)
                   getDetailBillInfo(m_BillItemInfo);         //发送调用信号
              is_TouchScroll = false;
              LOG("release!");
          }
      }
     return QWidget::eventFilter(obj, event);
}
 //设置收费支出类型文本
void BillDataItemWithProcessBar::setTypeText(QString typetext)
{
    ui->label_TypeText->setText(typetext);
}
//设置收费支出图标
void BillDataItemWithProcessBar::setTypeIcon(QString typeiconurl)
{
    //LOG("11111");
//    QPixmap pix(typeiconurl);
//    ui->label_TypeIcon->setPixmap(pix);
       QImage image(typeiconurl);
       //std::unique_ptr<QImageReader> imgRead(new QImageReader(typeiconurl));
       //image = imgRead->read();
       QPixmap pixmap = QPixmap::fromImage(image);
       ui->label_TypeIcon->setPixmap(pixmap);

    //ui->label_TypeIcon->setStyleSheet(QString("#label_TypeIcon{border-image: url(%1);}").arg(typeiconurl));
}
 //设置收费支出金额
void BillDataItemWithProcessBar::setMoneynum(double num)
{
    //m_BillItemInfo.moneyAmount = num;
    ui->label_Moneynum->setText(QString::number(num,'f',2));
}

//设置所占百分比文本
void BillDataItemWithProcessBar::setPercent(double percent)
{
    ui->label_Percent->setText(QString("%1").arg(QString::number(percent*100.0,'f',1))+"%");
    setProgressBarvalue(int(percent*100));
}
//设置进度条value
void BillDataItemWithProcessBar::setProgressBarvalue(int value)
{
    ui->progressBar->setValue(value);
}
//设置日期
void BillDataItemWithProcessBar::setDate(QDate date)
{
    //m_BillItemInfo.date = date;
    ui->label_Date->setText(QString("%1年%2月%3日").arg(date.year()).arg(date.month()).arg(date.day()));
}
//隐藏日期
void BillDataItemWithProcessBar::hideDate()
{
     ui->label_Date->setText("");
}
BillDataItemWithProcessBar::~BillDataItemWithProcessBar()
{
    delete ui;
}
