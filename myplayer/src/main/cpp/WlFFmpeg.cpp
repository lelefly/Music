//
// Created by liuxiaole on 2019/2/11.
//
#include "WlFFmpeg.h"

WlFFmpeg::WlFFmpeg(WlCallJava *callJava, const char *url) {
    this->callJava = callJava;
    this->url = url;
}

void *decodeFFmpeg(void *data) {
    WlFFmpeg *wlFFmpeg = (WlFFmpeg *) (data);
    wlFFmpeg->decodeFFmpegThread();
    pthread_exit(&wlFFmpeg->decodeThread);
}

void WlFFmpeg::prepared() {
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);
}

void WlFFmpeg::decodeFFmpegThread() {
    av_register_all();
    avformat_network_init();
    pFormatContext = avformat_alloc_context();
    if (avformat_open_input(&pFormatContext, url, NULL, NULL) != 0) {
        if (LOG_DEBUG) {
            LOGD("can not open url :%s", url);
            return;
        }
    }
    if (avformat_find_stream_info(pFormatContext, NULL) < 0) {
        if (LOG_DEBUG) {
            LOGD("can not find stream from url :%s", url);
            return;
        }
    }

    for (int i = 0; i < pFormatContext->nb_streams; i++) {
        if(pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){
            if(audio == NULL){
                audio = new WlAudio();
                audio->streamIndex = i;
                audio->codePar = pFormatContext->streams[i]->codecpar;
            }
        }
    }

    AVCodec *dec = avcodec_find_decoder(audio->codePar->codec_id);
    if(!dec){
        if(LOG_DEBUG){
            LOGE("can not find decoder");
        }
        return;
    }

    audio->codecContext = avcodec_alloc_context3(dec);
    if(!audio->codecContext){
        if(LOG_DEBUG){
            LOGE("can not alloc new decoderctx")
        }
        return;
    }
    if(avcodec_parameters_to_context(audio->codecContext,audio->codePar) < 0){
        if(LOG_DEBUG){
            LOGE("can not fill decoderctx");
        }
        return;
    }

    if(avcodec_open2(audio->codecContext,dec,0) != 0){
        if(LOG_DEBUG){
            LOGE("can not open audio stream");
        }
        return;
    }

    callJava->onCallPrepared(CHILD_THREAD);
}
