#include "apppagemanager.h"
#include "Kit/LogInfo/clog.h"
#include "Controler/GlobalInfo/PublicUerInfo.h"
#include "Controler/PublicApi/PublicDbFunc.h"
#include <QtAndroid>
using namespace ScreenFunc;
AppPageManager * AppPageManager::self = nullptr;
QMutex AppPageManager::my_Mutex;
AppPageManager::AppPageManager(QObject *parent) : QObject(parent)
{
    SYSTEMSTATE::NETWORKSTATE = requestPermission("android.permission.INTERNET");
}

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
     login = new LoginForm;
     login->resize(getScreenSize());
     connect(login,&LoginForm::sendSignalToApppaagemanager,this,&AppPageManager::receiveSignal);

     if(getUserInfoAfterSuccessLogin())
     {
         LOG("arrive main.cpp 15line");
         return QApplication::exec();
     }
     else
     {
        LOG("退出登录");
        return 0;
     }
     return 0;
}
//获取用户信息在成功登录后
bool AppPageManager::getUserInfoAfterSuccessLogin()
{
    if(login != nullptr){
        if(login->exec() == QDialog::Accepted){
            login = nullptr;
            return true;
        }
    }
    return false;
}
//页面管理类 收到信号 字符串格式 "close *** page->open *** page"
void AppPageManager::receiveSignal(QString str)
{
    LOG("Apppagemanager recieve signal message!\n content is :%s",str.toStdString().c_str());
    QString openStr;
    QString closeStr;
    QStringList strList;
    if(!str.isEmpty()){
        strList = str.split("->");
    }
    if(strList.size() >= 2){
        closeStr = strList[0];
        openStr = strList[1];
        LOG("arrive 82line");
        LOG("closeStr:%s,openStr:%s",closeStr.toStdString().c_str(),openStr.toStdString().c_str());
    }
    if(closeStr == "MainArea"){
        if(mainarea != nullptr)
        {
            LOG("arrive close mainarea! line 98");
            mainarea->close();
            mainarea = nullptr;
           // delete mainarea;
        }
    }
    else if(closeStr == "LoginForm"){
        if(login != nullptr)
        {
            LOG("arrive close LoginForm! line 91");\
            login->accept();
        }
    }

    if(openStr == "LoginForm"){
        if(login == nullptr)
        {
            LOG("arrive new LoginForm! line 107");
            login = new LoginForm();
            login->resize(getScreenSize());
            connect(login,&LoginForm::sendSignalToApppaagemanager,this,&AppPageManager::receiveSignal);
            getUserInfoAfterSuccessLogin();
        }
    }
    else if(openStr == "MainArea"){
        if(mainarea == nullptr)
        {
            LOG("arrive new MainArea! line 116");
            mainarea = new MainArea();
            connect(mainarea,&MainArea::sendSignalToApppagemanager,this,&AppPageManager::receiveSignal);
            mainarea->raise();
            mainarea->show();
        };
    }

    return ;
}
