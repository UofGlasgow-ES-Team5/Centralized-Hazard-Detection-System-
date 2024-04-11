#include <iostream>
#include <cstdint>
#include <pigpio.h>
#include <atomic>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

const uint16_t MAX_CO2 = 1000; // CO2 threshold in ppm
const int FAN_GPIO = 18; // GPIO pin for the fan
std::atomic<bool> keepRunning(true);

void CheckCO2AndControlFan() {
    uint16_t co2Level;
    uint16_t lastCO2Level = 0; // Variable to store the last CO2 level read
    std::ifstream file("co2level.txt");

    // Read through file until end to get the latest CO2 level
    while (file >> co2Level) {
        lastCO2Level = co2Level;
    }

    // Use last CO2 level to compare with threshold
    if (lastCO2Level > MAX_CO2) {
        gpioWrite(FAN_GPIO, 1); // Turn the fan on
        std::cout << "Fan ON\n";
    }
    else {
        gpioWrite(FAN_GPIO, 0); // Turn the fan off
        std::cout << "Fan OFF\n";
    }

    if (file.eof()) {
        std::cout << "FanEOF\n";
        // Reached end of file, normal condition when reading the last value
    }
    else {
        // An error occurred during reading
        std::cerr << "Failed to read CO2 level from file\n" << std::endl;
    }

    file.close();
}

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed\n" << std::endl;
        return 1;
    }

    gpioSetMode(FAN_GPIO, PI_OUTPUT); // Set fan GPIO pin as output

    while (keepRunning) {
        CheckCO2AndControlFan();
    }

    gpioTerminate();
    return 0;
}

