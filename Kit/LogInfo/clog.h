#ifndef CLOG_H
#define CLOG_H

#include <android/log.h>


#define LOG_TAG "QtDebug"
//#define LOGW(str)  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,str)
#define LOG(...)  __android_log_print(ANDROID_LOG_DEBUG ,LOG_TAG, __VA_ARGS__)

//------------新添加NEW_OBJECT的宏
#define NEW_OBJECT(pObj, TYPE) \
    if (nullptr == pObj) { pObj = new TYPE();}

//------------新添加Delete_OBJECT的宏
#define DELETE_OBJECT(dObj) \
    if (nullptr != dObj) { delete dObj;dObj = nullptr;}

#endif // CLOG_H
