#ifndef CLOG_H
#define CLOG_H

#include <android/log.h>


#define LOG_TAG "QtDebug"
//#define LOGW(str)  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,str)
#define LOG(...)  __android_log_print(ANDROID_LOG_DEBUG ,LOG_TAG, __VA_ARGS__)

#endif // CLOG_H
