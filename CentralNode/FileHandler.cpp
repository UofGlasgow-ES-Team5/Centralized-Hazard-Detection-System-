#include "FileHandler.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

const std::string SENSOR_LIMITS_FILE = "./GUI_HD/sensor_limits.txt";

nlohmann::json FileHandler::readFileToJson() {
    std::ifstream file(SENSOR_LIMITS_FILE);
    nlohmann::json jsonData;

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
        std::cerr << "Unable to open file: " << SENSOR_LIMITS_FILE << std::endl;
    }

    return jsonData;
}

void FileHandler::readFileAndRunServer(nlohmann::json& sensorLimits, TCPServer& server) {
    while (true) {
        sensorLimits = FileHandler::readFileToJson();
        std::string sensorLimitsString = sensorLimits.dump();

        // Update server with new sensor limits
        server.update(sensorLimitsString);

        // Sleep for 5 seconds
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
