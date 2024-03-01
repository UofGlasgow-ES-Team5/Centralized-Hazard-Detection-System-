#include <bits/stdc++.h>
#include <regex>
#include <iostream>
#include <chrono>
#include <thread>


std::string findStrongestWifi(std::vector<std::pair<std::string, int>> & myMap) {
    if (myMap.empty()) {
        return "";
    }

    auto maxElement = std::max_element(myMap.begin(), myMap.end(),
                                       [](const auto& p1, const auto& p2) {
                                           return p1.second < p2.second;
                                       });

    return maxElement->first;
}

std::vector<std::pair<std::string, int>> scanForWifi() {
    FILE* pipe = popen("sudo iwlist wlan0 scan", "r");
    if (!pipe) {
        std::cerr << "Error: Couldn't open pipe." << std::endl;
    }

    // Read the output of the command into a buffer
    constexpr int bufferSize = 4096;
    char buffer[bufferSize];
    std::string commandOutput;

    while (fgets(buffer, bufferSize, pipe) != nullptr) {
        commandOutput += buffer;
    }

    // Close the pipe
    pclose(pipe);

    std::vector<std::pair<std::string, int>> qualityAndEssidPairs;

    // Regular expressions to match "Quality=61/" and 'ESSID:"..."' patterns
    std::regex qualityPattern("Quality=(\\d{1,2})/");
    std::regex essidPattern("ESSID:\"([^\"]+)\"");

    // Iterator for finding matches
    std::sregex_iterator iter(commandOutput.begin(), commandOutput.end(), qualityPattern);
    std::sregex_iterator end;

    // Iterate through matches and store quality and ESSID pairs
    while (iter != end) {
        std::string qualityString = iter->str(1);
        int qualityInteger = std::stoi(qualityString);
        // Find ESSID pattern after the current Quality match
        std::sregex_iterator essidIter(iter->suffix().first, commandOutput.end(), essidPattern);
        if (essidIter != std::sregex_iterator()) {
            std::string essid = essidIter->str(1);
            qualityAndEssidPairs.emplace_back(essid, qualityInteger);
        }

        ++iter;
    }

    // Print the stored values
    std::cout << "ESSID values and corresponding Quality values:\n";
    for (const auto& pair : qualityAndEssidPairs) {
        std::cout << "ESSID: " << pair.first << " - Quality: " << pair.second << std::endl;
    }
    return qualityAndEssidPairs;
}

std::map<std::string, std::string> readNetworkFile() {
    std::string filePath = "network_info.txt";

    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }

    // Map to store key-value pairs
    std::map<std::string, std::string> keyValueMap;

    // Read the file content line by line
    std::string line;
    while (std::getline(inputFile, line)) {
        // Use stringstream to extract key and value
        std::istringstream iss(line);
        std::string key, value;

        // Read key and value separated by ':'
        if (std::getline(iss, key, ':') && std::getline(iss, value)) {
            // Trim leading and trailing whitespaces from key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            keyValueMap[key] = value;
        }
    }

    inputFile.close();

    for (const auto& pair : keyValueMap) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }

    return keyValueMap;
}

void connectToWifi(std::string SSID, std::string password) {
    std::cout<<"Test ";
    std::string command = "sudo nmcli device wifi connect "+SSID+" password "+password;
    system(command.c_str());
    std::cout << "Connected to : " << SSID << std::endl;
}

void connectToStrongestWifi(std::map<std::string, std::string> networkInfo, std::vector<std::pair<std::string, int>> scannedWifis) {
    std::string closestWifi = findStrongestWifi(scannedWifis);
    std::cout << "Strongest WiFi : " << closestWifi << std::endl;
    auto searchElement = networkInfo.find(closestWifi);
    if(searchElement != networkInfo.end()) {
        std::cout<<"Will connect to strongest wifi!!" << std::endl;
        connectToWifi(searchElement->first, searchElement->second);
    }
    else {
        std::cout << "Closest network not available not persent in the provided network" << std::endl;
    }
    
}

void startNetworkProcess(std::map<std::string, std::string> &networkInfo) {
    auto wifiMap = scanForWifi();
    connectToStrongestWifi(networkInfo, wifiMap);
}



int main() {

    const std::chrono::seconds interval(20);        //The main thread will run every 20 sec
    int iteration = 0;

    auto networkInfo = readNetworkFile();
    while (true) {
        // Call your function
        std::cout << "Iteration : " << ++iteration << "--------------------------" << std::endl;
        startNetworkProcess(networkInfo);
        // Sleep for 20
        std::this_thread::sleep_for(interval);
    }



    return EXIT_SUCCESS;
}
