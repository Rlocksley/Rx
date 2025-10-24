# Rx
A lightweight C++ game engine and demos built on top of flecs (ECS), Vulkan (rendering), and Jolt Physics (physics). This repo contains a set of increasingly complex examples that show how the engine's systems (rendering, materials, instancing, animation, physics, input, and gameplay) fit together.

Platforms
- Windows and Linux supported.
- Build with CMake and vcpkg (install glm, glfw, Vulkan SDK, and Jolt Physics).

Quick start
1. Clone repository: git clone https://github.com/Rlocksley/Rx.git
2. Install dependencies via vcpkg and point CMake to the toolchain.
3. Build: mkdir build && cd build && cmake .. && cmake --build .

What you'll find here
- A series of numbered examples (example1 .. example12). Each example is a self-contained small application that demonstrates a particular set of features.

Examples with short code samples
Note: these snippets are illustrative — they show the key calls used in each example, not full files.

example1 — Basic PBR cube grid
Description: A grid of PBR cubes rendered efficiently with one draw per mesh and a single directional light.
C++ snippet:

```cpp
// setup: create camera, light, and a single cube mesh
Engine engine = Engine::Create();
auto camera = engine.World.create<CameraComponent>(...);
auto dirLight = engine.World.create<DirectionalLight>(direction, color);
Mesh cubeMesh = Mesh::CreateCubePBR();

// instantiate grid
for (int x=0;x<10;++x) for (int y=0;y<10;++y) {
  Entity e = engine.World.create();
  e.emplace<Transform>(vec3(x*2.0f, 0, y*2.0f));
  e.emplace<MeshInstance>(cubeMesh, defaultPBRMaterial);
}

engine.Run();
```

example2 — Noise-driven voxel terrain
Description: Procedural terrain using noise to place colored voxel cubes and a lightweight instanced material.
C++ snippet:

```cpp
Noise noise(seed);
for (int x=0; x<w; ++x) for (int z=0; z<d; ++z) {
  float h = noise.FractalBrownianMotion(x*0.05f, z*0.05f) * maxHeight;
  placeVoxel(x,z,(int)h);
}

// placeVoxel collects instance transforms and colors, then submits a single instanced draw
instancer.SubmitInstances(cubeMesh, instanceTransforms, instanceColors);
```

example3 — Physics sandbox
Description: Rigid bodies, kinematic movers, and instanced rendering updated from the physics step.
C++ snippet:

```cpp
// create dynamic rigid body
BodyDesc bd; bd.type = BodyType::Dynamic; bd.shape = Box(vec3(1));
RigidBody* box = physicsWorld.CreateBody(bd);

// attach render instance
Entity e = world.create();
e.emplace<RenderFromPhysics>(box->GetId());

// simulate step
physicsWorld.Step(deltaTime);
// system: sync physics -> render transforms
SyncPhysicsToRenderSystem(world, physicsWorld);
```

example4 — Third-person combat prototype
Description: Controllable character, flying camera, projectile spawning, collisions, and procedural shard debris.
C++ snippet:

```cpp
// spawn projectile on input
void OnFire() {
  Entity proj = world.create();
  proj.emplace<Transform>(playerPos + forward*1.5f);
  proj.emplace<Projectile>(speed, damage);
  proj.emplace<RigidBody>(MakeSphere(0.2f), BodyType::Dynamic);
}

// collision event: spawn shards
OnCollision(proj, hit) { SpawnShards(hit.position); }
```

example5 — Batched textured models with indirect drawing
Description: Cubes and spheres rendered through indirect draw commands and shared GPU buffers.
C++ snippet:

```cpp
// build a large GPU buffer containing vertex data for several models
SharedBuffer modelBuffer = BuildSharedModelBuffer({cube, sphere});

// record an indirect command buffer for every visible instance
IndirectBuffer commands = BuildIndirectCommands(instances);
vkCmdExecuteIndirect(cmdBuf, commands);
```

example6 — Textured wizard model with instancing
Description: Load a textured character model and replicate it with GPU instancing; fly camera through scene.
C++ snippet:

```cpp
Model wizard = AssetLoader::LoadModel("wizard.gltf");
for (int i=0;i<100;++i) instancer.AddInstance(wizard.meshes, transformFor(i));
stancer.Flush();
```

example7 — Crowds of animated skeleton characters
Description: Many skeletons driven by a shared skeleton buffer and a small animation state machine.
C++ snippet:

```cpp
SkeletonBuffer sharedSkeleton = BakeSharedSkeleton(wizard.skeleton);
for (auto& actor : crowd) {
  actor.animation.AssignStateMachine(walkRunBlend);
  actor.skeletonBuffer = sharedSkeleton;
}
AnimationSystem.Step(dt);
```

example8 — Full gameplay slice
Description: NPC controllers, fireball combat, physics sensors, and camera-follow logic integrated into a playable demo.
C++ snippet:

```cpp
npcController.Update(npcEntity, dt);
if (player.Fired()) SpawnFireball(player.Position(), player.Forward());
camera.Follow(playerEntity, {distance:6, height:2});
```

example9 — Shadowed color-mesh demo
Description: Color meshes lit by four spot lights that cast shadows; rotating instances.
C++ snippet:

```cpp
for (int i=0;i<instances;++i) {
  transforms[i] = RotateY(instanceAngle + i*0.1f);
}
shadowRenderer.RenderShadowMaps(spotLights);
mainRenderer.DrawOpaque(objects, shadowMaps);
```

example10 — Indirect instanced animation with shadows
Description: Per-instance animation blended on GPU, drawn with indirect commands and shadow mapping.
C++ snippet:

```cpp
// compute per-instance animation data (pose matrices) into a GPU buffer
ComputeInstancePosesGPU(animationTextures, instanceParams);
vkCmdDrawIndirect(...); // uses per-instance pose buffer in vertex shader
```

example11 — Textured instancing + animated wizard
Description: Combined textured instancing with the animated wizard character using descriptor sets that support shadows.
C++ snippet:

```cpp
descriptorSet.Bind(materialTextures, shadowMapSampler, skeletonBuffer);
instancedPipeline.Bind(cmdBuf);
instancedPipeline.DrawInstanced(instanceCount);
```

example12 — Stress test: instancing + physics + controllable character
Description: A heavier scene that combines instanced rendering, physics, and player control to push the shadow and sync systems.
C++ snippet:

```cpp
// physics-heavy spawn loop
for (int i=0;i<1000;++i) {
  SpawnPhysicalInstance(randomPos(), randomModel());
}
// run frame: physics -> sync -> render
physicsWorld.Step(dt);
SyncPhysicsToRenderSystem(world, physicsWorld);
renderer.DrawFrame();
```

Contributing
- Issues and PRs welcome. If you add an example, follow the naming convention exampleNN and keep the app self-contained.
- Keep systems modular: rendering, animation, physics, input and gameplay should be decoupled via ECS systems.

License
- MIT — see LICENSE file.
