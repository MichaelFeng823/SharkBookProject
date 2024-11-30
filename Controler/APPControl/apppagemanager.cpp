#include "apppagemanager.h"
#include "Kit/LogInfo/clog.h"
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include "Controler/GlobalInfo/GlobalDocumentPath.h"
#include <QtAndroid>
#include <QFile>
#include <QSplashScreen>
#include <QPixmap>

using namespace ReadWriteIni;
using namespace DocPath;
using namespace ScreenFunc;
using namespace SqlFunc;
AppPageManager * AppPageManager::self = nullptr;
QMutex AppPageManager::my_Mutex;
AppPageManager::AppPageManager(QObject *parent) : QObject(parent)
{
    SYSTEMSTATE::NETWORKSTATE = requestPermission("android.permission.INTERNET");
    hasConnected = DataBaseObj::getConnecttion(coon);
    if (hasConnected) {
        QFile file(Android_Local_Userini_dir);

        if(!file.exists()||file.size() == 0){
            QString infostr = QString::fromLocal8Bit("File is not exsit,bulit it! and path:%1").arg(Android_Local_Userini_dir);
            LOG("%s",infostr.toStdString().c_str());
            file.copy("assets:/user.ini",Android_Local_Userini_dir);      //拷到安卓本地目录
            file.setPermissions(QFile::WriteOwner | QFile::ReadOwner); //以读写的方式进行拷贝
            file.close();
        }
    }
}

//获取用户单例对象
AppPageManager * AppPageManager::instance()
{
    if(!self){
        QMutexLocker locker(&my_Mutex);
        self = new AppPageManager;
    }
    return self;
}
//请求系统权限
bool AppPageManager::requestPermission(QString permission)
{
    QtAndroid::PermissionResult r = QtAndroid::checkPermission(permission);
        if(r == QtAndroid::PermissionResult::Denied) {
            QtAndroid::requestPermissionsSync( QStringList() << permission );
            r = QtAndroid::checkPermission(permission);
            if(r == QtAndroid::PermissionResult::Denied) {
                LOG("Request  %s Fail",permission.toStdString().c_str());
                 return false;
            }
       }
       LOG("Request %s Success",permission.toStdString().c_str());
       return true;
}
//初始化界面
int AppPageManager::initPage()
{
     //每次启动应用 首先是应用启动动画（广告）或 读取本地存储的上次登录的用户信息 是否记住密码和自动登录
         //如果是是记住密码 且 自动登录 那么连接数据库 验证用户密码 通过 则直接 跳到 主界面
         //如果没记住密码 或 没勾选自动登录 则跳转到登录界面
     QPixmap pixmap(":/login/image/Login.jpg");
     QSplashScreen spalsh(pixmap);
     spalsh.show();
     if (preLogin()) {
         mainarea = new MainArea();
         connect(mainarea,&MainArea::sendSignalToApppagemanager,this,&AppPageManager::receiveSignal);
         mainarea->raise();
         mainarea->show();
         spalsh.finish(mainarea);
         return QApplication::exec();
     } else {
         login = new LoginForm;
         login->resize(getScreenSize());
         connect(login,&LoginForm::sendSignalToAppPageManager,this,&AppPageManager::receiveSignal);
         spalsh.finish(login);

         if(getUserInfoAfterSuccessLogin())
         {
             LOG("Arrive apppagemanager.cpp %d line",__LINE__);
             return QApplication::exec();
         }
         LOG("退出登录");
         return 0;
     }
}

bool AppPageManager::preLogin()
{
    //读取用户信息
    QString userName;
    QString userPassword;
    QString rememberPassword;
    QString autoLogin;

    readIni(Android_Local_Userini_dir,"config","username",userName);
    readIni(Android_Local_Userini_dir, "config","password",userPassword);
    readIni(Android_Local_Userini_dir,"config", "remember_states",rememberPassword);
    readIni(Android_Local_Userini_dir, "config","auto_states",autoLogin);
    if (rememberPassword == "1" && autoLogin == "1") {
        QSqlQuery query(coon);
        //登录检查
        int u_Id = loginCheck(query,userName,userPassword);
        if(u_Id >= 0)
        {
            LOG("Success Login! and UserID:%d,UserName:%s,UserPassword:%s",u_Id,userName.toStdString().c_str(),userPassword.toStdString().c_str());

            UserInfo::UserID = u_Id;
            UserInfo::UserName = userName;
            UserInfo::UserPassWord = userPassword;
            UserInfo::RemmberPassState = rememberPassword;
            UserInfo::AutoLogin = autoLogin;
            return true;
        }
        else
        {
            LOG("Error:%s",this->coon.lastError().text().toStdString().c_str());
            LOG("用户名或者密码错误！");

        }
    }
    return false;
}

//获取用户信息在成功登录后
bool AppPageManager::getUserInfoAfterSuccessLogin()
{
    if(login != nullptr){
        if(login->exec() == QDialog::Accepted){
            LOG("return true");
            login = nullptr;
            return true;

        }
    }
    return false;
}
//页面管理类 收到信号 字符串格式 "close *** page->open *** page"
void AppPageManager::receiveSignal(QString str)
{
    LOG("AppPageManager recieve signal message!\n content is :%s",str.toStdString().c_str());
    QString openStr;
    QString closeStr;
    QStringList strList;
    if(!str.isEmpty()){
        strList = str.split("->");
    }

    if(strList.size() >= 2){
        closeStr = strList[0];
        openStr = strList[1];
        LOG("closeStr:%s,openStr:%s",closeStr.toStdString().c_str(),openStr.toStdString().c_str());
    }
    if(closeStr == "MainArea"){
        if(mainarea != nullptr)
        {
            mainarea->close();
            delete mainarea;
            mainarea = nullptr;
        }
    }
    if(openStr == "LoginForm"){
        if(login == nullptr)
        {
            LOG("arrive new LoginForm! line 107");
            login = new LoginForm;
            login->resize(getScreenSize());
            connect(login,&LoginForm::sendSignalToAppPageManager,this,&AppPageManager::receiveSignal);
            getUserInfoAfterSuccessLogin();
        }
    }
    if(openStr == "MainArea"){
        if(mainarea == nullptr)
        {
            LOG("Prepare new build class MainArea");
            mainarea = new MainArea();
            connect(mainarea,&MainArea::sendSignalToApppagemanager,this,&AppPageManager::receiveSignal);
            mainarea->raise();
            mainarea->show();
            LOG("build MainArea Finished");
        };
    }

    if(closeStr == "LoginForm"){
        if(login != nullptr)
        {
            LOG("Prepare close LoginForm!");
            login->accept();
        }
    }
    QCoreApplication::processEvents();
}
