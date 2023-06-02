#ifndef PUBLICUERINFO_H
#define PUBLICUERINFO_H
#pragma once
#include<QString>
#include<QStringList>
class UserInfo {
public:
    inline static QString UserName;
    inline static QString UserPassWord;
    inline static int     UserID;
    inline static QString RemmberPassState;
    inline static QString AutoLogin;
};
class SYSTEMSTATE{
public:
    inline static bool NETWORKSTATE;
};
class GLOBALDATA{
public:
    inline static QVector<QString> expandituremenuvector;         //记账支出菜单容器
    inline static QVector<QString> incomemenuvector;              //记账收入菜单容器
};
enum InAndOutType
{
    InType = 1,
    OutType
};

inline const QStringList dayOfWeak = {"日","一","二","三","四","五","六"};
#endif // PUBLICUERINFO_H
