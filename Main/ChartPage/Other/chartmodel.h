#ifndef CHARTMODEL_H
#define CHARTMODEL_H
#include <QWidget>
#include "Controler/DataStruct/PublicDataStruct.h"
#include <QLineF>
#include <QMap>
#include <QPainter>

///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/5/30
/// @file    : chartmodel.h
/// @brief   : 图表界面模型
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
class ChartModel:public QWidget
{
    Q_OBJECT
public:
    ChartModel(ChartSelectType type,QWidget * parent = nullptr);
public:
    void setId(int id);
private:
    struct MaxThreeMessage
    {
        float expand;
        int   expandType;
        QDate date;
    };
private:
    void paintEvent(QPaintEvent *event) override;        //重写绘画事件
    void drawTextNum(QPainter * painter);                //画文字和数字
    void drawLine(QPainter * painter);                   //画线
    void initMonthMap();            //初始化月份Map容器
private:
    ChartSelectType m_CurrentType;  //当前类型
    int m_CurrentWeekNum;           //当前周数
    int m_CurrentMonthNum;          //当前月数
    int m_CurrentYearNum;           //当前年数
    QLineF topline;                 //顶部横线
    QLineF bottomLine;              //底部横线
    int m_TotalExpand = 0;              //总支出
    int m_Average = 0;                  //平均值
    float m_WeekData[7] = {0};      //周数据容器
    float m_MonthData[31] = {0};    //月数据容器
    int monthDataEndIndex;          //月数据容器末尾index
    float m_YearData[12] = {0};     //年数据容器
    int yearDataEndIndex;           //年数据容器末尾数据index
    MaxThreeMessage message[3];     //最大三比支出消息容器
    QMap<int,int> monthMap;         //月份对应天数容器
};

#endif // CHARTMODEL_H
