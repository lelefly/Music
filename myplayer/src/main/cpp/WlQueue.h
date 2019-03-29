//
// Created by liuxiaole on 2019/2/12.
//

#ifndef MUSIC_WLQUEUE_H
#define MUSIC_WLQUEUE_H

extern "C"{
#include <libavcodec/avcodec.h>
};
#include "queue"
#include "pthread.h"
#include "AndroidLog.h"
#include "WlPlaystatus.h"


class WlQueue {

public:
    std::queue<AVPacket *> queuePacket;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;
    WlPlaystatus *playstatus = NULL;

public:
    WlQueue(WlPlaystatus *wlPlaystatus);

    ~WlQueue();

    int putAvpacket(AVPacket *packet);

    int getAvpacket(AVPacket *packet);

    int getQueueSize();

    void clearAvpaket();
};


#endif //MUSIC_WLQUEUE_H
