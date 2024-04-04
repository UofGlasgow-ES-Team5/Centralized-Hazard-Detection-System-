#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
#include <chrono>
#include <thread>

class WifiManager {
private:
    std::map<std::string, std::string> networkInfo;

    std::string findStrongestWifi(std::vector<std::pair<std::string, int>> &myMap);
    std::vector<std::pair<std::string, int>> scanForWifi();
    std::map<std::string, std::string> readNetworkFile();
    void connectToWifi(std::string SSID, std::string password);
    void connectToStrongestWifi(std::map<std::string, std::string> networkInfo, std::vector<std::pair<std::string, int>> scannedWifis);

public:
    WifiManager();
    void startNetworkProcess();
};

#endif
