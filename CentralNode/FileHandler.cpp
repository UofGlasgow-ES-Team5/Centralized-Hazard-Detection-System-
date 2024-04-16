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

void FileHandler::writeToSensorDataFile(std::string sensorDataString, const nlohmann::json& sensorLimits){
    if (sensorDataString.empty() || sensorLimits.empty()) {
        // return {"Empty sensor Data", false}; // or throw an exception
        return;
    }

    nlohmann::json jsonData;

    try {
        // Attempt to parse sensorDataString
        jsonData = nlohmann::json::parse(sensorDataString);
    } catch (const std::exception& e) {
        // return {"Parsing error", false}; // Parsing failed, return empty string or handle the error as needed
        return;
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
            // return {"Failed to extract required data", false}; // Failed to extract required data, return empty string or handle the error
            return;
        }
    }

    // Extract CO2, Temperature, and Humidity limits from sensorLimits
    double co2Limit = std::stod(sensorLimits["CO2"].get<std::string>());
    double temperatureLimit = std::stod(sensorLimits["Temperature"].get<std::string>());
    double humidityLimit = std::stod(sensorLimits["Humidity"].get<std::string>());

    int co2Int = static_cast<int>(co2);
    int temperatureInt = static_cast<int>(temperature);
    int humidityInt = static_cast<int>(humidity);

    std::ofstream outputFile("./GUI_HD/sensor_data.txt");

    if (outputFile.is_open()) {
        outputFile << "CO2:" << co2Int << std::endl;
        outputFile << "Temperature:" << temperatureInt << std::endl;
        outputFile << "Humidity:" << humidityInt << std::endl;

        // Close the file
        outputFile.close();
        std::cout << "Data has been written to data.txt" << std::endl;
    } else {
        std::cout << "Unable to open file for writing" << std::endl;
    }
}

void FileHandler::writeToLocationFile(int hazardZoneID) {
    std::string hazardDescription = "Hazard at zone #" + std::to_string(hazardZoneID) + ".";
    std::ofstream outFile("./GUI_HD/location.txt");

     if (!outFile) {
        std::cerr << "Error: Unable to open location file!" << std::endl;
        return;
    }

    // Write hazard description to file
    outFile << hazardDescription << std::endl;

    // Close file
    outFile.close();

    // std::cout << "Hazard report has been written to hazard_report.txt" << std::endl;

    return;
}