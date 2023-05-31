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
            getDateByWeekNum();
        }
        case ChartSelectType::month : {
            m_CurrentMonthNum = id;
            monthDataEndIndex = monthMap[m_CurrentMonthNum];
//            if(m_CurrentMonthNum == QDate::currentDate().month()){
//                monthDataEndIndex = QDate::currentDate().daysInMonth();
//            }
            getdateByMonthNum();
        }
        case ChartSelectType::year : {
            m_CurrentYearNum = id;
            if(QDate::currentDate().year() == m_CurrentYearNum)
                yearDataEndIndex = QDate::currentDate().month();
        }
    }
    update();
}
//获取日期通过周数
void ChartModel::getDateByWeekNum()
{
    //1.先确定当前年的1月1日是周几 如果 大于 周一 则为去年的周数
    QDate date = QDate::currentDate();
    date.setDate(date.year(),1,1);
    if(date.weekNumber() != 1){
        date = date.addDays(7-date.dayOfWeek());
    }
    date = date.addDays((m_CurrentWeekNum - 1) * 7);
    for(int i = 0 ; i < 7; i++){
        m_WeekCoordinate[i] = date.addDays(i+1).toString("MM-dd");
    }
}
//获取日期通过月数
void ChartModel::getdateByMonthNum()
{
     m_MonthCoordinate.clear();
     if(monthDataEndIndex == 28){
         m_MonthCoordinate = {"01","05","09","13","16","20","24","28"};
     }
     else if(monthDataEndIndex == 30)
     {
         m_MonthCoordinate = {"01","05","09","13","18","22","26","30"};
     }
     else if(monthDataEndIndex == 31)
     {
         m_MonthCoordinate = {"01","05","10","14","18","22","27","31"};
     }
}
//重写绘画事件
void ChartModel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.translate(0,0);
    painter.scale(1,1);
    drawTextNum(&painter);
    drawLine(&painter);
    drawCoordinateAxis(&painter);
}
//画文字和数字
void ChartModel::drawTextNum(QPainter * painter)
{
    QFont ft;
    ft.setFamily("Microsoft YaHei");                //文本字体
    ft.setPixelSize(32);
    painter->setFont(ft);
    painter->setPen(QPen(QColor(114,114,114)));
    painter->save();
    QString str1 = QString("总支出:%1").arg(m_TotalExpand);
    QString str2 = QString("平均值:%2").arg(m_Average);
    painter->drawText(30,height()/12,str1);
    painter->drawText(30,height()/12*2.2,str2);
    painter->restore();
}
//画线
void ChartModel::drawLine(QPainter * painter)
{
    topline.setP1(QPoint(30,20 + height() / 8*2));
    topline.setP2(QPoint(width()-30,20 + height() / 8*2));
    bottomLine.setP1(QPoint(30,height()-55));
    bottomLine.setP2(QPoint(width()-30,height()-55));

    painter->setPen(QPen(QColor(0,0,0)));
    painter->save();
    painter->drawLine(topline);
    painter->drawLine(bottomLine);
    painter->restore();
}
//画坐标轴
void ChartModel::drawCoordinateAxis(QPainter * painter)
{
    switch (m_CurrentType){
        case ChartSelectType::week :{
           drawWeekCoordinateAxis(painter);
           break;
        }
        case ChartSelectType::month : {
            drawMonthCoordinateAxis(painter);
            break;
        }
        case ChartSelectType::year : {
            drawYearCoordinateAxis(painter);
            break;
        }
    }
}
//画周坐标轴
void ChartModel::drawWeekCoordinateAxis(QPainter * painter)
{
    QFont font;
    font.setPixelSize(20);
    int length = (width()) / 7;
    painter->setPen(QColor(134,134,134));
    painter->save();
    for(int i = 0; i < 7; i++)
    {
        painter->drawText(30 + i*length,height()-20,m_WeekCoordinate[i]);
    }
    painter->restore();
}
//画月坐标轴
void ChartModel::drawMonthCoordinateAxis(QPainter * painter)
{
    QFont font;
    font.setPixelSize(20);
    int length = (width() - 60) / 7;
    painter->setPen(QColor(134,134,134));
    painter->save();
    for(int i = 0; i < m_MonthCoordinate.size(); i++)
    {
        painter->drawText(20 + i*length,height()-20,m_MonthCoordinate[i]);
    }
    painter->restore();
}
//画年坐标轴
void ChartModel::drawYearCoordinateAxis(QPainter * painter)
{
    QFont font;
    font.setPixelSize(20);
    int length = (width()) / 12;
    painter->setPen(QColor(134,134,134));
    painter->save();
    for(int i = 0; i < 12; i++)
    {
        painter->drawText(20 + i*length,height()-20,m_YearCoordinate[i]);
    }
    painter->restore();
}
//初始化月份Map容器
void ChartModel::initMonthMap()
{
    monthMap[1] = monthMap[3] = monthMap[5] = monthMap[7] = monthMap[8] = monthMap[10] = monthMap[12] = 31;
    monthMap[2] = 28;
    monthMap[4] = monthMap[6] = monthMap[9] = monthMap[11] = 30;
}
