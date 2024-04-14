#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <pigpio.h>

#include "TCPServer.h"
#include "json.hpp"

std::string SENSOR_LIMITS_FILE = "./GUI_HD/sensor_limits.txt";

const int FAN_GPIO_1 = 15; // GPIO pin for the fan 1
const int FAN_GPIO_2 = 18; // GPIO pin for the fan 2

std::map<int, bool> zoneMap = {
    {0, false},
    {1, false}
};

using json = nlohmann::json;

json readFileToJson(const std::string& filename) {
    std::ifstream file(filename);
    json jsonData;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // Extract field name and value from each line
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);
                // Trim whitespace from key and value
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                // Store key-value pair in JSON object
                jsonData[key] = value;
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return jsonData;
}

void readFileAndRunServer(json& sensorLimits, TCPServer& server) {
    while (true) {
        sensorLimits = readFileToJson(SENSOR_LIMITS_FILE);
        std::string sensorLimitsString = sensorLimits.dump();

        // Update server with new sensor limits
        server.update(sensorLimitsString);

        // Sleep for 5 seconds
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

std::pair<std::string, bool> zoneDetector(std::string sensorDataString, json& sensorLimits) {
    // Validate input
    if (sensorDataString.empty() || sensorLimits.empty()) {
        return {"Empty sensor Data", false}; // or throw an exception
    }

    json jsonData;

    try {
        // Attempt to parse sensorDataString
        jsonData = json::parse(sensorDataString);
    } catch (const std::exception& e) {
        return {"Parsing error", false}; // Parsing failed, return empty string or handle the error as needed
    }

    // Extract CO2, Temperature, and Humidity values from jsonData
    double co2 = 0.0, temperature = 0.0, humidity = 0.0;

    try {
        // Try uppercase keys first
        co2 = std::stod(jsonData["CO2"].get<std::string>());
        temperature = std::stod(jsonData["Temperature"].get<std::string>());
        humidity = std::stod(jsonData["Humidity"].get<std::string>());
    } catch (const std::exception& e) {
        // If uppercase keys failed, try lowercase keys
        try {
            co2 = jsonData["co2"];
            temperature = jsonData["temperature"];
            humidity = jsonData["humidity"];
        } catch (const std::exception& e) {
            return {"Failed to extract required data", false}; // Failed to extract required data, return empty string or handle the error
        }
    }

    // Extract CO2, Temperature, and Humidity limits from sensorLimits
    double co2Limit = std::stod(sensorLimits["CO2"].get<std::string>());
    double temperatureLimit = std::stod(sensorLimits["Temperature"].get<std::string>());
    double humidityLimit = std::stod(sensorLimits["Humidity"].get<std::string>());

    // Check for hazard zones
    std::pair<std::string, bool> zoneInfo;
    std::string branchNode = jsonData["branchNode"].get<std::string>();

    if (co2 > co2Limit || temperature > temperatureLimit || humidity > humidityLimit) {
        zoneInfo = {branchNode, true};
    }
    else {
        zoneInfo = {branchNode, false};
    }
    // std::cout << co2 << " - " << temperature << "-" << humidity << "-->" << branchNode << std::endl;

    return zoneInfo;
}

void zoneFanAction(int zoneID, bool fanAction) {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed\n" << std::endl;
        return;
    }

    if(zoneID == 0) {
        if(fanAction == true){
            gpioSetMode(FAN_GPIO_1, PI_OUTPUT); // Set fan GPIO pin as output
            if(zoneMap[zoneID] == false) {
                gpioWrite(FAN_GPIO_1, 1);
                zoneMap[zoneID] = true;
            }
            else
                return;
        }
        else {
            if(zoneMap[zoneID] == true) {
                gpioWrite(FAN_GPIO_1, 0);
                zoneMap[zoneID] = false;
            }
            else
                return;
        }
    }
    if(zoneID == 1) {
        if(fanAction == true){
            gpioSetMode(FAN_GPIO_2, PI_OUTPUT); // Set fan GPIO pin as output
            if(zoneMap[zoneID] == false) {
                gpioWrite(FAN_GPIO_2, 1);
            }
            else
                return;
        }
        else {
            if(zoneMap[zoneID] == true) {
                gpioWrite(FAN_GPIO_2, 0);
            }
            else
                return;
        }
    }
}

int mapBranchZones(std::string hazardZoneMAC) {
    if(hazardZoneMAC == "02:00:00:00:00:00")
        return 0;
    return -1;  // return -1 if there is not match
}

int main() {
    std::string filename = "./GUI_HD/sensor_limits.txt"; // Change this to your file path
    json sensorLimits = readFileToJson(filename);

    TCPServer server;
    if (!server.start(1352)) {
        std::cerr << "Failed to start server" << std::endl;
        return -1;
    }

    // Start a thread to read the file and update the server
    std::thread fileReaderThread(readFileAndRunServer, std::ref(sensorLimits), std::ref(server));


    // Start server in a separate thread
    std::thread serverThread(&TCPServer::run, &server);

    // Main thread to display request strings received by the server
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 second
        // Get request queue from server
        std::vector<std::string> requestQueue = server.getRequestQueue();
        
        // Display received request strings
        for (const auto& request : requestQueue) {
            std::cout << "Received request: " << request << std::endl;
            std::pair<std::string, bool> zoneInfo = zoneDetector(request, sensorLimits);
            int hazardZoneID = mapBranchZones(zoneInfo.first);
            if(hazardZoneID > -1) {
                std::cout << "Hazard zone #" << hazardZoneID << std::endl;
                zoneFanAction(hazardZoneID, zoneInfo.second);
            }
        }

        // Clear the request queue
        server.clearRequestQueue();
    }

    // Join the threads
    fileReaderThread.join();
    serverThread.join();

    return 0;
}