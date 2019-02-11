package com.le.myplayer.player;

import android.text.TextUtils;
import com.le.myplayer.log.MyLog;
import com.le.myplayer.player.listener.WLOnPrepareListener;

/**
 * Created by liuxiaole on 2019/2/11.
 */
public class WLPlayer {
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

  private String source;

  private WLOnPrepareListener onPrepareListener;

  public WLPlayer() {
  }

  public void setSource(String source) {
    this.source = source;
  }

  public void setOnPrepareListener(WLOnPrepareListener onPrepareListener) {
    this.onPrepareListener = onPrepareListener;
  }

  public void prepared() {
    if (TextUtils.isEmpty(source)) {
      MyLog.d("empty");
      return;
    }
    new Thread(new Runnable() {
      @Override public void run() {
        n_prepared(source);
      }
    }).start();
  }

  public void onCallPrepared() {
    if (onPrepareListener != null) {
      onPrepareListener.onPrepared();
    }
  }

  public native void n_prepared(String source);
}
