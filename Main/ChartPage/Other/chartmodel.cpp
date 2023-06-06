#include "chartmodel.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include <QResizeEvent>


using namespace  ScreenFunc;

ChartModel::ChartModel(ChartSelectType type,QWidget * parent):m_CurrentType(type)
{
    Q_UNUSED(parent);
    this->setAttribute(Qt::WA_TranslucentBackground);
    initMonthMap();
    initOther();
    update();
    this->installEventFilter(this);
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
            getdateByMonthNum();
        }
        case ChartSelectType::year : {
            m_CurrentYearNum = id;
        }
    }
    update();
}
//设置账单数据
void ChartModel::setBillInfo(QVector<BillTableStruct> info)
{
    billinfo.clear();
    billinfo = info;
    switch (m_CurrentType){
        case ChartSelectType::week :{
            calculateWeekBill();
            break;
        }
        case ChartSelectType::month : {
            calculateMonthBill();
            break;
        }
        case ChartSelectType::year : {
            calculateYearBill();
            break;
        }
    }
}
//刷新加载数据
void ChartModel::updateLoadData()
{
    switch (m_CurrentType){
        case ChartSelectType::week :{
            calculateWeekBill();
            break;
        }
        case ChartSelectType::month : {
            calculateMonthBill();
            break;
        }
        case ChartSelectType::year : {
            calculateYearBill();
            break;
        }
    }
}
//计算周账单
void ChartModel::calculateWeekBill()
{
    bindWeekData();                  //1.绑定筛选数据
    m_Max = getWeekMax();            //2.求出数据的MAX
    calculateWeekDotPosition();      //3.计算数据点对应的pos
    calculateWeekTotalAndAverage();  //4.计算总数和平均值
    update();
    sortWeekDotsTopThree();          //5.计算升序排序每个数据点中金额前三的数据
}
//计算月账单
void ChartModel::calculateMonthBill()
{
    bindMonthData();
    m_Max = getMonthMax();
    calculateMonthDotPosition();
    calculateMonthTotalAndAverage();
    update();
    sortMonthDotsTopThree();
}
//计算年账单
void ChartModel::calculateYearBill()
{
    bindYearData();
    m_Max = getYearMax();
    calculateYearDotPosition();
    calculateYearTotalAndAverage();
    update();
    sortYearDotsTopThree();
}
//绑定周数据
void ChartModel::bindWeekData()
{
    isHaveData = false;
    for(int i = 0; i < 7; i++){
        //1.先确定好每个点对应的数据结构中的id和日期
        week_dots[i].id = i+1;
        week_dots[i].date = m_WeekDate[i];
        m_WeekData[i] = 0;
        week_dots[i].isnotEmpty = false;
        week_dots[i].message.clear();
        for(int j = 0; j < billinfo.size(); j++){
            if(billinfo[j].date == m_WeekDate[i] && m_type == billinfo[j].InOrOut){
                isHaveData = true;
                //2.算出每天的金额总数
                m_WeekData[i] += billinfo[j].moneyAmount;
                week_dots[i].isnotEmpty = true;
                MaxThreeMessage message;
                message.date = billinfo[j].date;
                message.Amount = billinfo[j].moneyAmount;
                message.expandorincomeType = billinfo[j].typeId;
                message.PayType = billinfo[j].PayType;
                //3.将数据添加到对应点的消息集合中
                week_dots[i].message.append(message);
            }
        }
    }
}
//绑定月数据
void ChartModel::bindMonthData()
{
    isHaveData = false;
    month_dots.clear();
    int count = 0;
    for(int i = 0; i < monthDataEndIndex; i++){
        //1.先确定好每个点对应的数据结构中的id和日期
        DotData data;
        data.id = i+1;
        m_MonthData[i] = 0;
        for(int j = 0; j < billinfo.size(); j++){
            if(billinfo[j].date.year() == QDate::currentDate().year() && billinfo[j].date.month() == m_CurrentMonthNum
                    && m_type == billinfo[j].InOrOut && billinfo[j].date.day() == i+1){
                count++;
                isHaveData = true;
                //2.算出每天的金额总数
                m_MonthData[i] += billinfo[j].moneyAmount;
                data.isnotEmpty = true;
                MaxThreeMessage message;
                message.date = billinfo[j].date;
                message.Amount = billinfo[j].moneyAmount;
                message.expandorincomeType = billinfo[j].typeId;
                message.PayType = billinfo[j].PayType;
                //3.将数据添加到对应点的消息集合中
                data.message.append(message);
            }
        }
        month_dots.append(data);
    }
    LOG("count:%d -------------------------------------------------",count);
}
//绑定年数据
void ChartModel::bindYearData()
{
    isHaveData = false;
    for(int i = 0; i < 12; i++){
        //1.先确定好每个点对应的数据结构中的id和日期
        year_dots[i].id = i+1;
        m_YearData[i] = 0;
        year_dots[i].isnotEmpty = false;
        year_dots[i].message.clear();
        for(int j = 0; j < billinfo.size(); j++){
            if(billinfo[j].date.year() == m_CurrentYearNum && billinfo[j].date.month() == i+1
                    && m_type == billinfo[j].InOrOut){
                isHaveData = true;
                //2.算出每月的金额总数
                m_YearData[i] += billinfo[j].moneyAmount;
                year_dots[i].isnotEmpty = true;
                MaxThreeMessage message;
                message.date = billinfo[j].date;
                message.Amount = billinfo[j].moneyAmount;
                message.expandorincomeType = billinfo[j].typeId;
                message.PayType = billinfo[j].PayType;
                //3.将数据添加到对应点的消息集合中
                year_dots[i].message.append(message);
            }
        }
    }
}
//获取周Max
float ChartModel::getWeekMax()
{
    int Max = 1;
    for(int i = 0; i < 7; i++){
        if(m_WeekData[i] > Max)
            Max = m_WeekData[i];
    }
    //4.获取周数据的Max
    return Max;
}
//获取月Max
float ChartModel::getMonthMax()
{
    int Max = 1;
    for(int i = 0; i < monthDataEndIndex; i++){
        if(m_MonthData[i] > Max)
            Max = m_MonthData[i];
    }
    return Max;
}
//获取年Max
float ChartModel::getYearMax()
{
    int Max = 1;
    for(int i = 0; i < 12; i++){
        if(m_YearData[i] > Max)
            Max = m_YearData[i];
    }
    return Max;
}
//计算周数据点的坐标
void ChartModel::calculateWeekDotPosition()
{
    int length = (width()) / 7;
    for(int i = 0; i < 7; i++){
        week_dots[i].x = 30+40+ i*(length);
        week_dots[i].y = m_BottomPos - (m_height  * m_WeekData[i] / m_Max);
        LOG("m_height:%d",m_height);
        LOG("m_WeekData[i]:%s",QString::number(m_WeekData[i]).toStdString().c_str());
        LOG("m_Max:%s",QString::number(m_Max).toStdString().c_str());
    }
}
//计算月数据点的坐标
void ChartModel::calculateMonthDotPosition()
{
    int length = (width()-60) / monthDataEndIndex+1;
    for(int i = 0; i < monthDataEndIndex; i++){
        month_dots[i].x = 30+14+ i*length;
        month_dots[i].y = m_BottomPos - m_height  * m_MonthData[i] / m_Max;
        //month_dots[i].y = m_BottomPos;
    }
}
//计算年数据点的坐标
void ChartModel::calculateYearDotPosition()
{
    int length = (width()) / 12;
    for(int i = 0; i < 12; i++){
        year_dots[i].x = 30+20+i*length;
        year_dots[i].y = m_BottomPos - m_height  * m_YearData[i] / m_Max;
        //year_dots[i].y = m_BottomPos;
    }
}
//计算周总数和平均值
void ChartModel::calculateWeekTotalAndAverage()
{
    m_Total = 0;
    for(int i = 0; i < 7; i++){
        m_Total += m_WeekData[i];
    }
    m_Average = m_Total / 7;
}
//计算月总数和平均值
void ChartModel::calculateMonthTotalAndAverage()
{
     m_Total = 0;
    for(int i = 0; i < monthDataEndIndex; i++){
        m_Total += m_MonthData[i];
    }
    m_Average = m_Total / monthDataEndIndex;
}
//计算年总数和平均值
void ChartModel::calculateYearTotalAndAverage()
{
    m_Total = 0;
    for(int i = 0; i < 12; i++){
        m_Total += m_YearData[i];
    }
    m_Average = m_Total / 12;
}
//排序每日周数据前三
void ChartModel::sortWeekDotsTopThree()
{
    for(int i = 0;i < 7; i++)
    {
        sortMaxThreeMessage(week_dots[i]);
    }

}
//排序每日月数据前三
void ChartModel::sortMonthDotsTopThree()
{
    for(int i = 0;i < month_dots.size(); i++)
    {
        sortMaxThreeMessage(month_dots[i]);
    }
}
//排序每日年数据前三
void ChartModel::sortYearDotsTopThree()
{
    for(int i = 0;i < 12; i++)
    {
        sortMaxThreeMessage(year_dots[i]);
    }
}
//将每个数据点中的消费数据按金额排升序
void ChartModel::sortMaxThreeMessage(DotData & dot)
{
    std::sort(dot.message.begin(),dot.message.end(),ChartModel::customsort);
}
//自定义排序
bool ChartModel::customsort(MaxThreeMessage previous,MaxThreeMessage  next)
{
    return previous.Amount > next.Amount;
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
        m_WeekDate[i] = date.addDays(i+1);
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
//
void ChartModel::resizeEvent(QResizeEvent *event)
{
    m_TopPos = event->size().height() / 8 * 2 + 20;
    m_BottomPos = event->size().height() - 55;
    m_height = m_BottomPos - m_TopPos;
}
//重写绘画事件
void ChartModel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing,true);
    painter.translate(0,0);
    //painter.scale(1,1);
    drawTextNum(&painter);
    drawLine(&painter);
    drawCoordinateAxis(&painter);
    drawDataLines(&painter);
    drawDataDot(&painter);
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
    QString type;
    m_type == InOrOut::Expand ? type = "总支出": type = "总收入";
    QString str1 = QString("%1: %2").arg(type).arg(QString::number(m_Total,'f',2));
    QString str2 = QString("平均值: %1").arg(QString::number(m_Average,'f',2));
    QString str3 = QString::number(m_Max,'f',2);
    painter->drawText(30,height()/12,str1);
    painter->drawText(30,height()/12*2.2,str2);
    if(isHaveData)
        painter->drawText(width()-140,m_TopPos-20,str3);
    painter->restore();
}
//画线
void ChartModel::drawLine(QPainter * painter)
{
    topline.setP1(QPoint(30,m_TopPos));
    topline.setP2(QPoint(width()-30,m_TopPos));
    bottomLine.setP1(QPoint(30,m_BottomPos));
    bottomLine.setP2(QPoint(width()-30,m_BottomPos));

    painter->setPen(QPen(QColor(0,0,0)));
    painter->save();
    if(isHaveData)
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
        painter->drawText(30 + i*length,height()-15,m_WeekCoordinate[i]);
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
        painter->drawText(20 + i*length,height()-15,m_MonthCoordinate[i]);
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
        painter->drawText(20 + i*length,height()-15,m_YearCoordinate[i]);
    }
    painter->restore();
}
//画数据点
void ChartModel::drawDataDot(QPainter * painter)
{
    switch (m_CurrentType){
        case ChartSelectType::week :{
           drawWeekDataDot(painter);
           break;
        }
        case ChartSelectType::month : {
            drawMonthDataDot(painter);
            break;
        }
        case ChartSelectType::year : {
            drawYearDataDot(painter);
            break;
        }
    }
}
//画周数据点
void ChartModel::drawWeekDataDot(QPainter * painter)
{
    QPen pen(QColor(0,0,0));
    pen.setWidth(1);
    painter->setPen(pen);
    int brushindex = 0;
    for(int i = 0; i < 7; i++)
    {
        week_dots[i].isnotEmpty == false ? brushindex = 0 : brushindex = 1;
        painter->setBrush(brushGroup[brushindex]);
        painter->save();
        painter->drawEllipse(QPoint(week_dots[i].x,week_dots[i].y),radius,radius);
        painter->restore();
    }
}
//画月数据点
void ChartModel::drawMonthDataDot(QPainter * painter)
{
    QPen pen(QColor(0,0,0));
    pen.setWidth(1);
    painter->setPen(pen);
    int brushindex = 0;
    for(int i = 0; i < monthDataEndIndex; i++)
    {
        month_dots[i].isnotEmpty == false ? brushindex = 0 : brushindex = 1;
        painter->setBrush(brushGroup[brushindex]);
        painter->save();
        painter->drawEllipse(QPoint(month_dots[i].x,month_dots[i].y),radius,radius);
        painter->restore();
    }
}
//画年数据点
void ChartModel::drawYearDataDot(QPainter * painter)
{
    QPen pen(QColor(0,0,0));
    pen.setWidth(1);
    painter->setPen(pen);
    int brushindex = 0;
    for(int i = 0; i < 12; i++)
    {
        year_dots[i].isnotEmpty == false ? brushindex = 0 : brushindex = 1;
        painter->setBrush(brushGroup[brushindex]);
        painter->save();
        painter->drawEllipse(QPoint(year_dots[i].x,year_dots[i].y),radius,radius);
        painter->restore();
    }
}
//画数据   折线
void ChartModel::drawDataLines(QPainter * painter)
{
    switch (m_CurrentType){
        case ChartSelectType::week :{
           drawWeekDataLines(painter);
           break;
        }
        case ChartSelectType::month : {
            drawMonthDataLines(painter);
            break;
        }
        case ChartSelectType::year : {
            drawYearDataLines(painter);
            break;
        }
    }
}
//画周数据 折线
void ChartModel::drawWeekDataLines(QPainter * painter)
{
    QPen pen(QColor(0,0,0));
    pen.setWidth(1);
    painter->setPen(pen);
    QVector<QLine> lines;
    for(int i = 0; i+1 < 7; i++)
    {
        QLine line;
        line.setP1(QPoint(week_dots[i].x,week_dots[i].y));
        line.setP2(QPoint(week_dots[i+1].x,week_dots[i+1].y));
        lines.append(line);
    }
    painter->drawLines(lines);
}
//画月数据 折线
void ChartModel::drawMonthDataLines(QPainter * painter)
{
    QPen pen(QColor(0,0,0));
    pen.setWidth(1);
    painter->setPen(pen);
    QVector<QLine> lines;
    for(int i = 0; i+1 < monthDataEndIndex; i++)
    {
        QLine line;
        line.setP1(QPoint(month_dots[i].x,month_dots[i].y));
        line.setP2(QPoint(month_dots[i+1].x,month_dots[i+1].y));
        lines.append(line);
    }
    painter->drawLines(lines);
}
//画年数据 折线
void ChartModel::drawYearDataLines(QPainter * painter)
{
    QPen pen(QColor(0,0,0));
    pen.setWidth(1);
    painter->setPen(pen);
    QVector<QLine> lines;
    for(int i = 0; i+1 < 12; i++)
    {
        QLine line;
        line.setP1(QPoint(year_dots[i].x,year_dots[i].y));
        line.setP2(QPoint(year_dots[i+1].x,year_dots[i+1].y));
        lines.append(line);
    }
    painter->drawLines(lines);
}
//初始化月份Map容器
void ChartModel::initMonthMap()
{
    monthMap[1] = monthMap[3] = monthMap[5] = monthMap[7] = monthMap[8] = monthMap[10] = monthMap[12] = 31;
    monthMap[2] = 28;
    monthMap[4] = monthMap[6] = monthMap[9] = monthMap[11] = 30;
}
//初始化其他
void ChartModel::initOther()
{
    brushGroup[0] = QBrush(QColor(255,255,255));
    brushGroup[1] = QBrush(QColor(249,219,97));
}
//事件过滤器
bool ChartModel::eventFilter(QObject *obj, QEvent *event)
{
    QMouseEvent * mouseevent = dynamic_cast<QMouseEvent*>(event);
    if(event->type() == QMouseEvent::MouseButtonPress){
        if(mouseevent->button() == Qt::LeftButton){
            if(dotwindow != nullptr){
                dotwindow->close();
                delete dotwindow;
            }
            judgmentPointPosition(mouseevent->pos());
            dotwindow = new DotDetailDataWindow(DataState::HaveData,this);
            dotwindow->setParent(this);
            dotwindow->move(mouseevent->pos().x()-dotwindow->width()/2,mouseevent->pos().y());
            dotwindow->raise();
            dotwindow->show();
        }

    }else if(event->type() == QMouseEvent::MouseButtonRelease){
        if(mouseevent->button() == Qt::LeftButton){
            dotwindow->startTimer();
        }
    }
     return QWidget::eventFilter(obj, event);
}
//判断点的位置
void ChartModel::judgmentPointPosition(QPoint pos)
{
    switch (m_CurrentType){
        case ChartSelectType::week :{
           judgmentPointInWeekPosition(pos);
           break;
        }
        case ChartSelectType::month : {
           judgmentPointInMonthPosition(pos);
           break;
        }
        case ChartSelectType::year : {
           judgmentPointInYearPosition(pos);
           break;
        }
    }
}
//判断点的在周的位置
void ChartModel::judgmentPointInWeekPosition(QPoint pos)
{


}
//判断点的在月的位置
void ChartModel::judgmentPointInMonthPosition(QPoint pos)
{

}
//判断点的在年的位置
void ChartModel::judgmentPointInYearPosition(QPoint pos)
{


}
