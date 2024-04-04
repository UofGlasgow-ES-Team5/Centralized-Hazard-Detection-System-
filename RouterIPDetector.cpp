#include <iostream>
#include <cstdio>
#include <regex>

std::string getRouterIP() {
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

int main() {
    std::string routerIP = getRouterIP();
    if (!routerIP.empty()) {
        std::cout << "Router IP: " << routerIP << std::endl;
    } else {
        std::cerr << "Unable to retrieve router IP." << std::endl;
    }
    return 0;
}
