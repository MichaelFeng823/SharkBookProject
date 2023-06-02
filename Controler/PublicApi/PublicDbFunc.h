#ifndef PUBLICDBFUNC_H
#define PUBLICDBFUNC_H
#pragma once
#include <QSqlQuery>
#include <QVariant>
#include <QApplication>
#include <QScreen>
#include <QSettings>
#include <QVector>
#include "Kit/LogInfo/clog.h"
#include "Controler/DataStruct/PublicDataStruct.h"

namespace SqlFunc {
    //用户登录检测
    inline int loginCheck(QSqlQuery & query,QString userName,QString Psd)
    {
        int query_Id = -1;
        QString sql = QString("select UserID,UserName from Users where UserName = '%1' and UserPassword = '%2'").arg(userName).arg(Psd);
        query.prepare(sql);
        if(!query.exec()){

            LOG("LoginCheck Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
        }
        else{
            if(query.next()){
                 QString query_UserName = query.value("UserName").toString();
                 query_Id = query.value("UserID").toInt();
                 LOG("arrive 25 query_Id = %d,UserName = %s",query_Id,query_UserName.toStdString().c_str());
            }
        }
        return query_Id;
    }
    //用户登录检测
    inline int loginCheck_ForName(QSqlQuery & query,QString userName)
    {
        int query_Id = -1;
        QString sql = QString("select UserID from Users where UserName = '%1';").arg(userName);
        query.prepare(sql);
        if(!query.exec()){
            LOG("File:%sLine:%d",__FILE__,__LINE__);
            LOG("LoginCheck Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
        }
        else{
            if(query.next())
                query_Id = query.value("UserID").toInt();
        }
        return query_Id;
    }
    //注册用户 userName 用户名 ， psd 用户密码  返回值(-2 用户已存在 -1 注册失败,1 注册成功)
    inline int registerUser(QSqlDatabase & database,QSqlQuery & query,QString userName,QString Psd)
    {
        int query_ret = -1;
        if(loginCheck_ForName(query,userName) >= 0){
            query_ret = -2;
            return query_ret;
        }
        QString sql = QString("insert into Users(UserName,UserPassword) values('%1','%2');").arg(userName).arg(Psd);
        query.prepare(sql);
        if(!query.exec()){
            LOG("register_User Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
            database.rollback();
        }
        else{
            query_ret = 1;
            database.commit();
        }
        return query_ret;
    }

    inline bool changeUserPassword(QSqlDatabase & database,QSqlQuery & query,const QString Username,const QString newPassword)
    {
        int query_ret = -1;
        QString sql = QString("update Users set UserPassword = '%1' where UserName = '%2'").arg(newPassword).arg(Username);
        query.prepare(sql);
        if(!query.exec()){
            LOG("update_Users Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
            database.rollback();
        }
        else{
            query_ret = 1;
            database.commit();
        }
        return query_ret;
    }
}

namespace DataQuery {
    //账单数据查询
    inline int billQurey(QSqlQuery & query,int UserId, QDate date, QVector<BillTableStruct> & billList)
    {
        billList.clear();
        int queryCounts = 0;
        QString sql = QString("select * from BillTable where UserId = %1 and Year = %2 and Month = %3;").arg(UserId).arg(date.year()).arg(date.month());
        query.prepare(sql);
        if(!query.exec()){
            LOG("File:%sLine:%d",__FILE__,__LINE__);
            LOG("billQurey Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
        }
        else{
            while(query.next()){
                queryCounts++;
                BillTableStruct billtable;
                billtable.billNo = query.value("BillNo").toInt();
                int year = query.value("Year").toInt();
                int month = query.value("Month").toInt();
                int day = query.value("Day").toInt();
                int type = query.value("TypeId").toInt();
                int inorout = query.value("InOrOut").toInt();
                billtable.date.setDate(year,month,day);
                billtable.moneyAmount = query.value("MoneyAmount").toDouble();
                billtable.remarks = query.value("Remark").toString();
                billtable.userId = query.value("UserId").toInt();
                billtable.typeId = type;
                billtable.InOrOut = inorout;
                billList.append(billtable);
                LOG("get bill data!");
                LOG("queryCounts:%d",queryCounts);
            }
        }
        LOG("queryCounts:%d",queryCounts);
        LOG("the sql is:%s",sql.toStdString().c_str());
        return queryCounts;
    }

    inline int billQureyOfYear(QSqlQuery & query,int UserId, QDate date, QVector<BillTableStruct> & billList)
    {
        billList.clear();
        int queryCounts = 0;
        QString sql = QString("select * from BillTable where UserId = %1 and Year = %2;").arg(UserId).arg(date.year());
        query.prepare(sql);
        if(!query.exec()){
            LOG("File:%sLine:%d",__FILE__,__LINE__);
            LOG("billQurey Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
        }
        else{
            while(query.next()){
                queryCounts++;
                BillTableStruct billtable;
                billtable.billNo = query.value("BillNo").toInt();
                int year = query.value("Year").toInt();
                int month = query.value("Month").toInt();
                int day = query.value("Day").toInt();
                int type = query.value("TypeId").toInt();
                int inorout = query.value("InOrOut").toInt();
                billtable.date.setDate(year,month,day);
                billtable.moneyAmount = query.value("MoneyAmount").toDouble();
                billtable.remarks = query.value("Remark").toString();
                billtable.userId = query.value("UserId").toInt();
                billtable.typeId = type;
                billtable.InOrOut = inorout;
                billList.append(billtable);
                LOG("get bill data!");
                LOG("queryCounts:%d",queryCounts);
            }
        }
        LOG("queryCounts:%d",queryCounts);
        LOG("the sql is:%s",sql.toStdString().c_str());
        return queryCounts;
    }

    inline int billQueryofMonth(QSqlQuery & query,int UserId,QDate currentdate,int month,QVector<BillTableStruct> & billList){
        billList.clear();
        int queryCounts = 0;
        QString sql = QString("select * from BillTable where UserId = %1 and Year = %2 and Month = %3;").arg(UserId).arg(currentdate.year()).arg(month);
        query.prepare(sql);
        if(!query.exec()){
            LOG("File:%sLine:%d",__FILE__,__LINE__);
            LOG("billQueryofMonth Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
        }
        else{
            while(query.next()){
                queryCounts++;
                BillTableStruct billtable;
                billtable.billNo = query.value("BillNo").toInt();
                int year = query.value("Year").toInt();
                int month = query.value("Month").toInt();
                int day = query.value("Day").toInt();
                int type = query.value("TypeId").toInt();
                int inorout = query.value("InOrOut").toInt();
                billtable.date.setDate(year,month,day);
                billtable.moneyAmount = query.value("MoneyAmount").toDouble();
                billtable.remarks = query.value("Remark").toString();
                billtable.userId = query.value("UserId").toInt();
                billtable.typeId = type;
                billtable.InOrOut = inorout;
                billList.append(billtable);
            }
        }
        LOG("queryCounts:%d",queryCounts);
        LOG("the sql is:%s",sql.toStdString().c_str());
        return queryCounts;
    }
    inline int billQueryofWeek(QSqlQuery & query,int UserId,QVector<QDate> datelist,QVector<BillTableStruct> & billList){
        billList.clear();
        int queryCounts = 0;
        for(int i = 0; i < datelist.size(); i++){
            QString sql = QString("select * from BillTable where UserId = %1 and Year = %2 and Month = %3 and Day = %4").arg(UserId)
                    .arg(datelist[i].year()).arg(datelist[i].month()).arg(datelist[i].day());
            query.prepare(sql);
            if(!query.exec()){
                LOG("File:%sLine:%d",__FILE__,__LINE__);
                LOG("billQueryofMonth Error:query.exec() = False");
                LOG("the sql is:%s",sql.toStdString().c_str());
            }
            else{
                while(query.next()){
                    queryCounts++;
                    BillTableStruct billtable;
                    billtable.billNo = query.value("BillNo").toInt();
                    int year = query.value("Year").toInt();
                    int month = query.value("Month").toInt();
                    int day = query.value("Day").toInt();
                    int type = query.value("TypeId").toInt();
                    int inorout = query.value("InOrOut").toInt();
                    billtable.date.setDate(year,month,day);
                    billtable.moneyAmount = query.value("MoneyAmount").toDouble();
                    billtable.remarks = query.value("Remark").toString();
                    billtable.userId = query.value("UserId").toInt();
                    billtable.typeId = type;
                    billtable.InOrOut = inorout;
                    billList.append(billtable);
                }
            }
            LOG("the sql is:%s",sql.toStdString().c_str());
        }
        LOG("queryCounts:%d",queryCounts);
        return queryCounts;
    }

    //通讯录数据查询
    inline int mailQurey(QSqlQuery & query,int UserId,QVector<MailTableStruct> & mailList)
    {
        mailList.clear();
        int queryCounts = 0;
        QString sql = QString("select * from MailTable where UserId = %1;").arg(UserId);
        query.clear();
        query.prepare(sql);
        if(!query.exec()){
            LOG("File:%sLine:%d",__FILE__,__LINE__);
            LOG("mailQurey Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
        }
        else{
            while(query.next()){
                queryCounts++;
                MailTableStruct mailtablestruct;
                mailtablestruct.mailId = query.value("MailId").toInt();
                mailtablestruct.personName = query.value("PersonName").toString();
                mailtablestruct.sex = query.value("Sex").toInt();
                mailtablestruct.age = query.value("Age").toInt();
                mailtablestruct.tel = query.value("Tel").toString();
                mailtablestruct.qq = query.value("QQ").toString();
                mailtablestruct.email = query.value("Email").toString();
                mailtablestruct.industry = query.value("Industry").toString();
                mailtablestruct.occupation = query.value("Occupation").toString();
                mailtablestruct.address = query.value("Address").toString();

                int BirthYear = query.value("BirthYear").toInt();
                int BirthMonth = query.value("BirthMonth").toInt();
                int BirthDay = query.value("BirthDay").toInt();
                mailtablestruct.birthdate.setDate(BirthYear,BirthMonth,BirthDay);

                int RecentUpdateYear = query.value("RecentUpdateYear").toInt();
                int RecentUpdateMonth = query.value("RecentUpdateMonth").toInt();
                int RecentUpdateDay = query.value("RecentUpdateDay").toInt();
                mailtablestruct.recentUpdateDate.setDate(RecentUpdateYear,RecentUpdateMonth,RecentUpdateDay);

                mailtablestruct.userId = query.value("UserId").toInt();
                mailList.append(mailtablestruct);

                LOG("get Mail data!");
                LOG("mailtablestruct.mailId = %d",mailtablestruct.mailId);
                LOG("mailtablestruct.sex = %d",mailtablestruct.sex);
                LOG("queryCounts:%d",queryCounts);
            }
        }
        for(int i=0; i < mailList.size();i++){
            sql = QString("select * from MailDetailInfo where MailId = %1;").arg(mailList[i].mailId);
            query.prepare(sql);
            if(!query.exec()){
                LOG("File:%sLine:%d",__FILE__,__LINE__);
                LOG("mailQurey Error:query.exec() = False");
                LOG("the sql is:%s",sql.toStdString().c_str());
            }
            else{
                 if(query.next()){
                     mailList[i].maildetailinfo.mailInfoId = query.value("MailInfoId").toInt();
                     mailList[i].maildetailinfo.nickName = query.value("NickName").toString();
                     mailList[i].maildetailinfo.hobby = query.value("Hobby").toString();
                     mailList[i].maildetailinfo.advantage = query.value("Advantage").toString();
                     mailList[i].maildetailinfo.shortcomming = query.value("ShortComming").toString();
                     mailList[i].maildetailinfo.majorEvent = query.value("MajorEvent").toString();
                     mailList[i].maildetailinfo.mailId = query.value("MailId").toInt();
                     LOG("nickName:%s",mailList[i].maildetailinfo.nickName.toStdString().c_str());
                     LOG("majorEvent:%s",mailList[i].maildetailinfo.majorEvent.toStdString().c_str());
                 }
            }
            LOG("the sql is:%s",sql.toStdString().c_str());
        }
        LOG("queryCounts:%d",queryCounts);
        return queryCounts;
    }
}

namespace DataInsert {
     //账单新增数据插入
    inline int billInsert(QSqlDatabase & database,QSqlQuery & query,BillTableStruct & billStrcut)
    {
        int queryResult = -1;
        QString sql = QString("insert into BillTable(Year,Month,Day,MoneyAmount,Remark,UserId,TypeId,InOrOut) values('%1','%2','%3','%4','%5','%6','%7','%8');").arg(billStrcut.date.year())
                .arg(billStrcut.date.month()).arg(billStrcut.date.day()).arg(billStrcut.moneyAmount).arg(billStrcut.remarks).arg(billStrcut.userId).arg(billStrcut.typeId).arg(billStrcut.InOrOut);
        query.prepare(sql);
        if(!query.exec()){
            LOG("File:%sLine:%d",__FILE__,__LINE__);
            LOG(" billInsert Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
            database.rollback();
        }
        else{
            queryResult = 1;
            database.commit();
        }
        LOG("the sql is:%s",sql.toStdString().c_str());
        LOG("the queryResult is:%d",queryResult);
        return queryResult;
    }

    //通讯录新增数据插入
   inline int mailInsert(QSqlDatabase & database,QSqlQuery & query,MailTableStruct & mailStrcut)
   {
       int queryResult = -1;
       QString sql = QString("insert into MailTable(PersonName,Sex,Age,Tel,QQ,Email,Industry,Occupation,Address,BirthYear,BirthMonth,BirthDay,RecentUpdateYear,RecentUpdateMonth,RecentUpdateDay,UserId)"
                             "values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16');")
               .arg(mailStrcut.personName)
               .arg(mailStrcut.sex)
               .arg(mailStrcut.age)
               .arg(mailStrcut.tel)
               .arg(mailStrcut.qq)
               .arg(mailStrcut.email)
               .arg(mailStrcut.industry)
               .arg(mailStrcut.occupation)
               .arg(mailStrcut.address)
               .arg(mailStrcut.birthdate.year())
               .arg(mailStrcut.birthdate.month())
               .arg(mailStrcut.birthdate.day())
               .arg(mailStrcut.recentUpdateDate.year())
               .arg(mailStrcut.recentUpdateDate.month())
               .arg(mailStrcut.recentUpdateDate.day())
               .arg(mailStrcut.userId);
       query.prepare(sql);
       if(!query.exec()){
           LOG("File:%sLine:%d",__FILE__,__LINE__);
           LOG(" MailTable Insert Error:query.exec() = False");
           LOG("the sql is:%s",sql.toStdString().c_str());
           database.rollback();
       }
       else{
           queryResult = 1;
           database.commit();
       }
       LOG("the sql is:%s",sql.toStdString().c_str());
       LOG("the queryResult is:%d",queryResult);

       sql = QString("select last_insert_rowid() from MailTable;");
       int lastInsertRowId = -1;
       query.prepare(sql);
       if(!query.exec()){
           LOG("File:%sLine:%d",__FILE__,__LINE__);
           LOG("last_insert_rowid Error:query.exec() = False");
           LOG("the sql is:%s",sql.toStdString().c_str());
       }
       else{
           if(query.next()){
               lastInsertRowId = query.value("last_insert_rowid()").toInt();
               mailStrcut.mailId = lastInsertRowId;
               mailStrcut.maildetailinfo.mailId = lastInsertRowId;
               LOG("lastInsertRowId:%d",lastInsertRowId);
           }
       }

       if(lastInsertRowId > 0){
           sql = QString("insert into MailDetailInfo(NickName,Hobby,Advantage,ShortComming,MajorEvent,MailId)"
                         "values('%1','%2','%3','%4','%5','%6');")
                   .arg(mailStrcut.maildetailinfo.nickName)
                   .arg(mailStrcut.maildetailinfo.hobby)
                   .arg(mailStrcut.maildetailinfo.advantage)
                   .arg(mailStrcut.maildetailinfo.shortcomming)
                   .arg(mailStrcut.maildetailinfo.majorEvent)
                   .arg(mailStrcut.maildetailinfo.mailId);
           query.prepare(sql);
           if(!query.exec()){
               LOG("File:%sLine:%d",__FILE__,__LINE__);
               LOG(" MailDetailInfo Insert Error:query.exec() = False");
               LOG("the sql is:%s",sql.toStdString().c_str());
               database.rollback();
           }
           else{
               queryResult = 1;
               database.commit();
           }
       }
       else{
           LOG("fail to get lastinsertrowid!");
           queryResult = -1;
       }
       return queryResult;
   }
}

namespace DataUpdate {
     //账单数据修改
    inline int billUpdate(QSqlDatabase & database,QSqlQuery & query,BillTableStruct & billStrcut)
    {
        int queryResult = -1;
        QString sql = QString("update BillTable set Year = '%1',Month = '%2',Day = '%3',MoneyAmount = '%4',Remark = '%5',UserId = '%6',TypeId = '%7',InOrOut = '%8' where BillNo = '%9';").arg(billStrcut.date.year())
                .arg(billStrcut.date.month()).arg(billStrcut.date.day()).arg(billStrcut.moneyAmount).arg(billStrcut.remarks).arg(billStrcut.userId).arg(billStrcut.typeId).arg(billStrcut.InOrOut).arg(billStrcut.billNo);
        query.prepare(sql);
        if(!query.exec()){
            LOG("File:%sLine:%d",__FILE__,__LINE__);
            LOG(" billUpdate Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
            database.rollback();
        }
        else{
            queryResult = 1;
            database.commit();
        }
        LOG("the sql is:%s",sql.toStdString().c_str());
        LOG("the queryResult is:%d",queryResult);
        return queryResult;
    }

    //通讯录数据修改
   inline int mailUpdate(QSqlDatabase & database,QSqlQuery & query,MailTableStruct & mailStrcut)
   {
       int queryResult = -1;
       QString sql = QString("update MailTable set PersonName = '%1', Sex = %2, Age = %3, Tel = '%4' , QQ = '%5' , Email = '%6' , Industry = '%7' ,Occupation = '%8' , Address = '%9' , BirthYear = %10,BirthMonth = %11, BirthDay = %12, RecentUpdateYear = %13 , RecentUpdateMonth = %14, RecentUpdateDay = %15,UserId = %16 where MailId = %17;").arg(mailStrcut.personName)
               .arg(mailStrcut.sex)
               .arg(mailStrcut.age)
               .arg(mailStrcut.tel)
               .arg(mailStrcut.qq)
               .arg(mailStrcut.email)
               .arg(mailStrcut.industry)
               .arg(mailStrcut.occupation)
               .arg(mailStrcut.address)
               .arg(mailStrcut.birthdate.year())
               .arg(mailStrcut.birthdate.month())
               .arg(mailStrcut.birthdate.day())
               .arg(mailStrcut.recentUpdateDate.year())
               .arg(mailStrcut.recentUpdateDate.month())
               .arg(mailStrcut.recentUpdateDate.day())
               .arg(mailStrcut.userId)
               .arg(mailStrcut.mailId);

       query.prepare(sql);
       if(!query.exec()){
           LOG("File:%sLine:%d",__FILE__,__LINE__);
           LOG("mailUpdate Error:query.exec() = False");
           LOG("the sql is:%s",sql.toStdString().c_str());
           database.rollback();
       }
       else{
           queryResult = 1;
           database.commit();
       }

       sql = QString("update MailDetailInfo set NickName = '%1',Hobby = '%2',Advantage = '%3',ShortComming = '%4',MajorEvent = '%5' where MailId = %6;").arg(mailStrcut.maildetailinfo.nickName)
               .arg(mailStrcut.maildetailinfo.hobby)
               .arg(mailStrcut.maildetailinfo.advantage)
               .arg(mailStrcut.maildetailinfo.shortcomming)
               .arg(mailStrcut.maildetailinfo.majorEvent)
               .arg(mailStrcut.maildetailinfo.mailId);

       query.prepare(sql);
       if(!query.exec()){
           LOG("File:%sLine:%d",__FILE__,__LINE__);
           LOG("mailUpdate Error:query.exec() = False");
           LOG("the sql is:%s",sql.toStdString().c_str());
           queryResult = -1;
           database.rollback();
       }
       else{
           queryResult = 1;
           database.commit();
       }

       LOG("the sql is:%s",sql.toStdString().c_str());
       LOG("the queryResult is:%d",queryResult);
       return queryResult;
   }
}

namespace DataDelete {
     //账单数据删除
    inline int billDelete(QSqlDatabase & database,QSqlQuery & query,BillTableStruct & billStrcut)
    {
        int queryResult = -1;
        QString sql = QString("delete from  BillTable where BillNo = '%1';").arg(billStrcut.billNo);
        query.prepare(sql);
        if(!query.exec()){
            LOG("File:%sLine:%d",__FILE__,__LINE__);
            LOG(" billDelete Error:query.exec() = False");
            LOG("the sql is:%s",sql.toStdString().c_str());
            database.rollback();
        }
        else{
            queryResult = 1;
            database.commit();
        }
        LOG("the sql is:%s",sql.toStdString().c_str());
        LOG("the queryResult is:%d",queryResult);
        return queryResult;
    }
    //通讯录数据删除
   inline int mailDelete(QSqlDatabase & database,QSqlQuery & query,MailTableStruct & mailStrcut)
   {
       int queryResult = -1;
       QString sql = QString("delete from MailTable where MailId = '%1';").arg(mailStrcut.mailId);
       query.prepare(sql);
       if(!query.exec()){
           LOG("File:%sLine:%d",__FILE__,__LINE__);
           LOG(" MailTable Delete Error:query.exec() = False");
           LOG("the sql is:%s",sql.toStdString().c_str());
           database.rollback();
       }
       else{
           queryResult = 1;
           database.commit();
       }

       sql = QString("delete from MailDetailInfo where MailInfoId = '%1'").arg(mailStrcut.maildetailinfo.mailInfoId);
       query.prepare(sql);
       if(!query.exec()){
           LOG("File:%sLine:%d",__FILE__,__LINE__);
           LOG(" MailDetailInfo Delete Error:query.exec() = False");
           LOG("the sql is:%s",sql.toStdString().c_str());
           database.rollback();
       }
       else{
           queryResult = 1;
           database.commit();
       }

       LOG("the sql is:%s",sql.toStdString().c_str());
       LOG("the queryResult is:%d",queryResult);
       return queryResult;
   }
}
namespace ScreenFunc{
    /*
    @breif:公共方法库
    @prama:
    @author:MichaelFeng
    @Date:2022-11-26
    */
    static QSize getScreenSize()
    {
        QScreen * iScreen = qApp->primaryScreen();
        QSize rScreenSize = iScreen->size();
        return rScreenSize;
    }
}

namespace ReadWriteIni {
    inline bool readIni(QString File,QString groupName,QString Key,QString & Value)
    {
        Value = QString("");
            if(File.isEmpty() || Key.isEmpty()){
                return false;
            }
            else{
                //创建操作对象
                QSettings config(File,QSettings::IniFormat);
                //读取
                Value = config.value(groupName+"/"+ Key).toString();
                LOG("Read Key:%s,Value:%s",Key.toStdString().c_str(),Value.toStdString().c_str());
                return true;
            }
    }
    inline bool writeIni(QString File,QString groupName,QString Key,QString Value)
    {
        if(File.isEmpty() || Key.isEmpty()){
               return false;
           }
           else{
               //创建文件
               QSettings config(File,QSettings::IniFormat);
               LOG("Write Key:%s,Value:%s",Key.toStdString().c_str(),Value.toStdString().c_str());
               //写入
               config.beginGroup(groupName);
               config.setValue(Key,Value);
               config.endGroup();
               return true;
           }
    }
}

namespace PublicSort{
    //通过金额排序
    inline static bool sortByMoney(const BillTableStruct & billfirst,const BillTableStruct & billsecond)
    {
         return billfirst.moneyAmount > billsecond.moneyAmount;
    }
    //通过时间排序
    inline static bool sortByTime(const BillTableStruct & billfirst,const BillTableStruct & billsecond)
    {
        return billfirst.date > billsecond.date;
    }
}



#endif // PUBLICDBFUNC_H
