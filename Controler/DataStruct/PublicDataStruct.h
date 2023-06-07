#ifndef PUBLICDATASTRUCT_H
#define PUBLICDATASTRUCT_H
#pragma once
#include <QDate>
///****************************************************************************
/// @author  : MichaelFeng
/// @date    : 2023/1/5
/// @file    : PublicDataStruct.h
/// @brief   : 公共数据结构类
///----------------------------------------------------------------------------
///                           Change History
///----------------------------------------------------------------------------
///
///****************************************************************************
///
///
///
enum InOrOut{
    Income = 1,
    Expand
};

enum TypeBudget
{
  MonthlyBudget = 1,
  YearBudget
};

enum ChartSelectType
{
    week = 1,
    month,
    year
};

struct MaxThreeMessage
{
    float Amount;
    int   expandorincomeType;
    QString PayType;
    int typeId;
    QDate date;
};

struct DotData
{
    int id = 0;
    int x = 0;
    int y = 0;
    bool isnotEmpty = false;
    QDate date;
    QList<MaxThreeMessage> message;     //最大三比支出消息容器
};

struct BillTableStruct
{
    int billNo;     //账单号
    QDate date;     //日期
    double moneyAmount; //金额
    QString remarks;    //备注
    int userId;         //所属用户ID
    int typeId;         //消费类型ID
    int InOrOut = 2;    //(1-收入,2-支出)

    //---------------------------------
    QString PayType;     //类型
    QString IconPath;   //对应类型图片路径
};

struct MailDetailInfo
{
    int mailInfoId;     //mailInfoId;
    QString nickName;   //昵称
    QString hobby;      //兴趣业余爱好
    QString advantage;  //优点
    QString shortcomming; //缺点
    QString majorEvent; //主要事件与成就
    int mailId;         //所属通讯录ID；
};

struct MailTableStruct
{
    int mailId = -1;     //通讯录ID
    QString personName; //姓名
    int sex = 0;            //性别(1-男,2-女)
    int age = 0;            //年龄
    QDate birthdate;     //出生年月日s
    QString tel;        //电话号码
    QString qq;         //QQ号码
    QString email;      //邮箱
    QString industry;   //所属行业(广义、大方向)
    QString occupation; //职业(详细职业)
    QString address;    //居住地址
    QDate recentUpdateDate; //最近更新日期;
    int userId = -1;         //所属/填写用户ID
    MailDetailInfo maildetailinfo;    //详细信息
};


#endif // PUBLICDATASTRUCT_H
