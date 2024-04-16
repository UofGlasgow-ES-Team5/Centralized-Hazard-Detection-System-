#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include "TCPServer.h"
#include "json.hpp"
#include "FileHandler.h"
#include "ZoneHandler.h"

// const std::string SENSOR_LIMITS_FILE = "./GUI_HD/sensor_limits.txt";

int main() {
    // Read sensor limits from file
    nlohmann::json sensorLimits = FileHandler::readFileToJson();

    // Initialize TCP server
    TCPServer server;
    if (!server.start(1352)) {
        std::cerr << "Failed to start server" << std::endl;
        return -1;
    }

    // Start a thread to read the file and update the server
    std::thread fileReaderThread(FileHandler::readFileAndRunServer, std::ref(sensorLimits), std::ref(server));

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
            std::pair<std::string, bool> zoneInfo = ZoneHandler::zoneDetector(request, sensorLimits);
            FileHandler::writeToSensorDataFile(request, sensorLimits);
            int hazardZoneID = ZoneHandler::mapBranchZones(zoneInfo.first);
            if(hazardZoneID > -1) {
                std::cout << "Hazard zone #" << hazardZoneID << std::endl;
                FileHandler::writeToLocationFile(hazardZoneID);
                ZoneHandler::zoneFanAction(hazardZoneID, zoneInfo.second);
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
