"# Centralized-Hazard-Detection-System-"

## To run the thread-controller.cpp file run following commands

'''
g++ -fopenmp thread-controller.cpp -o controller

./controller

'''

1. Hardware Requirements:
    1.1. Main Node:
        1.1.1.	Raspberry Pi 3 Model B
        1.1.2.	MicroSD card (with Raspbian OS installed)
        1.1.3.	2 x 5V DC fans
        1.1.4.	2 x 2N2222 NPN Transistors
        1.1.5.	2 x 1N4001 Diodes (for reverse polarity protection)
        1.1.6.	2 x 220ohm Resistors

    1.2. Edge Node:
        1.2.1.	Raspberry Pi 4 Model B
        1.2.2.	MicroSD card (with Raspbian OS installed)
        1.2.3.	SCD40 Module (I2C-compatible CO2, temperature, and humidity sensor)
        1.2.4.	Hw-131 Power Module
        1.2.5.	I2C-compatible LCD
        1.2.6.	2x 220m Ohm Resistors
        1.2.7.	Red LED (for hazard indication)
        1.2.8.	Blue LED (for Wi-Fi connectivity indication)
        1.2.9.	3.3V Active Buzzer


