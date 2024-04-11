#include <iostream>
#include <unistd.h>
#include <pigpio.h>
extern "C" {
#include <lcd1602.h>
}
#include <cstring>
#include <cstdio>

// Interrupt callback function
void gpioInterruptCallback(int gpio, int level, uint32_t tick) {
    char message[17]; // LCD can typically display 16 characters per line

    if (level == 1) {
        snprintf(message, sizeof(message), "GPIO %d: HIGH ", gpio);
    } else if (level == 0) {
        snprintf(message, sizeof(message), "GPIO %d: LOW  ", gpio);
    } else {
        snprintf(message, sizeof(message), "GPIO %d: UNKNOWN", gpio);
    }

    lcd1602SetCursor(0, 0);
    lcd1602WriteString(message);
}

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return 1;
    }

    // Initialize LCD
    if (lcd1602Init(1, 0x3f) != 0) {
        std::cerr << "LCD initialization failed" << std::endl;
        gpioTerminate();
        return 1;
    }

    // Set GPIO 4 as input with pull-up
    gpioSetMode(4, PI_INPUT);
    gpioSetPullUpDown(4, PI_PUD_UP);

    // Set up interrupt callback for both rising and falling edges
    gpioSetISRFunc(4, EITHER_EDGE, 0, gpioInterruptCallback);

    std::cout << "Press Enter to quit" << std::endl;
    std::cin.get();

    lcd1602Shutdown();
    gpioTerminate(); // Properly terminate pigpio use
    return 0;
}
