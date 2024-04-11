#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>

#include "TCPServer.h"
#include "json.hpp"

std::string SENSOR_LIMITS_FILE = "./GUI_HD/sensor_limits.txt";

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

std::string zoneDetector(std::string sensorDataString, json& sensorLimits) {
   
    json jsonData = json::parse(sensorDataString);

    // Extract CO2, Temperature, and Humidity values from jsonData
    double co2 = std::stod(jsonData["CO2"].get<std::string>());
    double temperature = std::stod(jsonData["Temperature"].get<std::string>());
    double humidity = std::stod(jsonData["Humidity"].get<std::string>());

    // // Extract CO2, Temperature, and Humidity limits from sensorLimits
    double co2Limit = std::stod(jsonData["CO2"].get<std::string>());
    double temperatureLimit = std::stod(jsonData["Temperature"].get<std::string>());
    double humidityLimit = std::stod(jsonData["Humidity"].get<std::string>());

    std::string hazardZone = "";

    // std::cout << "TEST 0::Hazard zone" << co2 <<"CO2 linit:" << co2Limit <<std::endl;
    // Compare extracted values with corresponding limits
    if (co2 > 10) {
        hazardZone = jsonData["branchNode"].get<std::string>();
        // hazardZone = "TEST";
    }
    if (temperature > temperatureLimit) {
        // std::cout << "TEST 0::Hazard zone" << std::endl;
        hazardZone = jsonData["branchNode"].get<std::string>();
    }
    if (humidity > humidityLimit) {
        //  std::cout << "TEST 0::Hazard zone" << std::endl;
        hazardZone = jsonData["branchNode"].get<std::string>();
    }

    // std::cout << "TEST 1::Hazard zone" << hazardZone << std::endl;
    return hazardZone;
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
        std::string hazardZone = zoneDetector(request, sensorLimits);
        // if(hazardZone.length()) {
            std::cout << "Hazard zone:" << hazardZone << std::endl;
        // }
    }

    // Clear the request queue
    server.clearRequestQueue();
}

    // Join the threads
    fileReaderThread.join();
    serverThread.join();

    return 0;
}