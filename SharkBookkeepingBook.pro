QT       += core gui sql network
QT       += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

android: include(./android_openssl-master/openssl.pri)

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#TARGET = SharkBookkeepingBook.apk
UI_DIR += ui_source
SOURCES += \
    BaseClass/basecustomwidget.cpp \
    BaseClass/circulardiagram.cpp \
    BaseClass/detialtableview.cpp \
    BaseClass/photolistview.cpp \
    BaseClass/picitem.cpp \
    BaseClass/vibrateringbutton.cpp \
    Controler/APPControl/apppagemanager.cpp \
    Controler/DataControl/databaseobj.cpp \
    Kit/Calendar/SubCtrls/scrollareakit.cpp \
    Kit/Calendar/calendar.cpp \
    Kit/Chinese2PinYin/chinese2pinyin.cpp \
    LoginAbout/ChangePassWord/changepasswordform.cpp \
    LoginAbout/Login/loginform.cpp \
    LoginAbout/Register/registerwindow.cpp \
    Main/BookPage/MainUi/bookkeepingselectarea.cpp \
    Main/BookPage/SubCtrls/bookkeepinginputkit.cpp \
    Main/BookPage/SubCtrls/menuicon.cpp \
    Main/ChartPage/MainUi/chartmaininterface.cpp \
    Main/ChartPage/Other/chartmasksubpage.cpp \
    Main/ChartPage/Other/chartmodel.cpp \
    Main/ChartPage/Other/dotdetaildatawindow.cpp \
    Main/ChartPage/Other/selectscrollbar.cpp \
    Main/DetaiPage/MainUi/SubCtrls/calendarkit.cpp \
    Main/DetaiPage/MainUi/SubCtrls/inandoutkit.cpp \
    Main/DetaiPage/MainUi/detailedsonpageinstackwidget.cpp \
    Main/DetaiPage/SubPage/AssertManage/assetmanagerpage.cpp \
    Main/DetaiPage/SubPage/Bill/SubCtrls/billdataitem.cpp \
    Main/DetaiPage/SubPage/Bill/billsubarea.cpp \
    Main/DetaiPage/SubPage/Budget/MainUi/budgetsubpage.cpp \
    Main/DetaiPage/SubPage/Budget/SubCtrls/budgetsubitem.cpp \
    Main/DetaiPage/SubPage/Budget/SubCtrls/modifybudgetmask.cpp \
    Main/DetaiPage/SubPage/Budget/addbudgetpane.cpp \
    Main/DetaiPage/SubPage/Budget/budgetmasksubpage.cpp \
    Main/DetaiPage/SubPage/Income/MainUi/inandoutdetailpage.cpp \
    Main/DetaiPage/SubPage/Income/SubCtrls/billdataitemwithprocessbar.cpp \
    Main/DetaiPage/SubPage/Income/SubPage/billdetailitemeditsubpage.cpp \
    Main/DetaiPage/SubPage/More/MainUi/morepage.cpp \
    Main/DetaiPage/SubPage/More/SubPage/girlpicwidget.cpp \
    Main/DetaiPage/SubPage/More/SubCtrls/photoframe.cpp \
    Main/DetaiPage/SubPage/Shopping/shoppage.cpp \
    Main/DiscoveryPage/MainUi/discoverymaininterface.cpp \
    Main/DiscoveryPage/SubPage/persondetailinfosubpage.cpp \
    Main/DiscoveryPage/SubPage/personinfodisplaykit.cpp \
    Main/MainFrame/mainarea.cpp \
    Main/MinePage/MainUi/minewidagetinstackwidget.cpp \
    Main/main.cpp \
    NetCore/msgqueue.cpp

HEADERS += \
    BaseClass/basecustomwidget.h \
    BaseClass/circulardiagram.h \
    BaseClass/detialtableview.h \
    BaseClass/photolistview.h \
    BaseClass/picitem.h \
    BaseClass/vibrateringbutton.h \
    Controler/APPControl/apppagemanager.h \
    Controler/DataControl/databaseobj.h \
    Controler/DataStruct/PublicDataStruct.h \
    Controler/GlobalInfo/GlobalDocumentPath.h \
    Controler/GlobalInfo/PublicUerInfo.h \
    Controler/PublicApi/PublicDbFunc.h \
    Kit/Calendar/SubCtrls/scrollareakit.h \
    Kit/Calendar/calendar.h \
    Kit/Chinese2PinYin/chinese2pinyin.h \
    Kit/LogInfo/clog.h \
    LoginAbout/ChangePassWord/changepasswordform.h \
    LoginAbout/Login/loginform.h \
    LoginAbout/Register/registerwindow.h \
    Main/BookPage/MainUi/bookkeepingselectarea.h \
    Main/BookPage/SubCtrls/bookkeepinginputkit.h \
    Main/BookPage/SubCtrls/menuicon.h \
    Main/ChartPage/MainUi/chartmaininterface.h \
    Main/ChartPage/Other/chartmasksubpage.h \
    Main/ChartPage/Other/chartmodel.h \
    Main/ChartPage/Other/dotdetaildatawindow.h \
    Main/ChartPage/Other/selectscrollbar.h \
    Main/DetaiPage/MainUi/SubCtrls/calendarkit.h \
    Main/DetaiPage/MainUi/SubCtrls/inandoutkit.h \
    Main/DetaiPage/MainUi/detailedsonpageinstackwidget.h \
    Main/DetaiPage/SubPage/AssertManage/assetmanagerpage.h \
    Main/DetaiPage/SubPage/Bill/SubCtrls/billdataitem.h \
    Main/DetaiPage/SubPage/Bill/billsubarea.h \
    Main/DetaiPage/SubPage/Budget/MainUi/budgetsubpage.h \
    Main/DetaiPage/SubPage/Budget/SubCtrls/budgetsubitem.h \
    Main/DetaiPage/SubPage/Budget/SubCtrls/modifybudgetmask.h \
    Main/DetaiPage/SubPage/Budget/addbudgetpane.h \
    Main/DetaiPage/SubPage/Budget/budgetmasksubpage.h \
    Main/DetaiPage/SubPage/Income/MainUi/inandoutdetailpage.h \
    Main/DetaiPage/SubPage/Income/SubCtrls/billdataitemwithprocessbar.h \
    Main/DetaiPage/SubPage/Income/SubPage/billdetailitemeditsubpage.h \
    Main/DetaiPage/SubPage/More/MainUi/morepage.h \
    Main/DetaiPage/SubPage/More/SubPage/girlpicwidget.h \
    Main/DetaiPage/SubPage/More/SubCtrls/photoframe.h \
    Main/DetaiPage/SubPage/Shopping/shoppage.h \
    Main/DiscoveryPage/MainUi/discoverymaininterface.h \
    Main/DiscoveryPage/SubPage/persondetailinfosubpage.h \
    Main/DiscoveryPage/SubPage/personinfodisplaykit.h \
    Main/MainFrame/mainarea.h \
    Main/MinePage/MainUi/minewidagetinstackwidget.h \
    NetCore/msgqueue.h

FORMS += \
    LoginAbout/ChangePassWord/changepasswordform.ui \
    LoginAbout/Login/loginform.ui \
    LoginAbout/Register/registerwindow.ui \
    Main/BookPage/MainUi/bookkeepingselectarea.ui \
    Main/BookPage/SubCtrls/bookkeepinginputkit.ui \
    Main/ChartPage/MainUi/chartmaininterface.ui \
    Main/ChartPage/Other/chartmasksubpage.ui \
    Main/DetaiPage/MainUi/detailedsonpageinstackwidget.ui \
    Main/DetaiPage/SubPage/AssertManage/assetmanagerpage.ui \
    Main/DetaiPage/SubPage/Bill/billsubarea.ui \
    Main/DetaiPage/SubPage/Budget/MainUi/budgetsubpage.ui \
    Main/DetaiPage/SubPage/Budget/SubCtrls/budgetsubitem.ui \
    Main/DetaiPage/SubPage/Budget/SubCtrls/modifybudgetmask.ui \
    Main/DetaiPage/SubPage/Budget/addbudgetpane.ui \
    Main/DetaiPage/SubPage/Budget/budgetmasksubpage.ui \
    Main/DetaiPage/SubPage/Income/MainUi/inandoutdetailpage.ui \
    Main/DetaiPage/SubPage/Income/SubCtrls/billdataitemwithprocessbar.ui \
    Main/DetaiPage/SubPage/Income/SubPage/billdetailitemeditsubpage.ui \
    Main/DetaiPage/SubPage/More/MainUi/morepage.ui \
    Main/DetaiPage/SubPage/More/SubPage/girlpicwidget.ui \
    Main/DetaiPage/SubPage/More/SubCtrls/photoframe.ui \
    Main/DetaiPage/SubPage/Shopping/shoppage.ui \
    Main/DiscoveryPage/MainUi/discoverymaininterface.ui \
    Main/DiscoveryPage/SubPage/persondetailinfosubpage.ui \
    Main/DiscoveryPage/SubPage/personinfodisplaykit.ui \
    Main/MainFrame/mainarea.ui \
    Main/MinePage/MainUi/minewidagetinstackwidget.ui

#设计Android数据库
android{
    data.files += source/myDb.db
    data.files += source/user.ini
    data.files += source/MenuSetting.ini
    data.path = /android_sources/assets
    INSTALLS += data
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android-build/android_sources/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-build/android_sources

RESOURCES += \
    Pic.qrc


