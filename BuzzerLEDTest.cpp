#include <iostream>
#include <pigpio.h>
#include <chrono>
#include <thread>

void controlLEDandBuzzer(int ledPin, int buzzerPin, int turnOffTime, int turnOnTime) {
    while (true) {
        // Turn on LED and buzzer
        gpioWrite(ledPin, 1);
        gpioWrite(buzzerPin, 1);
        std::cout << "LED and buzzer turned on\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(turnOnTime));

        // Turn off LED and buzzer
        gpioWrite(ledPin, 0);
        gpioWrite(buzzerPin, 0);
        std::cout << "LED and buzzer turned off\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(turnOffTime));
    }
}

int main() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed." << std::endl;
        return 1;
    }

    // Define GPIO pins for LED and buzzer
    int ledPin = 16;
    int buzzerPin = 18; 
    
    // Call the function to control LED and buzzer
    controlLEDandBuzzer(ledPin, buzzerPin, 1000, 1000); // Example: 1 second on, 1 second off

    gpioTerminate();
    return 0;
}
