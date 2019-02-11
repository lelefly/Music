//
// Created by liuxiaole on 2019/2/11.
//

#include "WlCallJava.h"
#include "AndroidLog.h"

WlCallJava::WlCallJava(JavaVM *javaVM, JNIEnv *env, jobject *obj)  {
    this->javaVM = javaVM;
    this->jniENV = env;
    this->jobj = env->NewGlobalRef(*obj);

    jclass jlz = env->GetObjectClass(jobj);
    if(!jlz){
      if(LOG_DEBUG)  {
          LOGE("get jclass wrong");
      }
        return ;
    }

    this->jmid_prepared = env->GetMethodID(jlz,"onCallPrepared","()V");
}

WlCallJava::~WlCallJava() {

}


void WlCallJava::onCallPrepared(int type) {
    if(type == MAIN_THREAD){
        jniENV->CallVoidMethod(jobj,jmid_prepared);
    }else if(type == CHILD_THREAD){
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv,0) != JNI_OK){
            if(LOG_DEBUG){
                LOGE("get child thread wrong");
                return;
            }
        }
        jniEnv->CallVoidMethod(jobj,jmid_prepared);
        javaVM->DetachCurrentThread();
    }
}
