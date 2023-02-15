#ifndef PERSONINFODISPLAYKIT_H
#define PERSONINFODISPLAYKIT_H

#include <QWidget>
#include "Controler/PublicDataStruct.h"
#include <QTimer>

namespace Ui {
class PersonInfoDisplayKit;
}

class PersonInfoDisplayKit : public QWidget
{
    Q_OBJECT

public:
    explicit PersonInfoDisplayKit(QWidget *parent = nullptr);
    ~PersonInfoDisplayKit();    
public:
    void setPersonName(QString name);             //设置人名

    void setSex(int sex);                         //设置性别

    void setAge(int age);                         //设置年龄

    void setBirthDate(QDate date);                //设置出生年月日

    void setIndustry(QString industry);           //设置行业名称

    void setOccupation(QString occupation);       //设置岗位

    void setTel(QString tel);                     //设置电话号码

    void setQQ(QString qq);                       //设置qq

    void setEmail(QString email);                 //设置邮箱

    void setAddress(QString address);             //设置地址

    void setMailInfo(MailTableStruct mailtable);  //设置mail信息

    void setRecentUpdate(QDate date);             //最近更新日期

public slots:
    void onDetailButtonClicked(bool);             //详情按钮按下槽函数

    void onDeleteButtonClicked(bool);             //删除按钮按下槽函数

signals:
    void callApplyMailDetail(MailTableStruct mailtable);                            //请求通讯录详情信息信号

    void callApplyDeleteMailInfo(MailTableStruct mailtable);                        //请求删除通讯录数据信号

private:
    bool eventFilter(QObject *obj, QEvent *event); //事件过滤器

    void checkDeleteButtonStatus();                //检测当前删除按钮是否可用

    void onPressedTimerFunc();                     //按下后的计时函数

private:
    Ui::PersonInfoDisplayKit *ui;

    MailTableStruct m_mailtablestruct;              //通讯录结构

    bool m_DeleteButtonIsvisiable = false;          //删除按钮是否可见

    int  m_PressedTime = 0;                         //按下时间

    QTimer timer;                                   //定时器对象


};

#endif // PERSONINFODISPLAYKIT_H
