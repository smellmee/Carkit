package com.example.pepe.carkitbluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.media.Image;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import javax.crypto.Mac;

public class MainActivity extends AppCompatActivity {
    private static int speed;
    private static String dir = "a";
    ImageButton upButt;ImageButton downButt;ImageButton rightButt;ImageButton leftButt;
    SeekBar speedBar;
    TextView speedText;
    Button btButt;
    BluetoothManager btManager;
    private Handler dirHandler;
    private Runnable sendDirection = new Runnable() {
        @Override
        public void run() {
            btManager.write(String.valueOf(dir));
            dirHandler.postDelayed(this, 100);
        }
    };
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        upButt = (ImageButton) findViewById(R.id.upButt);
        downButt = (ImageButton) findViewById(R.id.downButt);
        rightButt = (ImageButton) findViewById(R.id.rightButt);
        leftButt = (ImageButton) findViewById(R.id.leftButt);
        btManager = new BluetoothManager();


        speedBar = (SeekBar)findViewById(R.id.speedSlider);
        speedText = (TextView)findViewById(R.id.speedText);
        speedBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                speedText.setText(String.valueOf(speed));
                speed = seekBar.getProgress();
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
        btButt = (Button)findViewById(R.id.btButt);
        btButt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            }
        });
        setupDirButtons();

    }
    public void setupDirButtons(){
        downButt.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch(motionEvent.getAction()){
                    case MotionEvent.ACTION_DOWN:
                        if(dir != "d"){dir = "d";}
                        dirHandler = new Handler();
                        dirHandler.postDelayed(sendDirection, 100);
                        break;
                    case MotionEvent.ACTION_UP:
                        if(dirHandler == null){return true;}
                        dirHandler.removeCallbacks(sendDirection);
                        dirHandler = null;
                        break;
                }
                return false;
            }
        });
        upButt.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch(motionEvent.getAction()){
                    case MotionEvent.ACTION_DOWN:
                        if(dir != 'u'){dir = 'u';}
                        dirHandler = new Handler();
                        dirHandler.postDelayed(sendDirection, 100);
                        break;
                    case MotionEvent.ACTION_UP:
                        if(dirHandler == null){return true;}
                        dirHandler.removeCallbacks(sendDirection);
                        dirHandler = null;
                        break;
                }
                return false;
            }
        });
        rightButt.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch(motionEvent.getAction()){
                    case MotionEvent.ACTION_DOWN:
                        if(dir != 'r'){dir = 'r';}
                        dirHandler = new Handler();
                        dirHandler.postDelayed(sendDirection, 100);
                        break;
                    case MotionEvent.ACTION_UP:
                        if(dirHandler == null){return true;}
                        dirHandler.removeCallbacks(sendDirection);
                        dirHandler = null;
                        break;
                }
                return false;
            }
        });
        leftButt.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch(motionEvent.getAction()){
                    case MotionEvent.ACTION_DOWN:
                        if(dir != 'l'){dir = 'l';}
                        dirHandler = new Handler();
                        dirHandler.postDelayed(sendDirection, 100);
                        break;
                    case MotionEvent.ACTION_UP:
                        if(dirHandler == null){return true;}
                        dirHandler.removeCallbacks(sendDirection);
                        dirHandler = null;
                        break;
                }
                return false;
            }
        });
    }
}
