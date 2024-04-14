cmake_minimum_required(VERSION 3.0)
project(CHDS_EDGE_Node)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

# Add library target
add_library(lcd1602 lcd1602.c)

# Add executable target
add_executable(main 
    main.cpp 
    Client.cpp 
    SensorReader.cpp 
    WifiManager.cpp 
    scd4x_i2c.c 
    sensirion_common.c 
    sensirion_i2c_hal.c 
    sensirion_i2c.c
)

# Link libraries
target_link_libraries(main PRIVATE pigpio lcd1602 rt pthread)

# Set include directories
target_include_directories(main PRIVATE .)

# Set library installation paths
install(TARGETS lcd1602 DESTINATION /usr/local/lib)
install(FILES lcd1602.h DESTINATION /usr/local/include)

# Note: This installation will require sudo access when running `make install`
