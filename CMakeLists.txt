# Rx/CMakeLists.txt

# Set the minimum required version of CMake.
cmake_minimum_required(VERSION 3.15)

# Define the project name, version, and language. This will be the name of the .sln file.
project(RxProject VERSION 1.0 LANGUAGES CXX)

# --- Find External Dependencies ---
# Use find_package to locate the necessary libraries. CMake will fail if they are not found.
# For this to work, you must either install these libraries to a system path
# or tell CMake where to find them using CMAKE_PREFIX_PATH (see instructions below).

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)



# Find the Vulkan SDK. The official SDK sets an environment variable that CMake uses.
find_package(Vulkan REQUIRED)

# Find the GLFW3 library.
find_package(glfw3 REQUIRED)

# Find the GLM mathematics library.
find_package(glm REQUIRED)

find_package(Jolt REQUIRED)

find_package(assimp REQUIRED)

# --- Build Internal Libraries ---
# Process the subdirectories to create our 'component_lib' and 'core_lib' targets.
add_subdirectory(./flecs)
add_subdirectory(./core)
add_subdirectory(./shape)
add_subdirectory(./component)
add_subdirectory(./application)
add_subdirectory(./asset)

add_subdirectory(./converter)
add_subdirectory(./analyzer)

add_subdirectory(./examples/example1)
add_subdirectory(./examples/example2)
add_subdirectory(./examples/example3)
add_subdirectory(./examples/example4)
add_subdirectory(./examples/example5)
add_subdirectory(./examples/example6)
add_subdirectory(./examples/example7)
add_subdirectory(./examples/example8)
add_subdirectory(./examples/example9)
add_subdirectory(./examples/example10)
add_subdirectory(./examples/example11)
add_subdirectory(./examples/example12)