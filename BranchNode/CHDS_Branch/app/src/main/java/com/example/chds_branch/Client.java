package com.example.chds_branch;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Client {

    private String serverName;
    private int serverPort;
    private Context context;

    public Client(Context context) {
        this.context = context;
    }

    public String getWifiIpAddress() {
        String ipAddress = null;

        // Check if the device is connected to a Wi-Fi network
        ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        if (connectivityManager != null) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                Network network = connectivityManager.getActiveNetwork();
                if (network != null) {
                    WifiManager wifiManager = (WifiManager) context.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
                    if (wifiManager != null) {
                        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
                        int ip = wifiInfo.getIpAddress();
                        ipAddress = formatIpAddress(ip);
                    }
                }
            } else {
                NetworkInfo networkInfo = connectivityManager.getActiveNetworkInfo();
                if (networkInfo != null && networkInfo.getType() == ConnectivityManager.TYPE_WIFI) {
                    WifiManager wifiManager = (WifiManager) context.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
                    if (wifiManager != null) {
                        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
                        int ip = wifiInfo.getIpAddress();
                        ipAddress = formatIpAddress(ip);
                    }
                }
            }
        }
        return ipAddress;
    }

    public static String getMacAddress(Context context) {
        WifiManager wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
        return wifiInfo.getMacAddress();
    }

    public void connectToServer(String sensorDataString) {
        this.serverName = getRouterIpAddress(this.context);
        this.serverPort = serverPort;

        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Socket socket = new Socket(serverName, serverPort);

                    //Send JSON string to server
                    PrintWriter writer = new PrintWriter(socket.getOutputStream(), true);
                    writer.println(sensorDataString);

                    BufferedReader br_input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    final String txtFromServer = br_input.readLine();

//                    callback.onSuccess(txtFromServer);
                } catch (IOException e) {
//                    callback.onError(e);
                }
            }
        }).start();
    }

    public String getRouterIpAddress(Context context) {
        WifiManager wifiManager = (WifiManager) context.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();

        int ipAddress = wifiInfo.getIpAddress(); // Get the IP address of the device

        // Convert IP address to String
        return formatIpAddress(ipAddress);
    }

    private String formatIpAddress(int ip) {
        return (ip & 0xFF) + "." +
                ((ip >> 8) & 0xFF) + "." +
                ((ip >> 16) & 0xFF) + "." +
                ((ip >> 24) & 0xFF);
    }

    public interface ConnectionCallback {
        void onSuccess(String response);
        void onError(Exception e);
    }
}
