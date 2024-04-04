#include <iostream>
#include <thread>
#include <chrono>
#include "SensorReader.h"
#include "WifiManager.h"

int main() {
    SensorReader sensor;
    WifiManager wifiManager;

    // Start SensorReader in a separate thread
    std::thread sensorThread([&]() {
        sensor.startReadingThread();
    });

    // Start WifiManager in a separate thread
   std::thread wifiThread([&]() {
        while (true) {
            wifiManager.startNetworkProcess();
            std::this_thread::sleep_for(std::chrono::seconds(20)); // Sleep for 20 seconds
        }
    });

    // Join the threads

    std::cout << "Press Enter to quit" << std::endl;
    std::cin.get();
    sensor.stopReadingThread();
    wifiThread.join();

    return 0;
}
