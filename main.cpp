#include <iostream>
#include "SensorReader.h"

int main() {
    SensorReader sensor;
    sensor.startReadingThread();
    std::cout << "Press Enter to quit" << std::endl;
    std::cin.get();
    sensor.stopReadingThread();
    return 0;
}
