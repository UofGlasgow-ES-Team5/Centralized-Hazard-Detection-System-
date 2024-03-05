 #include <iostream>
#include <cstdint> // For fixed width integers
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

// Thresholds
const uint16_t MAX_CO2 = 1000; // CO2 threshold in ppm
const float MAX_TEMP = 25.0; // Temperature threshold in °C

// GPIO for LED and Buzzer
const int LED_GPIO = 4; // GPIO 4 as per BCM numbering
const int BUZZER_GPIO = 17; 
std::atomic<bool> keepRunning(true);

// Function to control LED state
//void setLEDState(bool state) {
//    gpioWrite(LED_GPIO, state ? 1 : 0); // Turn LED on/off based on state
//}

// Function that will be run in a separate thread for reading sensor data
void sensorReadingThread() {
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
            bool shouldFlash = co2 > MAX_CO2 || temperature > MAX_TEMP;
            
            if (shouldFlash && !isFlashing) {
    // If we need to flash and are not already doing so, start flashing
                isFlashing = true;
                std::thread([&]() { // Capture by reference to allow the thread to see updates to isFlashing
                    int delay = 1000; // Initializing the delay ( in ms)
                    while (isFlashing) {
                        gpioWrite(LED_GPIO, 1); // LED on
                        gpioWrite(BUZZER_GPIO, 1); // BUZZER on
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // delay ms on
                        gpioWrite(LED_GPIO, 0); // LED off
                        gpioWrite(BUZZER_GPIO, 0); // Buzzer off
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // delay ms off
                        delay = delay-50; // this increases the flashing and buzzing frequency as the loop count increases
                        if (delay<0){
                            delay = 0; // eliminate the effect of negative delay values
                        }
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



int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return 1;
    }

    gpioSetMode(LED_GPIO, PI_OUTPUT); // Set LED GPIO as output
    gpioSetMode(BUZZER_GPIO, PI_OUTPUT); // Set BUZZER GPIO as output


    if (lcd1602Init(1, 0x3f) != 0) {
        std::cerr << "LCD initialization failed" << std::endl;
        gpioTerminate();
        return 1;
    }

    std::thread sensorThread(sensorReadingThread);

    std::cout << "Press Enter to quit" << std::endl;
    std::cin.get();

    keepRunning = false;
    sensorThread.join();

  //  setLEDState(false); // Ensure the LED is turned off before exiting

    lcd1602Shutdown();
    gpioTerminate();
    return 0;
}
