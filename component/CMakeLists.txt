cmake_minimum_required(VERSION 3.16.3)

project(neon)

add_library(neon_component STATIC
transform.hpp
transform.cpp
colorMesh.hpp
colorMesh.cpp
colorGraphics.hpp
colorGraphics.cpp
flyingCamera.hpp
velocity.hpp
velocity.cpp
mesh.hpp
mesh.cpp
)

target_include_directories(neon_component PUBLIC ../core)
target_link_libraries(neon_component PUBLIC glfw vulkan neon_core)