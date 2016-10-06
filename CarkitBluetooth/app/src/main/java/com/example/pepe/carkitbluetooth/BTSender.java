package com.example.pepe.carkitbluetooth;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.Toast;

import java.io.IOException;
import java.util.UUID;

/**
 * Created by Pepe on 6.10.2016.
 */

public class BTSender extends AsyncTask<Void, Void, Void> {
    private static int mSpeed, mDir;
    private Context mC;
    BluetoothAdapter btAdapter = null;
    BluetoothSocket btSocket = null;
    private static boolean isConnected = true, isConnecting = false;
    String address = "98:D3:31:FD:23:35";
    UUID myUUID = UUID.fromString("51a4c565-1b49-4a3e-8d41-6dae973fb5a1");

    public boolean isConnected(){return isConnected;}
    public boolean isConnecting(){return isConnecting;}
    public BTSender(int speed, int dir, Context c){
        mSpeed = speed; mDir = dir; mC = c;
    }
    @Override
    protected void onPreExecute(){
        (Toast.makeText(mC, "Connecting...", Toast.LENGTH_SHORT)).show();
        isConnecting = true;
    }
    @Override
    protected Void doInBackground(Void... devices) {
        try {
            if(btSocket == null || !isConnected){
                btAdapter = BluetoothAdapter.getDefaultAdapter();
                BluetoothDevice btReceiver = btAdapter.getRemoteDevice(address);
                btSocket = btReceiver.createInsecureRfcommSocketToServiceRecord(myUUID);
                btAdapter.getDefaultAdapter().cancelDiscovery();
                btSocket.connect();
                isConnected = true;
            }
        } catch (IOException e) {
            isConnected = false;
        }
        return null;
    }
    @Override
    protected void onPostExecute(Void result){
        super.onPostExecute(result);
        //isConnected = true;
        (Toast.makeText(mC, "Connected!", Toast.LENGTH_SHORT)).show();
    }
}
