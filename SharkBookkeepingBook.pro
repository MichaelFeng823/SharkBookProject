QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controler/apppagemanager.cpp \
    Controler/databaseobj.cpp \
    Kit/addbudgetpane.cpp \
    Kit/billdataitem.cpp \
    Kit/billdataitemcopy.cpp \
    Kit/billdataitemwithprocessbar.cpp \
    Kit/billdetailitemeditsubpage.cpp \
    Kit/billsubarea.cpp \
    Kit/bookkeepinginputkit.cpp \
    Kit/budgetmasksubpage.cpp \
    Kit/budgetsubpage.cpp \
    Kit/calendarkit.cpp \
    Kit/chinese2pinyin.cpp \
    Kit/inandoutkit.cpp \
    Kit/menuicon.cpp \
    Kit/persondetailinfosubpage.cpp \
    Kit/personinfodisplaykit.cpp \
    Kit/scrollareakit.cpp \
    Util/calendar.cpp \
    bookkeepingselectarea.cpp \
    changepasswordform.cpp \
    chartmaininterface.cpp \
    detailedsonpageinstackwidget.cpp \
    detialtableview.cpp \
    discoverymaininterface.cpp \
    inandoutdetailpage.cpp \
    loginform.cpp \
    main.cpp \
    mainarea.cpp \
    minewidagetinstackwidget.cpp \
    registerwindow.cpp

HEADERS += \
    Controler/GlobalDocumentPath.h \
    Controler/PublicDataStruct.h \
    Controler/PublicDbFunc.h \
    Controler/PublicUerInfo.h \
    Controler/apppagemanager.h \
    Controler/databaseobj.h \
    Kit/addbudgetpane.h \
    Kit/billdataitem.h \
    Kit/billdataitemcopy.h \
    Kit/billdataitemwithprocessbar.h \
    Kit/billdetailitemeditsubpage.h \
    Kit/billsubarea.h \
    Kit/bookkeepinginputkit.h \
    Kit/budgetmasksubpage.h \
    Kit/budgetsubpage.h \
    Kit/calendarkit.h \
    Kit/chinese2pinyin.h \
    Kit/inandoutkit.h \
    Kit/menuicon.h \
    Kit/persondetailinfosubpage.h \
    Kit/personinfodisplaykit.h \
    Kit/scrollareakit.h \
    Util/calendar.h \
    Util/clog.h \
    bookkeepingselectarea.h \
    changepasswordform.h \
    chartmaininterface.h \
    detailedsonpageinstackwidget.h \
    detialtableview.h \
    discoverymaininterface.h \
    inandoutdetailpage.h \
    loginform.h \
    mainarea.h \
    minewidagetinstackwidget.h \
    registerwindow.h

FORMS += \
    Kit/addbudgetpane.ui \
    Kit/billdataitemwithprocessbar.ui \
    Kit/billdetailitemeditsubpage.ui \
    Kit/billsubarea.ui \
    Kit/bookkeepinginputkit.ui \
    Kit/budgetmasksubpage.ui \
    Kit/budgetsubpage.ui \
    Kit/persondetailinfosubpage.ui \
    Kit/personinfodisplaykit.ui \
    bookkeepingselectarea.ui \
    changepasswordform.ui \
    chartmaininterface.ui \
    detailedsonpageinstackwidget.ui \
    discoverymaininterface.ui \
    inandoutdetailpage.ui \
    loginform.ui \
    mainarea.ui \
    minewidagetinstackwidget.ui \
    registerwindow.ui

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
