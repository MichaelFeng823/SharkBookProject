#ifndef GLOBALDOCUMENTPATH_H
#define GLOBALDOCUMENTPATH_H
#pragma once
#include<QString>
namespace DocPath {
     #if 0
     const QString Android_Local_Userini_dir = "./user.ini";
     #else
     const QString Android_Local_Userini_dir = "./mydata/user.ini";
     #endif

     #if 1
     const QString android_local_db_dir = "./myDb.db";
     #else
     const QString android_local_db_dir = "/storage/emulated/0/myDb.db";
     #endif

     const QString android_local_sql_dir = "./sqllist.txt";

     const QString android_local_menuproperty_dir = "./MenuSetting.ini";
}

#endif // GLOBALDOCUMENTPATH_H
