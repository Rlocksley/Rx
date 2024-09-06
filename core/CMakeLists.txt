cmake_minimum_required(VERSION 3.16.3)

project(neon)

add_library(neon_core STATIC
window.hpp
window.cpp
time.hpp
time.cpp
input.hpp
input.cpp
instance.hpp
instance.cpp
surface.hpp
surface.cpp
physicalDevice.hpp
physicalDevice.cpp
device.hpp
device.cpp
queue.hpp
queue.cpp
command.hpp
command.cpp
allocator.hpp
allocator.cpp
descriptorPool.hpp
descriptorPool.cpp
semaphore.hpp
semaphore.cpp
fence.hpp
fence.cpp
image.hpp
image.cpp
buffer.hpp
buffer.cpp
eye.hpp
eye.cpp
swapchain.hpp
swapchain.cpp
renderPass.hpp
renderPass.cpp
framebuffers.hpp
framebuffers.cpp
shader.hpp
shader.cpp
core.hpp
core.cpp)

target_link_libraries(neon_core PUBLIC glfw vulkan)