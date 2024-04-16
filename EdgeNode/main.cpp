#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <map>

#include "json.hpp"

#include "SensorReader.h"
#include "WifiManager.h"
#include "Client.h"

int main() {
    nlohmann::json sensorData, sensorDataLimits;
    SensorReader sensor;
    WifiManager wifiManager;
    Client client;

    std::string branchNode = "", branchNodeIP = "";
    std::map<std::string, std::string> networkInfo = wifiManager.readNetworkFile();

    sensorData["co2"] = 0;
    sensorData["temperature"] = 0;
    sensorData["humidity"] = 0;
    
    sensorDataLimits["co2"] = 1000.0;
    sensorDataLimits["temperature"] = 25.0;
    sensorDataLimits["humidity"] = 40;


     // Start WifiManager in a separate thread
    std::thread wifiThread([&]() {
        while (true) {
            wifiManager.startNetworkProcess(branchNode, branchNodeIP, networkInfo);
            std::this_thread::sleep_for(std::chrono::seconds(7)); // Sleep for 7 seconds
        }
    });
    
    // Start SensorReader in a separate thread
    std::thread sensorThread([&]() {
        sensor.startReadingThread(sensorData, sensorDataLimits);
    });

   

    // Start WifiManager in a separate thread
    std::thread clientThread([&]() {
        while (true) {
            // client.connectToServer(branchNodeIP, sensorData);
            client.connectToServer(branchNodeIP, sensorData, sensorDataLimits);
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Sleep for 5 seconds
        }
    });

    // Join the threads
    std::cout << "Press Enter to quit" << std::endl;
    std::cin.get();
    sensor.stopReadingThread();
    // wifiThread.join();
    clientThread.join();

    return 0;
}
