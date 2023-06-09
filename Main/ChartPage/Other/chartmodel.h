#ifndef CHARTMODEL_H
#define CHARTMODEL_H
#include <QWidget>
#include "Controler/DataStruct/PublicDataStruct.h"
#include <QLineF>
#include <QMap>
#include <QPainter>
#include <QBrush>
#include <QPointer>
#include "Main/ChartPage/Other/dotdetaildatawindow.h"

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
    void setBillInfo(QVector<BillTableStruct> info);
    void setInOrOutType(InOrOut type){m_type = type;};
    void updateLoadData();
    void clearDotWindow();                               //清除当前DotWindow;
private:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;        //重写绘画事件
    void drawTextNum(QPainter * painter);                //画文字和数字
    void drawLine(QPainter * painter);                   //画线
    void drawCoordinateAxis(QPainter * painter);         //画坐标轴
    void drawWeekCoordinateAxis(QPainter * painter);     //画周坐标轴
    void drawMonthCoordinateAxis(QPainter * painter);    //画月坐标轴
    void drawYearCoordinateAxis(QPainter * painter);     //画年坐标轴
    void drawDataDot(QPainter * painter);                //画数据点
    void drawWeekDataDot(QPainter * painter);            //画周数据点
    void drawMonthDataDot(QPainter * painter);           //画月数据点
    void drawYearDataDot(QPainter * painter);            //画年数据点
    void drawDataLines(QPainter * painter);              //画数据   折线
    void drawWeekDataLines(QPainter * painter);          //画周数据 折线
    void drawMonthDataLines(QPainter * painter);         //画月数据 折线
    void drawYearDataLines(QPainter * painter);          //画年数据 折线
    void initMonthMap();            //初始化月份Map容器
    void initOther();               //初始化其他
    void getDateByWeekNum();        //获取日期通过周数
    void getdateByMonthNum();       //获取日期通过月数
    void calculateWeekBill();       //计算周账单
    void calculateMonthBill();      //计算月账单
    void calculateYearBill();       //计算年账单
    void bindWeekData();            //绑定周数据
    void bindMonthData();           //绑定月数据
    void bindYearData();            //绑定年数据
    float getWeekMax();             //获取周Max
    float getMonthMax();            //获取月Max
    float getYearMax();             //获取年Max
    void calculateWeekDotPosition();//计算周数据点的坐标
    void calculateMonthDotPosition();//计算月数据点的坐标
    void calculateYearDotPosition();//计算年数据点的坐标
    void sortWeekDotsTopThree();    //排序每日周数据前三
    void sortMonthDotsTopThree();   //排序每日月数据前三
    void sortYearDotsTopThree();    //排序每日年数据前三
    void sortMaxThreeMessage(DotData & dot); //将每个数据点中的消费数据按金额排升序
    static bool customsort(MaxThreeMessage previous,MaxThreeMessage next);   //自定义排序
    void calculateWeekTotalAndAverage();  //计算周总数和平均值
    void calculateMonthTotalAndAverage(); //计算周总数和平均值
    void calculateYearTotalAndAverage();  //计算周总数和平均值

    bool eventFilter(QObject *obj, QEvent *event) override;   //事件过滤器
    DotData judgmentPointPosition(QPoint pos);                   //判断点的位置
    DotData judgmentPointInWeekPosition(QPoint pos);          //判断点的在周的位置
    DotData judgmentPointInMonthPosition(QPoint pos);         //判断点的在月的位置
    DotData judgmentPointInYearPosition(QPoint pos);          //判断点的在年的位置
    void buildDotWindow(DotData data);                        //创建一个数据窗口

private:
    DotDetailDataWindow * dotwindow = nullptr;
private:
    ChartSelectType m_CurrentType;  //当前类型
    InOrOut m_type = InOrOut::Expand; //当前类型
    int m_CurrentWeekNum;           //当前周数
    int m_CurrentMonthNum;          //当前月数
    int m_CurrentYearNum;           //当前年数
    QLineF topline;                 //顶部横线
    QLineF bottomLine;              //底部横线
    float m_Total = 0;                //总支出/总收入
    float m_Average = 0;              //平均值
    float m_Max = 0;                //金额最大值
    int m_TopPos = 0;               //
    int m_BottomPos = 0;            //
    int m_height = 0;               //
    bool isHaveData = false;        //是否有数据

    QString m_WeekCoordinate[7];    //周数据坐标容器[3-21,3-22,3-23,3-24,3-25,3-26]
    QDate m_WeekDate[7];                //周日期数据容器
    QList<QString> m_MonthCoordinate;   //月数据坐标容器[01,05,09,13,18,22,26,30]
    QList<QDate> m_MonthDateList;   //月日期容器
    int monthDataEndIndex;          //月数据容器末尾index
    QDate m_YearDate[12];           //年日期容器
    //年数据坐标轴容器[1月，2月，3月，... ，12月]
    QString m_YearCoordinate[12] = {"1月","2月","3月","4月","5月","6月","7月","8月","9月","10月","11月","12月"};
    int yearDataEndIndex;           //年数据容器末尾数据index
    QMap<int,int> monthMap;         //月份对应天数容器
    //------------------------------------------------------------------------------------------------------//
    DotData week_dots[7];           //周数据数组
    QList<DotData> month_dots;      //月数据数组
    DotData year_dots[12];          //年数据数组
    QVector<BillTableStruct> billinfo;//账单数据
    //-------------------------------------------------------------------------------------------------------//
    float m_WeekData[7] = {0};      //周数据容器
    float m_MonthData[31] = {0};    //月数据容器
    float m_YearData[12] = {0};     //年数据容器
    //--------------------------------------------------------------------------------------------------------//
    QBrush brushGroup[2];           //画刷组
    const int radius = 10;           //圆半径

};

#endif // CHARTMODEL_H
