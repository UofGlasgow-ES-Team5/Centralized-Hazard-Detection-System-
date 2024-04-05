#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <map>
#include "SensorReader.h"
#include "WifiManager.h"
#include "Client.h"


int main() {
    SensorReader sensor;
    WifiManager wifiManager;
    Client client;

    std::string branchNode = "", branchNodeIP = "";
    std::map<std::string, std::string> networkInfo = wifiManager.readNetworkFile();

    


    // Start SensorReader in a separate thread
    std::thread sensorThread([&]() {
        sensor.startReadingThread();
    });

    // Start WifiManager in a separate thread
    std::thread wifiThread([&]() {
        while (true) {
            wifiManager.startNetworkProcess(branchNode, branchNodeIP, networkInfo);
            std::this_thread::sleep_for(std::chrono::seconds(20)); // Sleep for 20 seconds
        }
    });

    // Start WifiManager in a separate thread
    std::thread clientThread([&]() {
        while (true) {
            client.connectToServer(branchNodeIP);
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Sleep for 20 seconds
        }
    });

    // Join the threads
    sensor.stopReadingThread();
    wifiThread.join();
    clientThread.join();

    return 0;
}
