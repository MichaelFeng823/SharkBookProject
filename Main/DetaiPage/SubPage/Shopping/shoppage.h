#ifndef SHOPPAGE_H
#define SHOPPAGE_H

#include <QWidget>
#include "Kit/Calendar/calendar.h"

namespace Ui {
class ShopPage;
}

class ShopPage : public QWidget
{
    Q_OBJECT

public:
    explicit ShopPage(QWidget *parent = nullptr);
    ~ShopPage();

protected:
    QString parsingData(QByteArray * data);     //解析数据
    QString mergeData(QJsonObject object_data); //合并数据

public slots:
    void onQueryButtonClicked();                //当查询按钮被点击是的槽函数
    void onRecvMsg(QByteArray data, QObject * obj); //当收到消息时的槽函数
    void onDateLineEditClicked();                   //当日期编辑框点击时槽函数
    void receiveTimeInfo(int year,int month,int day); //收到时间信息槽函数

private:
    Ui::ShopPage *ui;
    Calendar * m_Calendar;                             //日历控件
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // SHOPPAGE_H
