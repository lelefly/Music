//
// Created by liuxiaole on 2019/2/11.
//

#ifndef MUSIC_WLFFMPEG_H
#define MUSIC_WLFFMPEG_H

#include "WlCallJava.h"
#include "pthread.h"
#include "WlAudio.h"

extern "C"
{
#include <libavformat/avformat.h>
};

class WlFFmpeg {

public:
    WlCallJava *callJava = NULL;
    const char* url = NULL;
    pthread_t  decodeThread;
    AVFormatContext *pFormatContext;
    WlAudio *audio = NULL;
    WlPlaystatus *playstatus = NULL;

public:
    WlFFmpeg(WlPlaystatus *playstatus1,WlCallJava *callJava, const char *url);
    ~WlFFmpeg();

    void prepared();

    void decodeFFmpegThread();

    void start();
};


#endif //MUSIC_WLFFMPEG_H
