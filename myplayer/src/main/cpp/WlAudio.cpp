//
// Created by liuxiaole on 2019/2/11.
//

#include "WlAudio.h"

WlAudio::WlAudio(WlPlaystatus *playstatus) {
    this->playstatus = playstatus;
    queue = new WlQueue(playstatus);
}

WlAudio::~WlAudio() {}

