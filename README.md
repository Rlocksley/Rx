# Rx
C++ Game Engine powered by flecs, Vulkan and Jolt Physics


### Platforms
1.] Windows \
2.] Linux \
Just clone vcpkg and install glm, glfw, vulkan and Jolt Physics and build with CMake


### Examples
- `example1`: basic PBR cube grid rendered with a single draw per mesh, illuminated by a directional light.
- `example2`: noise-driven voxel terrain generator using instanced color cubes with lightweight material variation and tree placement.
- `example3`: physics sandbox showcasing rigid bodies, kinematic movers, and instanced rendering that updates from the physics step.
- `example4`: third-person combat prototype with a flying camera, projectile spawning, collision handling, and procedural shard debris.
- `example5`: batched textured models (cube and sphere) rendered through indirect drawing and shared GPU buffers.
- `example6`: loads a textured wizard model asset and replicates it via instancing while flying the camera through the scene.
- `example7`: crowds of animated skeleton characters driven by the animation state machine and shared skeleton buffers.
- `example8`: full gameplay slice with NPC controllers, fireball combat, physics sensors, and camera-follow logic.
- `example9`: color-mesh shadow demo lit by four shadow spot lights with rotating instances.
- `example10`: color mesh array instancing with indirect draw commands, shadowed spot lights, and per-instance animation.
- `example11`: textured instancing plus the animated wizard character integrated with shadow-capable descriptor sets.
- `example12`: combines textured instancing, physics, and the controllable wizard to stress-test the shadow pipelines.
