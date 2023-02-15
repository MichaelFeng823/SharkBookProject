#include "billdataitem.h"
#include <QEvent>
#include "Util/clog.h"

BillDataItem::BillDataItem(InAndOutType type,QWidget *parent) : QWidget(parent)
{
    //LOG("BillDataItem()");
    currenttype = type;
    buildUiLayout();
    buildUiStyle();
    titleWidget->installEventFilter(this);
    detailWidget->installEventFilter(this);
    this->setDate(QDate::currentDate());
    this->setTitlePayNum(-200.5);
    this->setPayType("餐饮");
    this->setDetialPayNum(-200.5);
    this->setPayDetailSoucres("和XXX一起在XXX店吃的晚饭!");
}
//构建界面布局
void BillDataItem::buildUiLayout()
{
    //构建Title部分布局
    m_labelPayDate = new QLabel();
    m_labelPayNum = new QLabel();
    titlelayout.addWidget(m_labelPayDate,0,0);
    titlelayout.addWidget(m_labelPayNum,0,1);

    titlelayout.setSpacing(0);
    titlelayout.setMargin(0);
    titlelayout.setColumnStretch(0,6);
    titlelayout.setColumnStretch(1,4);

    titleWidget = new QWidget();
    titleWidget->setLayout(&titlelayout);

    //构建下面部分布局
    IconLabel = new QLabel();
    labelPayType = new QLabel();
    labelPayDetailSources = new QLabel();
    PaynumLabel = new QLabel();
    detailelayout.addWidget(IconLabel,0,0);
    detailelayout.addWidget(labelPayType,0,1);
    detailelayout.addWidget(labelPayDetailSources,0,2);
    detailelayout.addWidget(PaynumLabel,0,3);

    detailelayout.setSpacing(0);
    detailelayout.setMargin(0);
    detailelayout.setColumnStretch(0,2);
    detailelayout.setColumnStretch(1,2);
    detailelayout.setColumnStretch(2,5);
    detailelayout.setColumnStretch(3,3);

    labelPayType->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    detailWidget = new QWidget();
    detailWidget->setLayout(&detailelayout);

    //构建外层总布局
    outterLayout.addWidget(titleWidget,0,0);
    outterLayout.addWidget(detailWidget,1,0);
    outterLayout.setSpacing(0);
    outterLayout.setMargin(0);
    outterLayout.setRowStretch(0,4);
    outterLayout.setRowStretch(1,6);

    //构建完成
    this->setLayout(&outterLayout);
}
//构建界面样式
void BillDataItem::buildUiStyle()
{
    //设置Title部分样式
    m_labelPayDate->setStyleSheet("color:rgb(140,140,140);font-size:12pt;");
    m_labelPayNum->setStyleSheet("color:rgb(140,140,140);font-size:12pt;padding-right:30px");
    titleWidget->setStyleSheet("QWidget{background:rgb(255,255,255);border-bottom:1px ;border-color: rgba(140,140,140,120);border-style: solid;}");
    //设置下面部分样式
    IconLabel->setStyleSheet("border-image: url(:/PayTypeIcon/image/EatTypeIcon.jpg);");
    labelPayType->setStyleSheet("color:rgb(30,30,30);font-size:15pt");
    labelPayDetailSources->setStyleSheet("color:rgb(30,30,30);font-size:12pt");
    PaynumLabel->setStyleSheet("color:rgb(30,30,30);padding-right:30px;font-size:15pt;");
    //设置外层部分样式
    this->setStyleSheet("background:rgb(255,255,255);");
}
//设置日期
void BillDataItem::setDate(QDate date)
{
    int dayindex = date.dayOfWeek();
    dayindex == 7 ? dayindex = 0:dayindex = dayindex;
    strPayDate = QString("%1月%2日 星期%3").arg(date.month()).arg(date.day()).arg(dayOfWeak[dayindex]);
    m_labelPayDate->setText(strPayDate);
}
//设置title支付数目
void BillDataItem::setTitlePayNum(double num)
{
    if(currenttype == InAndOutType::OutType){
        num = num * -1;
        strTitlePayNum = QString("支出: %1").arg(QString::number(num));
    }
    else{
        strTitlePayNum = QString("收入: %1").arg(QString::number(num));
    }
    m_labelPayNum->setText(strTitlePayNum);
    m_labelPayNum->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
}
//设置支付类型
void BillDataItem::setPayType(QString strType)
{
    strPayType = strType;
    labelPayType->setText(strPayType);
}
//设置图标
void BillDataItem::setIcon(QString strIcon)
{
    strIconstyle = strIcon;
    //IconButton.setIcon()
    QString stylesheet = QString("QLabel{border-iamge:url(%1);}").arg(strIconstyle);
    IconLabel->setStyleSheet(stylesheet);
}
void BillDataItem::setDetialPayNum(double num)
{
    strPayNum = QString::number(num);
    PaynumLabel->setText(strPayNum);
    PaynumLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
}
void BillDataItem::setPayDetailSoucres(QString strSources)
{
    strPayDetailSources = strSources;
    labelPayDetailSources->setText(strPayDetailSources);
}
bool BillDataItem::eventFilter(QObject *obj, QEvent *event)  //事件过滤器
{
    if(obj == titleWidget)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            LOG("titleWidget was pressed!");
            titleWidget->setStyleSheet("QWidget{background:rgba(140,140,140,50);border-bottom:1px rgb(140,140,140);border-color: gray;border-style: solid;}");
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            titleWidget->setStyleSheet("QWidget{background:rgb(255,255,255);border-bottom:1px rgb(140,140,140);border-color: gray;border-style: solid;}");
        }
    }
    else if(obj == detailWidget)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            LOG("detailWidget was pressed!");
            detailWidget->setStyleSheet("background:rgba(140,140,140,50);");
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            detailWidget->setStyleSheet("background:rgb(255,255,255);");
        }
    }
    return QWidget::eventFilter(obj, event);
}
//析构函数
BillDataItem::~BillDataItem()
{
    //LOG("~BillDataItem()");
}

