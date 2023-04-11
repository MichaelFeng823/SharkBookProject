#ifndef PERSONDETAILINFOSUBPAGE_H
#define PERSONDETAILINFOSUBPAGE_H
#include <QWidget>
#include "Controler/DataStruct/PublicDataStruct.h"
#include "Kit/Calendar/calendar.h"
#include <QSqlDatabase>

namespace Ui {
class PersonDetailInfoSubPage;
}

class PersonDetailInfoSubPage : public QWidget
{
    Q_OBJECT
public:
    explicit PersonDetailInfoSubPage(QWidget *parent = nullptr);
    PersonDetailInfoSubPage(bool is_add,QWidget *parent = nullptr);
    ~PersonDetailInfoSubPage();
public:
    void setBaseInfo(MailTableStruct mailtable);        //设置基本信息

    void setPersonName(QString name);                   //设置姓名

    void setPersonSex(int sex);                         //设置性别

    void setPersonNickName(QString nickname);           //设置昵称

    void setPersonAge(int age);                         //设置年龄

    void setPersonBirthday(QDate date);                 //设置生日

    void setPersonIndustry(QString industry);           //设置行业

    void setPersonOccupation(QString occupation);       //设置职业

    void setPersonTel(QString tel);                     //设置电话

    void setPersonQQ(QString qq);                       //设置QQ

    void setPersonEmail(QString email);                 //设置邮箱

    void setPersonAddress(QString address);             //设置住址

    void setPersonHobby(QString hobby);                 //设置爱好

    void setPersonAdvantage(QString advantage);         //设置优点

    void setPersonShortComming(QString shortcomming);   //设置缺点

    void setPersonMajorEvent(QString majorevent);       //设置主要事件

    void setInputStatus(bool status);                   //设置输入状态

    void getInputData();                                //获取修改后输入数据

    bool InputIsEmptyCheck();                           //插入数据时非空检测

public slots:
    void onEditButtonClicked();                        //当编辑按钮点击槽函数

    void onSaveButtonClicked();                        //当保存按钮点击槽函数

    void onReturnButtonClicked();                      //当返回按钮点击槽函数

    void onBirthdayLineEditClicked();                  //当生日编辑框点击时槽函数

    void receiveTimeInfo(int year,int month,int day);  //收到时间信息槽函数

signals:
    void callApplyUpdateSignal(bool);                      //发送申请

private:
     bool eventFilter(QObject *obj, QEvent *event); //事件过滤器

private:
    Ui::PersonDetailInfoSubPage *ui;
    MailTableStruct m_mailInfo;                        //通讯录结构变量
    Calendar * m_Calendar;                             //日历控件
    QSqlDatabase coon;
    bool hasConnected;                                 //数据库是否已连接
    bool m_IsAdd = false;                                      //是否为添加
};

#endif // PERSONDETAILINFOSUBPAGE_H
