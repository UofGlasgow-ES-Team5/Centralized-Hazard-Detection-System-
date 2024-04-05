#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>
#include <regex>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <chrono>
#include <atomic>

class Client {
private:
    std::string ip_address;
    const int PORT = 1350;
    std::atomic<bool> running{false};
    std::thread clientThread;
    std::string getRouterIP();

public:
    Client();
    void connectToServer(std::string &branchNodeIP);
};

#endif // CLIENT_H
