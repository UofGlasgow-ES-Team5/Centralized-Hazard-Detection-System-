#ifndef CO2SENSOR_H
#define CO2SENSOR_H

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

class CO2Sensor {
public:
    CO2Sensor();
    ~CO2Sensor();
    void startReadingThread();
    void stopReadingThread();

private:
    const uint16_t MAX_CO2 = 1000;
    const float MAX_TEMP = 25.0;
    const int LED_GPIO = 4;
    const int BUZZER_GPIO = 22;
    std::atomic<bool> keepRunning;
    std::thread sensorThread;

    void sensorReadingThread();
    void initializeSensor();
    void updateDisplay(uint16_t co2, float temperature);
};

#endif // CO2SENSOR_H
