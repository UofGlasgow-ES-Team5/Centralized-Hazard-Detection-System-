#ifndef ZONEHANDLER_H
#define ZONEHANDLER_H

#include <string>
#include "json.hpp"

class ZoneHandler {
public:
    static std::pair<std::string, bool> zoneDetector(std::string sensorDataString, const nlohmann::json& sensorLimits);
    static void zoneFanAction(int zoneID, bool fanAction);
    static int mapBranchZones(const std::string& hazardZoneMAC);
};

#endif /* ZONEHANDLER_H */
