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
    void drawCoordinateAxis(QPainter * painter);         //画坐标轴
    void drawWeekCoordinateAxis(QPainter * painter);     //画周坐标轴
    void drawMonthCoordinateAxis(QPainter * painter);    //画月坐标轴
    void drawYearCoordinateAxis(QPainter * painter);     //画年坐标轴
    void initMonthMap();            //初始化月份Map容器
    void getDateByWeekNum();        //获取日期通过周数
    void getdateByMonthNum();       //获取日期通过月数
private:
    ChartSelectType m_CurrentType;  //当前类型
    int m_CurrentWeekNum;           //当前周数
    int m_CurrentMonthNum;          //当前月数
    int m_CurrentYearNum;           //当前年数
    QLineF topline;                 //顶部横线
    QLineF bottomLine;              //底部横线
    int m_TotalExpand = 0;          //总支出
    int m_Average = 0;              //平均值
    float m_WeekData[7] = {0};      //周数据容器
    QString m_WeekCoordinate[7];    //周数据坐标容器[3-21,3-22,3-23,3-24,3-25,3-26]
    float m_MonthData[31] = {0};    //月数据容器
    QList<QString> m_MonthCoordinate;   //月数据坐标容器[01,05,09,13,18,22,26,30]
    int monthDataEndIndex;          //月数据容器末尾index
    float m_YearData[12] = {0};     //年数据容器
    //年数据坐标轴容器[1月，2月，3月，... ，12月]
    QString m_YearCoordinate[12] = {"1月","2月","3月","4月","5月","6月","7月","8月","9月","10月","11月","12月"};
    int yearDataEndIndex;           //年数据容器末尾数据index
    MaxThreeMessage message[3];     //最大三比支出消息容器
    QMap<int,int> monthMap;         //月份对应天数容器
};

#endif // CHARTMODEL_H
