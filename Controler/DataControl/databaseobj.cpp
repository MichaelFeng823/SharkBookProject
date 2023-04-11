#include "databaseobj.h"
#include <QMutexLocker>
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QVariantList>
#include "Kit/LogInfo/clog.h"
#include "Controler/GlobalInfo/GlobalDocumentPath.h"
using namespace  DocPath;
DataBaseObj * DataBaseObj::m_Self = NULL;
QMutex DataBaseObj::my_Mutex;
bool DataBaseObj::my_DbStatus = false;

DataBaseObj::DataBaseObj()
{

}
//获取单实例
 DataBaseObj * DataBaseObj::getInstance()
 {
     if(DataBaseObj::m_Self == NULL){
         //防止多线程创造两个对象
         QMutexLocker locker(&DataBaseObj::my_Mutex);
         //再判断一次，防止其他线程抢先new过对象
         if(DataBaseObj::m_Self == NULL){
             DataBaseObj::m_Self = new DataBaseObj();
         }
     }

    //返回单实例
     return DataBaseObj::m_Self;
 }
///获取数据库连接对象封装()
bool DataBaseObj::getConnecttion(QSqlDatabase & conn)
{
    //是否成功
   bool isSucc = true;
   QString app_dir;
   app_dir = QApplication::applicationDirPath();

   LOG("appdir:%s",app_dir.toStdString().c_str());
   //获取单实例
   DataBaseObj * singleTon = DataBaseObj::getInstance();
   //QString db_dir;
#ifdef Q_OS_WIN
   app_dir = QApplication::applicationDirPath();
#endif

#ifdef Q_OS_ANDROID
   //db_dir = "assets:/souce/myDb.db";
#endif
   QFile file(android_local_db_dir);
#if 0
   if(0==0){
       QString infostr = QString::fromLocal8Bit("File is not exsit,bulit it! and path:%1").arg(android_local_db_dir);
       LOG("%s",infostr.toStdString().c_str());
       file.copy("assets:/myDb.db",android_local_db_dir);      //拷到安卓本地目录
       file.setPermissions(QFile::WriteOwner | QFile::ReadOwner); //以读写的方式进行拷贝
       file.close();
   }

#else
   if(!file.exists()||file.size() == 0){
       QString infostr = QString::fromLocal8Bit("File is not exsit,bulit it! and path:%1").arg(android_local_db_dir);
       LOG("%s",infostr.toStdString().c_str());
       file.copy("assets:/myDb.db",android_local_db_dir);      //拷到安卓本地目录
       file.setPermissions(QFile::WriteOwner | QFile::ReadOwner); //以读写的方式进行拷贝
       file.close();
   }
#endif

   QFile file_test_assets(android_local_db_dir);
   if(file_test_assets.open(QIODevice::ReadWrite)){
       LOG("%s","Find ./myDb.db");

   }
   else
   {
        LOG("%s","Fail to Find ./myDb.db");
   }

   //加锁
   QMutexLocker lockers(&DataBaseObj::my_Mutex);
   if(!singleTon->my_DbStatus){

       QStringList driverlist = QSqlDatabase::drivers();
       for(int i = 0; i <driverlist.size(); i++)
       {
           LOG("%s\n",driverlist[i].toStdString().c_str());
           LOG("Driverlist_size:%d\n",driverlist.size());
       }
       singleTon->my_ConnObj = QSqlDatabase::addDatabase("QSQLITE");
       //设置数据库名，添加内存数据库。
       singleTon->my_ConnObj.setDatabaseName(android_local_db_dir);
      //若未打开数据库则显示错误，
       if(!singleTon->my_ConnObj.open())
       {
           QMessageBox::warning(0,"error",singleTon->my_ConnObj.lastError().text());
           LOG("%s",singleTon->my_ConnObj.lastError().text().toStdString().c_str());
           return false;
       }
       else
       {
           LOG("%s","DataBase is open!");
           singleTon->my_DbStatus = true;
       }
       QSqlQuery sqlquery(singleTon->my_ConnObj);
       singleTon->my_ConnObj.transaction();
       if(checkTableIsExsit(singleTon->my_ConnObj,sqlquery))
           singleTon->my_ConnObj.commit();

       if(isSucc)
           conn = m_Self->my_ConnObj;
       LOG("Arrive 102line");
       return true;
   }
   else
   {
       LOG("Arrive 102line database already open");
       conn = singleTon->my_ConnObj;
       return true;
   }
}
//创建sql
 QString DataBaseObj::BuildSql()
 {
     LOG("初始化sql语句");
     QString sql = "create table if not exists Users("
                   "UserID INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "UserName varchar(64) default '',"
                   "UserPassword varchar(64) default '');:\n"

                   "insert into Users(UserID,UserName,UserPassword) Select '0','MichaelFeng','fj082300'"
                   "where not exists(select 1 from Users where UserID = '0');:\n"

                   //"drop Table if exists BillTable;:\n"

                   "create table if not exists BillTable ("
                   "BillNo INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "Year INTEGER,"
                   "Month  INTEGER,"
                   "Day INTEGER,"
                   "MoneyAmount NUMERIC,"
                   "Remark VARHCAR,"
                   "UserId INTEGER,"
                   "TypeId INTEGER,"
                   "InOrOut INTEGER);:\n"

                   "insert into BillTable(BillNo,Year,Month,Day,MoneyAmount,Remark,UserId,TypeId,InOrOut)"
                   "Select '3','2023','1','5','200.5','测试','0','1','2' where not exists(select 1 from BillTable where BillNo = '3');:\n"

                   "insert into BillTable(BillNo,Year,Month,Day,MoneyAmount,Remark,UserId,TypeId,InOrOut)"
                   "Select '4','2023','2','5','300.5','测试0106','0','2','2' where not exists(select 1 from BillTable where BillNo = '4');:\n"

                   //"drop Table if exists MailTable;:\n"
                   //"drop Table if exists MailDetailInfo;:\n"

                   "create table if not exists MailTable ("
                    "MailId            INTEGER    PRIMARY KEY ASC AUTOINCREMENT,"
                    "PersonName        VCHAR (32),"
                    "Sex               INTEGER,"
                    "Age               INTEGER,"
                    "Tel               VCHAR,"
                    "QQ                VCHAR,"
                    "Email             VCHAR,"
                    "Industry          VCHAR,"
                    "Occupation        VCHAR,"
                    "Address           VCHAR,"
                    "BirthYear         INTEGER,"
                    "BirthMonth        INTEGER,"
                    "BirthDay          INTEGER,"
                    "RecentUpdateYear  INTEGER,"
                    "RecentUpdateMonth INTEGER,"
                    "RecentUpdateDay   INTEGER,"
                    "UserId            INTEGER);:\n"

                    "create table if not exists MailDetailInfo ("
                    "MailInfoId   INTEGER PRIMARY KEY ASC AUTOINCREMENT,"
                    "NickName     VCHAR,"
                    "Hobby        VCHAR,"
                    "Advantage    VCHAR,"
                    "ShortComming VCHAR,"
                    "MajorEvent   VCHAR,"
                    "MailId       INTEGER);:\n"


                    "insert into MailTable(PersonName,Sex,Age,Tel,QQ,Email,Industry,Occupation,Address,BirthYear,BirthMonth,BirthDay,RecentUpdateYear,RecentUpdateMonth,RecentUpdateDay,UserId)"
                    "Select '封俊',1,22,19173489702,3234210243,'15575423419@163.com','IT','C++软件开发','麒麟科技园12楼',2000,8,23,2023,1,16,0 where not exists(select 1 from MailTable where MailId = '1');:\n"

                    "insert into MailDetailInfo(NickName,Hobby,Advantage,ShortComming,MajorEvent,MailId)"
                    "Select '杰瑞','吉他','优秀','糟糕','一事无成',1 where not exists(select 1 from MailDetailInfo where MailInfoId = '1')";
     return sql;
 }
 bool DataBaseObj::checkTableIsExsit(QSqlDatabase & conobj,QSqlQuery & query)     //检测表格是否存在 不存在则建表
 {
     QString sqlstr;
     QFile file_sql(android_local_sql_dir);

     if(file_sql.open(QIODevice::WriteOnly)){
         sqlstr = BuildSql();
         file_sql.write(sqlstr.toStdString().c_str());
         LOG("find and built it Write sql:%s",sqlstr.toStdString().c_str());
     }
     file_sql.close();

     QFile file(android_local_sql_dir);
     if(!file.open(QIODevice::ReadWrite)){
         qDebug() << "the dbsql.txt is not exsit,build it!" << endl;
     }
     else
     {
         QByteArray sqldata = file.readAll();
         sqlstr = QString::fromLocal8Bit(sqldata);
         QStringList sqllist = sqlstr.split(":");

         bool query_ret = false;

         for(QString str:sqllist){
             LOG("sql:%s",str.toStdString().c_str());
             query.prepare(str);
             query_ret = query.exec();
             query.clear();
             if(!query_ret){
                 LOG("error:%s",conobj.lastError().text().toStdString().c_str());
                 LOG("query_result:%d",query_ret);
                 m_Self->my_ConnObj.rollback();
                 break;
             }
             LOG("query_result:%d",query_ret);
         }

     }

     file.close();

     return true;
 }
//释放清理数据库对象
bool DataBaseObj::destoryDb()
{


}

