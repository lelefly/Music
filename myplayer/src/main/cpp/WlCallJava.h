//
// Created by liuxiaole on 2019/2/11.
//

#ifndef MUSIC_WLCALLJAVA_H
#define MUSIC_WLCALLJAVA_H

#define MAIN_THREAD 0
#define CHILD_THREAD 1


#include <jni.h>
#include <cwchar>
#include "AndroidLog.h"

class WlCallJava {

public:
    JavaVM *javaVM = NULL;
    JNIEnv *jniENV = NULL;
    jobject jobj;

    jmethodID jmid_prepared;
    jmethodID jmid_load;
    jmethodID jmid_timeinfo;
    jmethodID jmid_error;

public:
    WlCallJava(JavaVM *javaVM, JNIEnv *env, jobject *obj);

    ~WlCallJava();

    void onCallPrepared(int type);

    void onCallLoad(int type, bool load);

    void onCallTimeInfo(int type, int curr, int total);

    void onCallError(int type, int code, char *msg);
};


#endif //MUSIC_WLCALLJAVA_H
