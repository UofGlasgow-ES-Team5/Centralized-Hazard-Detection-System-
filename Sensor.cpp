// Include necessary headers for IO operations, threading, timing, atomic operations, GPIO, and LCD control
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


// Global variable to control the running of the sensor reading thread
std::atomic<bool> keepRunning(true);

// Function that will be run in a separate thread for reading sensor data
void sensorReadingThread() {
    // Initialize the I2C HAL (Hardware Abstraction Layer) for communication with the sensor
    sensirion_i2c_hal_init();

    // Wake up the sensor and start periodic measurements
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();
    scd4x_start_periodic_measurement();

    // Loop to read sensor data continuously until keepRunning is set to false
    while (keepRunning) {
        bool data_ready_flag = false;
        // Wait for 0.5 seconds for the sensor to be ready to send new data
        sensirion_i2c_hal_sleep_usec(500000); // 0.5 seconds delay
        scd4x_get_data_ready_flag(&data_ready_flag);
        if (!data_ready_flag) {
            continue; // Skip the rest of the loop if data is not ready
        }
        uint16_t co2;
        float temperature, humidity;
        // Read the measurement from the sensor. If successful, update the display.
        if (scd4x_read_measurement(&co2, &temperature, &humidity) == 0) {
            char buffer[32];
            // Prepare the string with CO2 and temperature data for the first line of the display
            snprintf(buffer, sizeof(buffer), "CO2:%u ppm", co2);
            lcd1602SetCursor(0, 0); // Set cursor to the beginning of the first line
            lcd1602WriteString(buffer); // Write the string to the display

            // Prepare the string with humidity data for the second line of the display
            snprintf(buffer, sizeof(buffer), "Temp:%.2fC", temperature);
            lcd1602SetCursor(0, 1); // Set cursor to the beginning of the second line
            lcd1602WriteString(buffer); // Write the string to the display
        }
    }

    // Stop the sensor's periodic measurements and free I2C HAL resources when done
    scd4x_stop_periodic_measurement();
    sensirion_i2c_hal_free();
}

int main() {
    // Initialize the GPIO library. If it fails, print an error message and exit.
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return 1;
    }

    // Initialize the LCD display. If it fails, print an error message, terminate GPIO and exit.
    if (lcd1602Init(1, 0x3f) != 0) {
        std::cerr << "LCD initialization failed" << std::endl;
        gpioTerminate();
        return 1;
    }

    // Start the sensor reading in a separate thread
    std::thread sensorThread(sensorReadingThread);

    std::cout << "Press Enter to quit" << std::endl;
    // Wait for the user to press Enter to quit the application
    std::cin.get();

    // Signal the sensor reading thread to stop and wait for it to finish
    keepRunning = false;
    sensorThread.join();

    // Shutdown the LCD display and terminate GPIO use before exiting
    lcd1602Shutdown();
    gpioTerminate();
    return 0;
}
