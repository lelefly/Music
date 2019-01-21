package com.le.music;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import com.le.myplayer.Demo;

public class MainActivity extends AppCompatActivity {
  Demo demo;

  // Used to load the 'native-lib' library on application startup.

  @Override protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    demo = new Demo();
    //// Example of a call to a native method
    //TextView tv = (TextView) findViewById(R.id.sample_text);
    //tv.setText(demo.stringFromJNI());
    demo.testFFmpeg();
  }
}
