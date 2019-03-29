//
// Created by liuxiaole on 2019/2/11.
//
#include "WlFFmpeg.h"

WlFFmpeg::WlFFmpeg(WlPlaystatus *playstatus1, WlCallJava *callJava, const char *url) {
    this->callJava = callJava;
    this->url = url;
    this->playstatus = playstatus1;
    pthread_mutex_init(&init_mutex, NULL);
}

void *decodeFFmpeg(void *data) {
    WlFFmpeg *wlFFmpeg = (WlFFmpeg *) (data);
    wlFFmpeg->decodeFFmpegThread();
    pthread_exit(&wlFFmpeg->decodeThread);
}

void WlFFmpeg::prepared() {
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);
}

int avformat_callback(void *ctx) {
    WlFFmpeg *fFmpeg = static_cast<WlFFmpeg *>(ctx);
    if (fFmpeg->playstatus->exit) {
        return AVERROR_EOF;
    }
    return 0;
}

void WlFFmpeg::decodeFFmpegThread() {
    pthread_mutex_lock(&init_mutex);
    av_register_all();
    avformat_network_init();
    pFormatContext = avformat_alloc_context();

    pFormatContext->interrupt_callback.callback = avformat_callback;
    pFormatContext->interrupt_callback.opaque = this;
    if (avformat_open_input(&pFormatContext, url, NULL, NULL) != 0) {
        if (LOG_DEBUG) {
            LOGD("can not open url :%s", url);
            callJava->onCallError(CHILD_THREAD, 1001, "can not open url");
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }
    if (avformat_find_stream_info(pFormatContext, NULL) < 0) {
        if (LOG_DEBUG) {
            LOGD("can not find stream from url :%s", url);
            callJava->onCallError(CHILD_THREAD, 1002, "can not find stream from url");
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }

    for (int i = 0; i < pFormatContext->nb_streams; i++) {
        if (pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (audio == NULL) {
                audio = new WlAudio(playstatus, pFormatContext->streams[i]->codecpar->sample_rate,
                                    callJava);
                audio->streamIndex = i;
                audio->codePar = pFormatContext->streams[i]->codecpar;
                audio->duration = pFormatContext->duration / AV_TIME_BASE;
                audio->time_base = pFormatContext->streams[i]->time_base;
            }
        }
    }

    AVCodec *dec = avcodec_find_decoder(audio->codePar->codec_id);
    if (!dec) {
        if (LOG_DEBUG) {
            LOGE("can not find decoder");
            callJava->onCallError(CHILD_THREAD, 1003, "can not find decoder");
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }

    audio->codecContext = avcodec_alloc_context3(dec);
    if (!audio->codecContext) {
        if (LOG_DEBUG) {
            LOGE("can not alloc new decoderctx")
            callJava->onCallError(CHILD_THREAD, 1004, "can not alloc new decoderctx");
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }
    if (avcodec_parameters_to_context(audio->codecContext, audio->codePar) < 0) {
        if (LOG_DEBUG) {
            LOGE("can not fill decoderctx");
            callJava->onCallError(CHILD_THREAD, 1005, "can not fill decoderctx");
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }

    if (avcodec_open2(audio->codecContext, dec, 0) != 0) {
        if (LOG_DEBUG) {
            LOGE("can not open audio stream");
            callJava->onCallError(CHILD_THREAD, 1006, "can not open audio stream");
        }
        exit = true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }
    callJava->onCallPrepared(CHILD_THREAD);
    pthread_mutex_unlock(&init_mutex);
}

void WlFFmpeg::start() {
    if (audio == NULL) {
        if (LOG_DEBUG) {
            callJava->onCallError(CHILD_THREAD, 1007, "audio is null");
            LOGE("audio is null");
        }
        return;
    }
    audio->play();
    int count = 0;
    while (playstatus != NULL && !playstatus->exit) {
        AVPacket *avPacket = av_packet_alloc();
        if (av_read_frame(pFormatContext, avPacket) == 0) {
            if (avPacket->stream_index == audio->streamIndex) {
                count++;
                audio->queue->putAvpacket(avPacket);
            } else {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
            }
        } else {
            if (LOG_DEBUG) {
                LOGE("decode finished");
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            while (playstatus != NULL && !playstatus->exit) {
                if (audio->queue->getQueueSize() > 0) {
                    continue;
                } else {
                    playstatus->exit = true;
                    break;
                }
            }
        }
    }
    exit = true;
    if (LOG_DEBUG) {
        LOGD("解码完成");
    }
}

void WlFFmpeg::pause() {
    if (audio != NULL) {
        audio->pause();
    }
}

void WlFFmpeg::resume() {
    if (audio != NULL) {
        audio->resume();
    }
}

void WlFFmpeg::release() {

    if (playstatus->exit) {
        return;
    }
    playstatus->exit = true;

    pthread_mutex_lock(&init_mutex);

    int sleepCount = 0;
    while (!exit) {
        if (sleepCount > 1000) {
            exit = true;
        }
        if (LOG_DEBUG) {
            LOGE("wait ffmpeg exit %d", sleepCount);
        }
        sleepCount++;
        av_usleep(10 * 1000);
    }

    if (audio != NULL) {
        audio->release();
        delete (audio);
        audio = NULL;
    }

    if (pFormatContext != NULL) {
        avformat_close_input(&pFormatContext);
        avformat_free_context(pFormatContext);
        pFormatContext = NULL;

        if (playstatus != NULL) {
            playstatus = NULL;
        }

        if (callJava != NULL) {
            callJava = NULL;
        }

    }

    pthread_mutex_unlock(&init_mutex);

}

WlFFmpeg::~WlFFmpeg() {

}
