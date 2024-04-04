#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>
#include <regex>
#include <string>
#include <sys/socket.h>

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

const int PORT = 1350;

void connectToServer(const std::string& ip_address) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    const char* hello = "Hello from client";

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
    send(sock, hello, strlen(hello), 0);
    std::cout << "Hello message sent" << std::endl;

    // Receive data from server
    valread = read(sock, buffer, 1024);
    std::cout << "Response received: " << buffer << std::endl;

    close(sock);
}

int main() {
    std::string ip_address = getRouterIP();
    std::cout << "Hotspot IP address:" << ip_address << std::endl;
    connectToServer(ip_address);

    return 0;
}
