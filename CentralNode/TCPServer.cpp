#include "TCPServer.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

TCPServer::TCPServer() : listeningSocket(-1) {}

TCPServer::~TCPServer() {
    if (listeningSocket != -1) {
        close(listeningSocket);
    }
}

bool TCPServer::start(int port) {
    // Implementation remains the same
    std::cout << "Starting server" << std::endl;

        // Create a socket
        listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (listeningSocket == -1) {
            std::cerr << "Can't create a socket";
            return false;
        }

        // Bind the socket to an IP/Port
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);    // Host to network short i.e. conversion to little endian to big endian if necessary
        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

        if (bind(listeningSocket, (sockaddr*)&hint, sizeof(hint)) == -1) {
            std::cerr << "Can't bind to IP/port";
            return false;
        }

        // Mark the socket for listening
        if (listen(listeningSocket, SOMAXCONN) == -1) {
            std::cerr << "Can't listen";
            return false;
        }

        return true;
}

void TCPServer::run() {
    while (true) {
        while (true) {
            // Accept a call
            sockaddr_in client;
            socklen_t clientSize = sizeof(client);
            char host[NI_MAXHOST];
            char svc[NI_MAXSERV];

            int clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);

            if (clientSocket == -1) {
                std::cerr << "Problem with client connecting!";
                continue;
            }

            memset(host, 0, NI_MAXHOST);
            memset(svc, 0, NI_MAXSERV);

            int result = getnameinfo((
                    sockaddr*)&client,
                    sizeof(client),
                    host,
                    NI_MAXHOST,
                    svc,
                    NI_MAXSERV,
                    0
            );

            if (result) {
                std::cout << "Host:" << host << " connected on " << svc << std::endl;
            } else {
                inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
                std::cout << "Host:"<< host << " Connected on " << ntohs(client.sin_port) << std::endl;
            }

            // While receiving - display message, echo message
            char buf[4096];

            // Clear the buffer
            memset(buf, 0, 4096);
            int bytesRecv = recv(clientSocket, buf, 4096, 0);
            if (bytesRecv == -1) {
                std::cerr << "There was a connection issue" << std::endl;
            }

            if (bytesRecv == 0) {
                std::cerr << "The client was disconnected" << std::endl;
            }

            // Display message
            // std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
            std::lock_guard<std::mutex> lock(queueMutex);
            requestQueue.push_back(std::string(buf, 0, bytesRecv));

            std::string response = sensorLimits + "\n";

            // Resend message
            send(clientSocket, response.c_str(), response.size() + 1, 0);

            // Close the client socket
            close(clientSocket);
        }
    }
}

void TCPServer::update(const std::string& newLimits) {
    // Implementation remains the same
    sensorLimits = newLimits;
}

std::vector<std::string> TCPServer::getRequestQueue() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return requestQueue;
}

void TCPServer::clearRequestQueue() {
    std::lock_guard<std::mutex> lock(queueMutex);
    requestQueue.clear();
}

