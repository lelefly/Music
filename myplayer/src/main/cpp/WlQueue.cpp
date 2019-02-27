//
// Created by liuxiaole on 2019/2/12.
//

#include "WlQueue.h"

WlQueue::WlQueue(WlPlaystatus *wlPlaystatus) {
    this->playstatus = wlPlaystatus;
    pthread_mutex_init(&mutexPacket, NULL);
    pthread_cond_init(&condPacket, NULL);
}

WlQueue::~WlQueue() {
    pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);
}

int WlQueue::putAvpacket(AVPacket *packet) {
    pthread_mutex_lock(&mutexPacket);
    queuePacket.push(packet);
    if (LOG_DEBUG) {
        LOGD("放入 avpacket 到队列 ，个数 %d", queuePacket.size())
    }
    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int WlQueue::getAvpacket(AVPacket *packet) {
    pthread_mutex_lock(&mutexPacket);

    while (playstatus != NULL && !playstatus->exit) {
        if (queuePacket.size() > 0) {
            AVPacket *avPacket = queuePacket.front();
            if (av_packet_ref(packet, avPacket) == 0) {
                queuePacket.pop();
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            if (LOG_DEBUG) {
                LOGD("取出Avpacket 还剩 %d", queuePacket.size());
            }
            break;
        } else {
            LOGE("wait");
            pthread_cond_wait(&condPacket, &mutexPacket);
        }
    }
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int WlQueue::getQueueSize() {
    int size = 0;
    pthread_mutex_lock(&mutexPacket);
    size = queuePacket.size();
    pthread_mutex_unlock(&mutexPacket);
    return size;
}
