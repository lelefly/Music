package com.le.music;

import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import com.le.myplayer.WlTimeInfoBean;
import com.le.myplayer.listener.WlOnErrorlistener;
import com.le.myplayer.listener.WlOnTimeInfoListener;
import com.le.myplayer.log.MyLog;
import com.le.myplayer.player.WLPlayer;
import com.le.myplayer.listener.WLOnPrepareListener;
import com.le.myplayer.listener.WlOnLoadListener;
import com.le.myplayer.listener.WlOnPauseResumeListener;

public class MainActivity extends AppCompatActivity {

  private WLPlayer wlPlayer;

  private TextView tv_time;

  // Used to load the 'native-lib' library on application startup.

  @Override protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    tv_time = findViewById(R.id.tv_time);
    wlPlayer = new WLPlayer();
    wlPlayer.setOnPrepareListener(new WLOnPrepareListener() {
      @Override public void onPrepared() {
        MyLog.d("onPrepared");
        wlPlayer.start();
      }
    });
    wlPlayer.setOnLoadListener(new WlOnLoadListener() {
      @Override public void onLoad(boolean load) {
        if (load) {
          MyLog.d("loading");
        } else {
          MyLog.d("playing");
        }
      }
    });
    wlPlayer.setOnPauseResumeListener(new WlOnPauseResumeListener() {
      @Override public void onPause(boolean pause) {
        if (pause) {
          MyLog.d("暂停");
        } else {
          MyLog.d("播放");
        }
      }
    });
    wlPlayer.setOnTimeInfoListener(new WlOnTimeInfoListener() {
      @Override public void onTimeInfo(WlTimeInfoBean timeInfoBean) {
        //MyLog.d(timeInfoBean.toString());
        Message message = Message.obtain();
        message.what = 1;
        message.obj = timeInfoBean;
        handler.sendMessage(message);
      }
    });

    wlPlayer.setOnErrorlistener(new WlOnErrorlistener() {
      @Override public void onError(int code, String msg) {
        MyLog.d(code + "---" + msg);
      }
    });
  }

  public void begin(View view) {
    wlPlayer.setSource("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp31");
    wlPlayer.prepared();
  }

  public void pause(View view) {
    wlPlayer.pause();
  }

  public void resume(View view) {
    wlPlayer.resume();
  }

  public void stop(View view) {
    wlPlayer.stop();
  }

  Handler handler = new Handler() {
    @Override public void handleMessage(Message msg) {
      super.handleMessage(msg);
      switch (msg.what) {
        case 1:
          WlTimeInfoBean infoBean = (WlTimeInfoBean) msg.obj;
          tv_time.setText(infoBean.getCurrentTime() + "/" + infoBean.getTotalTime());
          break;
      }
    }
  };
}
