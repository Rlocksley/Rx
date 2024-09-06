cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Rx_Test)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(./core)
add_subdirectory(./component)

# Add the executable
add_executable(Rx_Test
    flecs.c
    RxWorld.cpp
)

# Include directories
target_include_directories(Rx_Test PRIVATE ./core ./component )

# Link the library
target_link_libraries(Rx_Test PRIVATE neon_core)