package com.example.pepe.carkitbluetooth;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

/**
 * Created by Pepe on 6.10.2016.
 */

public class BluetoothThread extends Thread {
    private final BluetoothSocket socket;
    private InputStream inStream;
    private OutputStream outStream;

    public BluetoothThread(BluetoothDevice device){
        BluetoothSocket tmp = null;
        try{
            tmp = device.createInsecureRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"));
        }catch(IOException e){
            e.printStackTrace();
        }
        socket = tmp;
    }
    public void run(){
        try{
            socket.connect();
        }catch(IOException e){
            e.printStackTrace();
            try{
                socket.close();
            }catch(IOException x){
                x.printStackTrace();
            }
            return;
        }
        try{
            inStream = socket.getInputStream();
            outStream = socket.getOutputStream();
        }catch(IOException e){
            e.printStackTrace();
            return;
        }
    }
    public void write(String out){
        if(outStream == null){
        }else{
            try{
                if(out != null){
                    outStream.write(out.getBytes());
                }
                else{
                    outStream.write(0);
                }
                outStream.write('\n');
            }catch(IOException e){
                e.printStackTrace();
            }
        }
    }
    public void cancel(){
        try{
            if(inStream != null){
                inStream.close();
            }
            if(outStream != null){
                outStream.close();
            }
            if(socket != null){
                socket.close();
            }
        }catch(IOException e){
            e.printStackTrace();
        }
    }
}
