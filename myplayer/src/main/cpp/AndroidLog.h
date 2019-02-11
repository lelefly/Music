//
// Created by liuxiaole on 2019/2/11.
//

#ifndef MUSIC_ANDROIDLOG_H
#define MUSIC_ANDROIDLOG_H

#endif //MUSIC_ANDROIDLOG_H

#include <android/log.h>

#define LOG_DEBUG true

#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG,"leffmpeg",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"leffmpeg",FORMAT,##__VA_ARGS__);
