#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include "json.hpp"
#include "TCPServer.h"

class FileHandler {
public:
    static nlohmann::json readFileToJson();
    static void readFileAndRunServer(nlohmann::json& sensorLimits, TCPServer& server);
};

#endif /* FILEHANDLER_H */
