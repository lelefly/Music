//
// Created by liuxiaole on 2019/2/11.
//

#ifndef MUSIC_WLAUDIO_H
#define MUSIC_WLAUDIO_H

#include "WlQueue.h"
#include "WlPlaystatus.h"
#include "WlCallJava.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
};

class WlAudio {
public:

    int streamIndex = -1;
    AVCodecParameters *codePar = NULL;
    AVCodecContext *codecContext = NULL;
    WlQueue *queue = NULL;
    WlPlaystatus *playstatus = NULL;
    WlCallJava *callJava = NULL;

    pthread_t thread_play;
    AVPacket *avPacket;

    int ret = -1;
    AVFrame *avFrame = NULL;

    uint8_t *buffer = NULL;
    int data_size = 0;
    int sample_rate = 0;
    int duration = 0;
    AVRational time_base;
    double now_time = 0;
    double clock = 0;
    double last_time = 0;

    // 引擎接口
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine = NULL;

    //混音器
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    //pcm
    SLObjectItf pcmPlayerObject = NULL;
    SLPlayItf pcmPlayerPlay = NULL;

    //缓冲器队列接口
    SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;

public:
    WlAudio(WlPlaystatus *playstatus, int sample_rate, WlCallJava *callJava);

    ~WlAudio();

    void play();

    int resampleAudio();

    void initOpenSLES();

    int getCurrentSampleRateForOpensles(int sample_rate);

    void pause();

    void resume();

    void stop();

    void release();
};


#endif //MUSIC_WLAUDIO_H
