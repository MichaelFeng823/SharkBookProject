#include "chartmodel.h"
#include "Controler/PublicApi/PublicDbFunc.h"
using namespace  ScreenFunc;

ChartModel::ChartModel(ChartSelectType type,QWidget * parent):m_CurrentType(type)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    initMonthMap();

}
//设置 周数 / 月数 / 年数
void ChartModel::setId(int id)
{
    switch (m_CurrentType){
        case ChartSelectType::week :{
            m_CurrentWeekNum = id;
        }
        case ChartSelectType::month : {
            m_CurrentMonthNum = id;
            monthDataEndIndex = monthMap[m_CurrentMonthNum];
            if(m_CurrentMonthNum == QDate::currentDate().month()){
                monthDataEndIndex = QDate::currentDate().daysInMonth();
            }
        }
        case ChartSelectType::year : {
            m_CurrentYearNum = id;
            if(QDate::currentDate().year() == m_CurrentYearNum)
                yearDataEndIndex = QDate::currentDate().month();
        }
    }
}
//重写绘画事件
void ChartModel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.translate(0,0);
    //painter.scale(getScreenSize().width()/width(),getScreenSize().width()/width());
    drawTextNum(&painter);
    drawLine(&painter);
}
//画文字和数字
void ChartModel::drawTextNum(QPainter * painter)
{
    QFont ft;
    ft.setFamily("Microsoft YaHei");                //文本字体
    ft.setPixelSize(34);
    painter->setFont(ft);
    painter->setPen(QPen(QColor(0,0,0)));
    painter->save();
    QString str1 = QString("总支出:%1").arg(m_TotalExpand);
    QString str2 = QString("平均值:%2").arg(m_Average);
    painter->drawText(QRect(20,20,width(),height() / 8),Qt::AlignLeft,str1);
    painter->drawText(QRect(20,20 + height() / 8,width(),height() / 8),Qt::AlignLeft,str2);
    painter->restore();
}
//画线
void ChartModel::drawLine(QPainter * painter)
{
    topline.setP1(QPoint(20,20 + height() / 8*2));
    topline.setP2(QPoint(width()-20,20 + height() / 8*2));
    bottomLine.setP1(QPoint(20,height()-20));
    bottomLine.setP2(QPoint(width()-20,height()-20));

    painter->setPen(QPen(QColor(0,0,0)));
    painter->drawLine(topline);
    painter->drawLine(bottomLine);
}
//初始化月份Map容器
void ChartModel::initMonthMap()
{
    monthMap[1] = monthMap[3] = monthMap[5] = monthMap[7] = monthMap[8] = monthMap[10] = monthMap[12] = 31;

    monthMap[2] = 28;

    monthMap[4] = monthMap[6] = monthMap[9] = monthMap[11] = 30;
}
