#ifndef SENSORREADER_H
#define SENSORREADER_H

#include <iostream>
#include <cstdint>
#include <thread>
#include <chrono>
#include <atomic>
#include <pigpio.h>
extern "C" {
    #include <lcd1602.h>
}
#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"

// Include nlohmann/json.hpp
#include "json.hpp"

class SensorReader {
public:
    SensorReader();
    ~SensorReader();
    void startReadingThread(nlohmann::json &sensorData, nlohmann::json &sensorDataLimits);
    void stopReadingThread();

private:
    const uint16_t MAX_CO2 = 1000;
    const float MAX_TEMP = 25.0;
    const int LED_GPIO = 4;
    const int BUZZER_GPIO = 16;
    std::atomic<bool> keepRunning;
    std::thread sensorThread;

    void sensorReadingThread(nlohmann::json &sensorData, nlohmann::json &sensorDataLimits);
};

#endif // SENSORREADER_H
