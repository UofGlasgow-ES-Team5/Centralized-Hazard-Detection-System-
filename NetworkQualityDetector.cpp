#include <bits/stdc++.h>
#include <regex>

void connectToWiFi(const string& ssid, const string& password) {
    // Path to the wpa_supplicant configuration file
    const string wpaConfPath = "/etc/wpa_supplicant/wpa_supplicant.conf";

    // Check if the network entry already exists in the configuration file
    ifstream wpaConfFile(wpaConfPath);
    string line;
    while (getline(wpaConfFile, line)) {
        if (line.find("ssid=\"" + ssid + "\"") != string::npos) {
            cout << "Network entry already exists in " << wpaConfPath << endl;
            return;
        }
    }
    wpaConfFile.close();

    // Add the new network entry to the wpa_supplicant configuration file
    ofstream wpaConfAppend(wpaConfPath, ios::app);
    wpaConfAppend << "\nnetwork={" << endl;
    wpaConfAppend << "    ssid=\"" << ssid << "\"" << endl;
    wpaConfAppend << "    psk=\"" << password << "\"" << endl;
    wpaConfAppend << "}" << endl;
    wpaConfAppend.close();

    // Restart the networking service to apply changes
    system("sudo systemctl restart networking");

    cout << "WiFi configuration updated successfully for SSID: " << ssid << endl;
}

int main() {
    // Run the command and open a pipe to capture its output
    FILE* pipe = popen("sudo iwlist wlan0 scan", "r");
    if (!pipe) {
        std::cerr << "Error: Couldn't open pipe." << std::endl;
        return EXIT_FAILURE;
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

    std::vector<std::pair<std::string, std::string>> qualityAndEssidPairs;

    // Regular expressions to match "Quality=61/" and 'ESSID:"..."' patterns
    std::regex qualityPattern("Quality=(\\d{1,2})/");
    std::regex essidPattern("ESSID:\"([^\"]+)\"");

    // Iterator for finding matches
    std::sregex_iterator iter(commandOutput.begin(), commandOutput.end(), qualityPattern);
    std::sregex_iterator end;

    // Iterate through matches and store quality and ESSID pairs
    while (iter != end) {
        std::string quality = iter->str(1);

        // Find ESSID pattern after the current Quality match
        std::sregex_iterator essidIter(iter->suffix().first, commandOutput.end(), essidPattern);
        if (essidIter != std::sregex_iterator()) {
            std::string essid = essidIter->str(1);
            qualityAndEssidPairs.emplace_back(quality, essid);
        }

        ++iter;
    }

    // Print the stored values
    std::cout << "ESSID values and corresponding Quality values:\n";
    for (const auto& pair : qualityAndEssidPairs) {
        std::cout << "ESSID: " << pair.second << " - Quality: " << pair.first << std::endl;
    }



    return EXIT_SUCCESS;
}
