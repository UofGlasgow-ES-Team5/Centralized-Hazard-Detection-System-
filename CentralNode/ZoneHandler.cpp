#include "ZoneHandler.h"
#include <iostream>
#include <map>
#include <pigpio.h>

const int FAN_GPIO_1 = 15; // GPIO pin for the fan 1
const int FAN_GPIO_2 = 23; // GPIO pin for the fan 2

std::map<int, bool> zoneMap = {
    {0, false},
    {1, false}
};

std::pair<std::string, bool> ZoneHandler::zoneDetector(std::string sensorDataString, const nlohmann::json& sensorLimits) {
    if (sensorDataString.empty() || sensorLimits.empty()) {
        return {"Empty sensor Data", false}; // or throw an exception
    }

    nlohmann::json jsonData;

    try {
        // Attempt to parse sensorDataString
        jsonData = nlohmann::json::parse(sensorDataString);
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

void ZoneHandler::zoneFanAction(int zoneID, bool fanAction) {
    // Implementation of zoneFanAction function
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
                zoneMap[zoneID] = true;
            }
            else
                return;
        }
        else {
            if(zoneMap[zoneID] == true) {
                gpioWrite(FAN_GPIO_2, 0);
                zoneMap[zoneID] = false;
            }
            else
                return;
        }
    }
}

int ZoneHandler::mapBranchZones(const std::string& hazardZoneMAC) {
    if(hazardZoneMAC == "4e79b18fb7e4dc6e")
        return 0;
    else if(hazardZoneMAC == "1a00f95e11d9081f")
        return 1;
    return -1;  // return -1 if there is not match
}
