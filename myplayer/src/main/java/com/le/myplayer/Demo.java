package com.le.myplayer;

/**
 * Created by liuxiaole on 2019/1/21.
 */
public class Demo {
  static {
    System.loadLibrary("native-lib");
    System.loadLibrary("avutil-55");
    System.loadLibrary("swresample-2");
    System.loadLibrary("avcodec-57");
    System.loadLibrary("avformat-57");
    System.loadLibrary("swscale-4");
    System.loadLibrary("postproc-54");
    System.loadLibrary("avfilter-6");
    System.loadLibrary("avdevice-57");
  }

  public native void testFFmpeg();

  public native void mutexThread();

}
