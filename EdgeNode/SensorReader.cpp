#include "SensorReader.h"
#include "json.hpp"

SensorReader::SensorReader() : keepRunning(true) {
    gpioInitialise();
    gpioSetMode(LED_GPIO, PI_OUTPUT);
    gpioSetMode(BUZZER_GPIO, PI_OUTPUT);
    if (lcd1602Init(1, 0x3f) != 0) {
        std::cerr << "LCD initialization failed" << std::endl;
        gpioTerminate();
        std::exit(1);
    }
}

SensorReader::~SensorReader() {
    lcd1602Shutdown();
    gpioTerminate();
}

void SensorReader::startReadingThread(nlohmann::json &sensorData, nlohmann::json &sensorDataLimits) {
    sensorThread = std::thread([this, &sensorData, &sensorDataLimits]() {
        this->sensorReadingThread(sensorData, sensorDataLimits);
    });
}

void SensorReader::stopReadingThread() {
    keepRunning = false;
    sensorThread.join();
}

void SensorReader::sensorReadingThread(nlohmann::json &sensorData, nlohmann::json &sensorDataLimits) {
    sensirion_i2c_hal_init();

    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();
    scd4x_start_periodic_measurement();

    bool isFlashing = false; // Track if we are currently flashing the LED

    while (keepRunning) {
        bool data_ready_flag = false;
        sensirion_i2c_hal_sleep_usec(1000000); // 1 second delay
        scd4x_get_data_ready_flag(&data_ready_flag);
        if (!data_ready_flag) {
            continue;
        }
        uint16_t co2;
        float temperature, humidity;
        if (scd4x_read_measurement(&co2, &temperature, &humidity) == 0) {
            // Determine if we need to flash the LED
            sensorData["co2"] = co2;
            sensorData["temperature"] = temperature;
            sensorData["humidity"] = humidity;
            bool shouldFlash = co2 > sensorDataLimits["co2"] || temperature > sensorDataLimits["temperature"];

            // Convert JSON object to string
            std::string jsonString = sensorData.dump();
            
            // Print the JSON string
            // std::cout << "JSON Data:\n" << jsonString << std::endl;
            
            if (shouldFlash && !isFlashing) {
    // If we need to flash and are not already doing so, start flashing
                isFlashing = true;
                std::thread([&]() { // Capture by reference to allow the thread to see updates to isFlashing
           
                    while (isFlashing) {
                        gpioWrite(LED_GPIO, 1); // LED on
                        gpioWrite(BUZZER_GPIO, 1); // BUZZER on
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // delay 1000ms on
                        gpioWrite(LED_GPIO, 0); // LED off
                        gpioWrite(BUZZER_GPIO, 0); // Buzzer off
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // delay 1000ms off

                     
                    }
                    gpioWrite(LED_GPIO, 0); // Ensure the LED is off when the flashing stops
                    gpioWrite(BUZZER_GPIO, 0); // Ensure the BUZZER is off when the buzzing stops
                }).detach(); // Detach the thread so we don't need to join it later
            } else if (!shouldFlash) {
                // If we should stop flashing, update the flag
                isFlashing = false;
            }

            
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "CO2:%u ppm", co2);
            lcd1602SetCursor(0, 0);
            lcd1602WriteString(buffer);

            snprintf(buffer, sizeof(buffer), "Temp:%.2fC", temperature);
            lcd1602SetCursor(0, 1);
            lcd1602WriteString(buffer);
        }
    }

    isFlashing = false; // Ensure flashing stops when the loop exits
    gpioWrite(LED_GPIO, 0); // Ensure the LED is off when stopping

    scd4x_stop_periodic_measurement();
    sensirion_i2c_hal_free();
}

