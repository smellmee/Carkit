package com.example.pepe.carkitbluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.io.IOException;
import java.util.UUID;

/**
 * Created by Pepe on 6.10.2016.
 */

public class BluetoothManager {
    BluetoothAdapter btAdapter = null;
    BluetoothDevice btDevice = null;
    String address = "98:D3:31:FD:23:35";
    public BluetoothThread btThread;
    public BluetoothManager(){
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        btDevice = btAdapter.getRemoteDevice(address);
        btThread = new BluetoothThread(btDevice);
        btThread.start();
    }
    public void write(String out){
        btThread.write(out);
    }
}
