# Rx/CMakeLists.txt

# Set the minimum required version of CMake.
cmake_minimum_required(VERSION 3.15)

# Define the project name, version, and language. This will be the name of the .sln file.
project(RxProject VERSION 1.0 LANGUAGES CXX)

find_package(Vulkan REQUIRED)

# Find the GLFW3 library.
find_package(glfw3 REQUIRED)

# Find the GLM mathematics library.
find_package(glm REQUIRED)

# --- Build The Main Executable ---
# Define the main executable target, "RxWorld".
add_executable(rx_example10 main.cpp)

# --- Link All Libraries ---
# Link RxWorld against all the libraries it needs. Modern CMake uses imported targets
# (like Vulkan::Vulkan, glm::glm, glfw) which also handle include directories automatically.

target_include_directories(rx_example10 PUBLIC ../../core ../../flecs ../../component  ../../application ../../shape)

target_link_libraries(rx_example10 PRIVATE
    # Our internal libraries
    rx_core
    rx_component
    rx_flecs
    rx_app
    rx_shape
    # External dependencies
    Vulkan::Vulkan  # The imported target for Vulkan
    glm::glm        # The imported target for GLM
    glfw            # The imported target for GLFW
)



