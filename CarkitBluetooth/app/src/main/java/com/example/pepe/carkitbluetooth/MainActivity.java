package com.example.pepe.carkitbluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.media.Image;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
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
    private static int dir, speed; //0 down 1 right 2 up 3 left
    List<ImageView> buttonArray = new ArrayList<ImageView>();
    ImageView upButt;ImageView downButt;ImageView rightButt;ImageView leftButt;
    SeekBar speedBar;
    TextView speedText;
    Button btButt;
    BluetoothDevice btReceiver;
    String address = "51a4c565-1b49-4a3e-8d41-6dae973fb5a1";
    private BluetoothAdapter btAdapter = null;
    private Set<BluetoothDevice> pairedDevices;
    private BTSender btSender;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        upButt = (ImageView)findViewById(R.id.upButt);
        downButt = (ImageView)findViewById(R.id.downButt);
        rightButt = (ImageView)findViewById(R.id.rightButt);
        leftButt = (ImageView)findViewById(R.id.leftButt);
        //buttonArray.add(upButt);buttonArray.add(rightButt);buttonArray.add(downButt);buttonArray.add(leftButt);
        upButt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dir = 2;
                turn();
            }
        });
        downButt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dir = 0;
                turn();
            }
        });
        rightButt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dir = 1;
                turn();
            }
        });
        leftButt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dir = 2;
                turn();
            }
        });
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
                getpairedDevice();
                btSender = new BTSender(speed, dir, getBaseContext());
                btSender.execute();}
        });
        startBT();
    }
    public void turn(){
        try{
            if(btSender.isConnected()){
                btSender.btSocket.getOutputStream().write("A".getBytes());
            }else {
                (Toast.makeText(getBaseContext(), "Still connecting", Toast.LENGTH_SHORT)).show();
            }
        }catch(IOException e){
            Log.d("", "IOEXC");
        }catch(NullPointerException e){
            (Toast.makeText(getBaseContext(), "Connect to BT first", Toast.LENGTH_SHORT)).show();
        }
    }
    public void startBT(){
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        if(btAdapter == null){
            (Toast.makeText(getBaseContext(), "No BT device found", Toast.LENGTH_SHORT)).show();
        }else{
            if(btAdapter.isEnabled()){}
            else{
                Intent turnBTOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(turnBTOn, 1);
            }
        }
    }
    public void getpairedDevice(){
        pairedDevices = btAdapter.getBondedDevices();
        ArrayList bondedList = new ArrayList();
        if(pairedDevices.size() > 0){
            for(BluetoothDevice a : pairedDevices){
                if(a.getAddress().equals(address)){
                    btReceiver = a;
                }
            }
        }else{
            (Toast.makeText(getBaseContext(), "Pair to a device", Toast.LENGTH_SHORT)).show();
        }
    }
}
