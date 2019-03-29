package com.le.myplayer.player;

import android.text.TextUtils;
import com.le.myplayer.WlTimeInfoBean;
import com.le.myplayer.listener.WLOnPrepareListener;
import com.le.myplayer.listener.WlOnLoadListener;
import com.le.myplayer.listener.WlOnPauseResumeListener;
import com.le.myplayer.listener.WlOnTimeInfoListener;
import com.le.myplayer.log.MyLog;

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
  private WlOnLoadListener onLoadListener;
  private WlOnPauseResumeListener onPauseResumeListener;
  private WlOnTimeInfoListener onTimeInfoListener;
  private static WlTimeInfoBean wlTimeInfoBean;

  public WLPlayer() {
  }

  public void setSource(String source) {
    this.source = source;
  }

  public void setOnPrepareListener(WLOnPrepareListener onPrepareListener) {
    this.onPrepareListener = onPrepareListener;
  }

  public void setOnLoadListener(WlOnLoadListener onLoadListener) {
    this.onLoadListener = onLoadListener;
  }

  public void setOnPauseResumeListener(WlOnPauseResumeListener onPauseResumeListener) {
    this.onPauseResumeListener = onPauseResumeListener;
  }

  public void setOnTimeInfoListener(WlOnTimeInfoListener onTimeInfoListener) {
    this.onTimeInfoListener = onTimeInfoListener;
  }

  public void prepared() {
    if (TextUtils.isEmpty(source)) {
      MyLog.d("empty");
      return;
    }
    onCallLoad(true);
    new Thread(new Runnable() {
      @Override public void run() {
        n_prepared(source);
      }
    }).start();
  }

  public void start() {
    if (TextUtils.isEmpty(source)) {
      MyLog.d("source empty");
      return;
    }
    new Thread(new Runnable() {
      @Override public void run() {
        n_start();
      }
    }).start();
  }

  public void pause() {
    n_pause();
    if (onPauseResumeListener != null) {
      onPauseResumeListener.onPause(true);
    }
  }

  public void resume() {
    n_resume();
    if (onPauseResumeListener != null) {
      onPauseResumeListener.onPause(false);
    }
  }

  public void stop() {
    new Thread(new Runnable() {
      @Override public void run() {
        n_stop();
      }
    }).start();
  }

  public void onCallPrepared() {
    if (onPrepareListener != null) {
      onPrepareListener.onPrepared();
    }
  }

  public void onCallLoad(boolean load) {
    if (onLoadListener != null) {
      onLoadListener.onLoad(load);
    }
  }

  public void onCallTimeInfo(int currentTime, int totalTime) {
    if (onTimeInfoListener != null) {
      if (wlTimeInfoBean == null) {
        wlTimeInfoBean = new WlTimeInfoBean();
      }
      wlTimeInfoBean.setCurrentTime(currentTime);
      wlTimeInfoBean.setTotalTime(totalTime);
      onTimeInfoListener.onTimeInfo(wlTimeInfoBean);
    }
  }

  private native void n_prepared(String source);

  private native void n_start();

  private native void n_pause();

  private native void n_resume();

  private native void n_stop();
}
