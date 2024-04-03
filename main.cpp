#include <iostream>
#include "CO2Sensor.h"

int main() {
    CO2Sensor sensor;
    sensor.startReadingThread();
    std::cout << "Press Enter to quit" << std::endl;
    std::cin.get();
    sensor.stopReadingThread();
    return 0;
}
