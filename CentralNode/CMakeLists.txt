cmake_minimum_required(VERSION 3.0)
project(CHDS_Central_Node)

# Add executable target
add_executable(main 
    main.cpp 
    TCPServer.cpp 
    FileHandler.cpp 
    ZoneHandler.cpp
)

# Find and link pthread library
find_package(Threads REQUIRED)
target_link_libraries(main PRIVATE Threads::Threads)

# Link pigpio library
target_link_libraries(main PRIVATE pigpio)
