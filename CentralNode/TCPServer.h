#pragma once

#include <string>
#include <vector>
#include <mutex>

class TCPServer {
private:
    int listeningSocket;
    std::string sensorLimits;
    std::vector<std::string> requestQueue;
    std::mutex queueMutex; // Mutex to protect access to requestQueue

public:
    TCPServer();
    ~TCPServer();
    bool start(int port);
    void run();
    void update(const std::string& newLimits);
    std::vector<std::string> getRequestQueue(); // Method to get the request queue
    void clearRequestQueue();
};
