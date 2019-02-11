#include <jni.h>
#include <string>
#include "WlCallJava.h"
#include "WlFFmpeg.h"

extern "C"
{
#include <libavformat/avformat.h>
}

JavaVM *javaVM = NULL;
WlCallJava *callJava = NULL;
WlFFmpeg *fFmpeg = NULL;


extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    javaVM = vm;
    JNIEnv *env;
    if(vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK){
        return result;
    }
    return JNI_VERSION_1_4;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_le_myplayer_player_WLPlayer_n_1prepared(JNIEnv *env, jobject instance, jstring source_) {
    const char *source = env->GetStringUTFChars(source_, 0);

    if(fFmpeg == NULL){
        if(callJava == NULL){
            callJava = new WlCallJava(javaVM,env,&instance);
        }
        fFmpeg = new WlFFmpeg(callJava,source);
        fFmpeg->prepared();
    }
//    env->ReleaseStringUTFChars(source_, source);
}