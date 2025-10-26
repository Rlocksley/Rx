# Rx
A lightweight C++ game engine and demos built on top of flecs (ECS), Vulkan (rendering), and Jolt Physics (physics). This repo contains a set of increasingly complex examples that show how the engine's systems (rendering, materials, instancing, animation, physics, input, and gameplay) fit together.

Platforms
- Windows and Linux supported.
- Build with CMake and vcpkg (install glm, glfw, Vulkan SDK, Assimp, and Jolt Physics).

Quick start
1. Clone repository: git clone https://github.com/Rlocksley/Rx.git
2. Install dependencies via vcpkg and point CMake to the toolchain.
3. Build: mkdir build && cd build && cmake .. && cmake --build .

What you'll find here
- A series of numbered examples (example1 .. example12). Each example is a self-contained small application that demonstrates a particular set of features.

Contributing
- Issues and PRs welcome. I try to build a Fantasy-RPG.
- Keep systems modular: rendering, animation, physics, and gameplay should be decoupled via ECS systems.
