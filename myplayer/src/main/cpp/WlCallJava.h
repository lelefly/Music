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

    jmethodID  jmid_prepared;

public:
    WlCallJava(JavaVM *javaVM,JNIEnv *env,jobject *obj);
    ~WlCallJava();

    void onCallPrepared(int type);
};


#endif //MUSIC_WLCALLJAVA_H
