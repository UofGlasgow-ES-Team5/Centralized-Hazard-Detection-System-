package com.example.chds_branch;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class MainActivity extends AppCompatActivity {

    private  TextView tvStatus;
    private  TextView centralNodeIP;
    private EditText etServerName, etServerPort;
    private Button btnServerStart;
    private Button btnServerStop;
    private  String serverName;
    private int serverPort;

    private Client client;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvStatus = findViewById(R.id.tvRecTxt);
        centralNodeIP = findViewById(R.id.centralNodeIP);
        btnServerStart = findViewById(R.id.btnStartServer);
        btnServerStop = findViewById(R.id.btnStopServer);

        client = new Client(this);
        String serverName = client.getWifiIpAddress();
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                centralNodeIP.setText("Central Node IP:" + serverName);
            }
        });
    }//On create

    private ServerThread serverThread;
    public void onClickStartServer(View view) {
        serverThread = new ServerThread(this);
        serverThread.startServer();
    }

    public void onClickStopServer(View view) {
        serverThread.stopServer();
    }

    public void onClickConnect(View view) {
        serverName = etServerName.getText().toString();
        serverPort = Integer.valueOf(etServerPort.getText().toString());

        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Socket socket = new Socket(serverName, serverPort);

                    BufferedReader br_input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    String txtFromServer = br_input.readLine();

                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            tvStatus.setText(txtFromServer);
                        }
                    });
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }


            }
        }).start();
    }

    //Server thread
    class ServerThread extends Thread implements Runnable {
        private boolean serverRunning;
        private ServerSocket serverSocket;
        private int count = 0;
        private Context context;

        public ServerThread(Context context) {
            this.context = context;
        }

        public void startServer() {
            serverRunning = true;
            start();        // Starts the thread
        }

        @Override
        public void run() {
            try {
                serverSocket = new ServerSocket(1350);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tvStatus.setText("Waiting for Clients");
                    }
                });

                while (serverRunning) {
                    Socket socket = serverSocket.accept();  // Accepting new client
                    count++;

                    BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    String jsonPacket = reader.readLine();

                    // Parse JSON packet
                    JSONObject sensorData = new JSONObject(jsonPacket);

//                     Append MAC address to JSON data
                    String macAddress = client.getMacAddress(context); // Implement this method to get MAC address
                    sensorData.put("branchNode", macAddress);

                    String sensorDataString = sensorData.toString();
                    client.connectToServer(sensorDataString);
//                     Log sensorData
                    Log.d("MainActivity", "Sensor Data: " + sensorDataString);
//                    Log.d("MainActivity", "Sensor Data: ");


                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            tvStatus.setText("Connect to:" + socket.getInetAddress() + ":"  + socket.getLocalPort());
                        }
                    });

                    PrintWriter output_Server = new PrintWriter(socket.getOutputStream());
                    output_Server.write("Welcome to Server:" + count);
                    output_Server.flush();

                    socket.close();
                }
            } catch (IOException e) {
                throw new RuntimeException(e);
            } catch (JSONException e) {
                throw new RuntimeException(e);
            }
        }
        public void stopServer() {
            serverRunning = false;
            new Thread(new Runnable() {
                @Override
                public void run() {
                    if(serverSocket != null) {
                        try {
                            serverSocket.close();

                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    tvStatus.setText("Server stopped");
                                }
                            });
                        } catch (IOException e) {
                            throw new RuntimeException(e);
                        }
                    }
                }
            }).start();
        }
    }//END:Server thread

    //Client Network thread


}
