#ifndef DISCOVERYMAININTERFACE_H
#define DISCOVERYMAININTERFACE_H

#include <QWidget>
#include <QStandardItemModel>
#include "Controler/PublicDataStruct.h"
#include <QSqlDatabase>

///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/1/18
/// @file    : DiscoveryMainInterface.h
/// @brief   : 介绍
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************

enum strType{
    Chinese = 1,
    Letter,
    Number,
    none
};

namespace Ui {
class DiscoveryMainInterface;
}

class DiscoveryMainInterface : public QWidget
{
    Q_OBJECT

public:
    explicit DiscoveryMainInterface(QWidget *parent = nullptr);
    ~DiscoveryMainInterface();

public slots:
    void onReceiveApplyMailDetail(MailTableStruct mailtable);    //当收到查看通讯录详情

    void onReceiveUpdateMailInfo(bool);                          //当收到刷新通讯录信息信号槽函数

    void onReceiveApplyDeleteMailInfo(MailTableStruct mailtable); //当收到删除通讯录数据请求信号槽函数

    void onAddButtonClicked();                                   //当添加按钮按下槽函数

    void onQueryButtonClicked();                                 //当查找按钮按下槽函数

    void onLineEditInput(QString str);                           //当搜索输入框输入时

    void onAboutButtonClicked();                                 //关于按钮按下槽函数

private:
    void initTableview();               //初始化表格
    void getMailInfo();                 //获取通讯数据信息
    static bool orderByChinese(MailTableStruct mailFirst,MailTableStruct mailSecond);  //通过汉字名称首字母排序
    static bool orderByLitter(MailTableStruct mailFirst,MailTableStruct mailSecond);   //通过首字母字符排序
    static bool orderByNumber(MailTableStruct mailFirst,MailTableStruct mailSecond);   //通过数字字符排序
    static bool orderByDefault(MailTableStruct mailFirst,MailTableStruct mailSecond);  //默认排序
    static bool orderByAccurate(MailTableStruct mailFirst,MailTableStruct mailSecond); //通过准确名字来排序
    strType checkStrType(QString str);  //检测字符串内容类型
private:
    QSqlDatabase coon;
    bool hasConnected;
    static QString comparedChar;               //被比较的首字符
    static QString comparedNumberChar;         //被比较的数字字符串
    Ui::DiscoveryMainInterface *ui;
    QStandardItemModel * model = nullptr;
    QVector<MailTableStruct> m_maillist;  //通讯数据汇总
};

#endif // DISCOVERYMAININTERFACE_H
