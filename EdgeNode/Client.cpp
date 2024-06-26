#include "Client.h"
#include "json.hpp"

std::string Client::getRouterIP() {
    std::string result;
    FILE* pipe = popen("ip route | grep default", "r");
    if (!pipe) {
        std::cerr << "Error executing command." << std::endl;
        return result;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    // Extract the IP address using regular expression
    std::regex regexIP("\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b");
    std::smatch match;
    if (std::regex_search(result, match, regexIP)) {
        return match.str();
    } else {
        return "";  // Return an empty string if no IP address is found
    }
}

Client::Client() : ip_address(getRouterIP()) {}

void Client::connectToServer(std::string &branchNodeIP, nlohmann::json &sensorData, nlohmann::json &sensorDataLimits) {
    if(branchNodeIP != ip_address) {
        ip_address = getRouterIP();
        if(ip_address.size()){
            branchNodeIP = ip_address;
            std::cout << "New IP: " << ip_address << std::endl;
        }
    }
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    // const char* hello = "Hello from client";
    std::string sensorDataString = sensorData.dump() + "\n";

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return;
    }

    // Send data to server
    send(sock, sensorDataString.c_str(), sensorDataString.length(), 0);
    std::cout << "Client sent:" << sensorDataString << std::endl;

    // Receive data from server
    valread = read(sock, buffer, 1024);
    std::cout << "Response received: " << buffer << std::endl;

    // Parse JSON data received in buffer
    nlohmann::json receivedJson;

    try {
        receivedJson = nlohmann::json::parse(buffer);
        updateSensorLimits(receivedJson, sensorDataLimits);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        close(sock);
        return;
    }

    close(sock);
}

void Client::updateSensorLimits(nlohmann::json receivedJson, nlohmann::json &sensorDataLimits) {
        double co2 = 0.0, temperature = 0.0, humidity = 0.0;

        co2 = std::stod(receivedJson["CO2"].get<std::string>());
        temperature = std::stod(receivedJson["Temperature"].get<std::string>());
        humidity = std::stod(receivedJson["Humidity"].get<std::string>());
        // std::cout << "New CO2:" << co2 << " - Temp:" << temperature << "- Humidity:" << humidity  << std::endl;

        sensorDataLimits["co2"] = co2;
        sensorDataLimits["temperature"] = temperature;
        sensorDataLimits["humidity"] = humidity;

        // std::cout << "New Limits" << sensorDataLimits.dump() << std::endl;
}