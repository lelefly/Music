//
// Created by liuxiaole on 2019/2/11.
//

#ifndef MUSIC_WLAUDIO_H
#define MUSIC_WLAUDIO_H

extern "C"
{
#include <libavcodec/avcodec.h>
};

class WlAudio {
public:

    int streamIndex = -1;
    AVCodecParameters *codePar = NULL;
    AVCodecContext *codecContext = NULL;

public:
    WlAudio();
    ~WlAudio();
};


#endif //MUSIC_WLAUDIO_H
