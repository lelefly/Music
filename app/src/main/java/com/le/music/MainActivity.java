package com.le.music;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import com.le.myplayer.Demo;
import com.le.myplayer.log.MyLog;
import com.le.myplayer.player.WLPlayer;
import com.le.myplayer.player.listener.WLOnPrepareListener;

public class MainActivity extends AppCompatActivity {

  private WLPlayer wlPlayer;

  // Used to load the 'native-lib' library on application startup.

  @Override protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    wlPlayer = new WLPlayer();
    wlPlayer.setOnPrepareListener(new WLOnPrepareListener() {
      @Override public void onPrepared() {
        MyLog.d("onPrepared");
      }
    });
  }

  public void begin(View view) {
    wlPlayer.setSource("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3");
    wlPlayer.prepared();
  }
}
