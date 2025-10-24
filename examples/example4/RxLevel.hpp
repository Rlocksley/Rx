#pragma once
#include "GameWorld.hpp"
#include "Level.hpp"
#include "Input.hpp"
#include "FlyingCamera.hpp"
#include "MeshArray.hpp"
#include "Transform.hpp"
#include "VkIndirectBuffer.hpp"
#include "VkColorMeshArray.hpp"
#include "VkInstancedColorModelBuffer.hpp"
#include "VkColorModelArrayDescriptorSet.hpp"
#include "Material.hpp"
#include "ColorMesh.hpp"
#include "VkColorMesh.hpp"
#include "VkColorModelDescriptorSet.hpp"
#define random(lower, upper) ((static_cast<float>(rand())/static_cast<float>(RAND_MAX))*((upper)-(lower)) + (lower))

struct Actors;
struct FireballSystem;
struct Move;
struct LevelManager;
namespace Rx {

    class RxLevel : public Rx::Level {
    public:
        // Constructor
        RxLevel() = default;

        // Destructor
        virtual ~RxLevel() = default;

        // Called when the level is loaded
        virtual void onLoad(flecs::world& world) override {
            // Implement level-specific loading logic here
                world.import<Actors>();
                world.import<Move>();
                world.import<FireballSystem>();
                //world.import<LevelManager>();
        }

        // Called when the level is unloaded
        virtual void onUnload(flecs::world& world) override {
            // Implement level-specific unloading logic here
        }
    };

} // namespace Rx

// struct LevelManager {
//     LevelManager(flecs::world& world) {
//         world.module<LevelManager>();
        
//         world.system().kind(flecs::OnLoad).run([&](flecs::iter& it) {
        
//             while(it.next()) {
//                 auto player = it.world().entity("FlyingCamera");
//                 if (!player.has<Rx::Component::FlyingCamera>()) {
//                     RX_LOGE("RxLevel","LevelManager","Player entity does not have a FlyingCamera component.");
//                 }else{
//                     auto cam = player.get<Rx::Component::FlyingCamera>();
//                         if(glm::length(cam.position) > 1000.f){
                            
//                             static_cast<Rx::GameWorld*>(world.get_ctx())->requestLevelSwitch<Rx::RxLevel2>();
//                             RX_LOGI("RxLevel","LevelManager","Switching to RxLevel2 due to player position.");
//                         }
//                 }
            
            
//             }

//         }).add<LevelAsset>();
//     }
// };
struct Actors {
 Actors(flecs::world& world) {
        world.module<Actors>();

        // --- Define ColorMesh Data ---

        // Generic cube indices, can be reused for all cube-based meshes
        std::vector<uint32_t> cubeIndices = {
            0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 5, 4, 7, 7, 6, 5,
            4, 0, 3, 3, 7, 4, 3, 2, 6, 6, 7, 3, 4, 5, 1, 1, 0, 4
        };

        // Helper lambda to create colored cube vertices
        auto createCubeVertices = [](glm::vec4 color) {
            std::vector<Rx::Vertex::Color> v(8);
            v[0] = {glm::vec3(-0.5f, -0.5f, -0.5f), {}, color};
            v[1] = {glm::vec3( 0.5f, -0.5f, -0.5f), {}, color};
            v[2] = {glm::vec3( 0.5f,  0.5f, -0.5f), {}, color};
            v[3] = {glm::vec3(-0.5f,  0.5f, -0.5f), {}, color};
            v[4] = {glm::vec3(-0.5f, -0.5f,  0.5f), {}, color};
            v[5] = {glm::vec3( 0.5f, -0.5f,  0.5f), {}, color};
            v[6] = {glm::vec3( 0.5f,  0.5f,  0.5f), {}, color};
            v[7] = {glm::vec3(-0.5f,  0.5f,  0.5f), {}, color};
            return v;
        };

        auto grassVertices = createCubeVertices({0.2f, 0.8f, 0.2f, 1.0f});
        auto dirtVertices = createCubeVertices({0.5f, 0.35f, 0.05f, 1.0f});
        auto trunkVertices = createCubeVertices({0.4f, 0.26f, 0.13f, 1.0f});
        auto leavesVertices = createCubeVertices({0.0f, 0.5f, 0.0f, 1.0f});

        // --- Set up Batch Rendering Entity ---

        auto batchRenderEntity = world.entity("BatchRender");
        Rx::Component::ColorMeshArray meshArray;
        meshArray.addMesh("Grass", grassVertices, cubeIndices);
        meshArray.addMesh("Dirt", dirtVertices, cubeIndices);
        meshArray.addMesh("Trunk", trunkVertices, cubeIndices);
        meshArray.addMesh("Leaves", leavesVertices, cubeIndices);
        //batchRenderEntity.add<LevelAsset>();
        batchRenderEntity.set<Rx::Component::ColorMeshArray>(meshArray);
        batchRenderEntity.add<Rx::Component::VkColorMeshArray>();

        Rx::Component::VkIndirectBuffer indirectBuffer;
        indirectBuffer.maxNumberCommands = 1000000;
        indirectBuffer.numberCommands = 0;
        batchRenderEntity.set<Rx::Component::VkIndirectBuffer>(indirectBuffer);

        Rx::Component::VkInstancedColorModelBuffer colorMeshInstanceBuffer;
        colorMeshInstanceBuffer.maxNumberInstances = 1000000;
        batchRenderEntity.set<Rx::Component::VkInstancedColorModelBuffer>(colorMeshInstanceBuffer);

        batchRenderEntity.add<Rx::Component::VkColorModelArrayDescriptorSet>();

        auto rel = world.lookup("ColorMeshArrayInstanceRelation");
        const auto& commands = batchRenderEntity.get<Rx::Component::ColorMeshArray>().meshNameToCommand;


        
        // --- Generate World ---

        int landscapeSize = 100;
        for (int i = -landscapeSize; i <= landscapeSize; i++) {
            for (int j = -landscapeSize; j <= landscapeSize; j++) {
                auto grass = world.entity();
                if(random(0.f, 1.f) < 0.5f){
                    grass.add(rel, batchRenderEntity)
                     .set<VkDrawIndexedIndirectCommand>(commands.at("Grass"))
                     .set<Rx::Component::Transform>({ glm::vec3(1.f), 0.f, {0,1,0}, glm::vec3(i, 0.f, j) })
                     .set<Rx::Component::Material>({ glm::vec3(0.2f, 0.8f, 0.2f), random(0.1f,0.9f), random(0.1f,0.9f), glm::vec3(0.0f) })
                     .add<LevelAsset>();
    
                }else{
                
                    auto dirt = world.entity();
                    dirt.add(rel, batchRenderEntity)
                        .set<VkDrawIndexedIndirectCommand>(commands.at("Dirt"))
                        .set<Rx::Component::Transform>({ glm::vec3(1.f), 0.f, {0,1,0}, glm::vec3(i, 0.f, j) })
                     .set<Rx::Component::Material>({ glm::vec3(0.5f, 0.35f, 0.05f), random(0.1f,0.9f), random(0.1f,0.9f), glm::vec3(0.0f) })
                        .add<LevelAsset>();
                }
            }
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(-landscapeSize, landscapeSize);
        int treeCount = 1000;

        for (int i = 0; i < treeCount; ++i) {
            float x = static_cast<float>(distrib(gen));
            float z = static_cast<float>(distrib(gen));
            int trunkHeight = 3 + (gen() % 3);

            for (int h = 0; h < trunkHeight; ++h) {
                world.entity()
                     .add(rel, batchRenderEntity)
                     .set<VkDrawIndexedIndirectCommand>(commands.at("Trunk"))
                     .set<Rx::Component::Transform>({ glm::vec3(1.f), 0.f, {0,1,0}, glm::vec3(x, 1.f + h, z) })
                     .set<Rx::Component::Material>({ glm::vec3(0.4f, 0.26f, 0.13f), random(0.1f,0.2f), random(0.1f,0.2f), glm::vec3(0.0f) })
                     .add<LevelAsset>();
            }

            float foliageY = 1.f + trunkHeight;
            for (int lx = -1; lx <= 1; ++lx) {
                for (int lz = -1; lz <= 1; ++lz) {
                    for (int ly = 0; ly <= 1; ++ly) {
                        if (std::abs(lx) + std::abs(lz) + std::abs(ly) > 1 && ly == 1) continue;
                        world.entity()
                             .add(rel, batchRenderEntity)
                             .set<VkDrawIndexedIndirectCommand>(commands.at("Leaves"))
                             .set<Rx::Component::Transform>({ glm::vec3(1.f), 0.f, {0,1,0}, glm::vec3(x + lx, foliageY + ly, z + lz) })
                         .set<Rx::Component::Material>({ glm::vec3(0.0f, 0.5f, 0.0f), random(0.1f,0.2f), random(0.1f,0.2f), glm::vec3(0.0f) })
                             .add<LevelAsset>();
                    }
                }
            }
        }
    }
};

struct SinusRunning{};
struct Move {
    Move(flecs::world& world) {
        world.system<Rx::Component::Transform>().with<SinusRunning>().src<SinusRunning>().kind(flecs::OnUpdate)
            .run([](flecs::iter& it) {
            while (it.next()) {
                auto transform = it.field<Rx::Component::Transform>(0);
                for (auto i : it) {
                    transform[i].angle += 0.01f;
                    transform[i].translation = glm::vec3(transform[i].translation[0],
                        20.f * glm::sin(transform[i].angle * 20 + transform[i].translation[2] / 4.f),
                        transform[i].translation[2]);
                }
            }
                }).add<LevelAsset>();

        world.system<>().kind(flecs::OnUpdate).run([](flecs::iter& it) {
            // Update the loading screen UI here.
            if(Rx::Input::keyX.pressed) {
                it.world().defer_begin();
                if (it.world().has<SinusRunning>()) {
                    it.world().remove<SinusRunning>();
                } else {
                    it.world().add<SinusRunning>();
                }
                it.world().defer_end();
            }
        }).add<LevelAsset>();

    }


};

// New Fireball component
struct Fireball {
    glm::vec3 velocity;
};

// Fireball system: update position of fireballs each frame.
struct FireballSystem {
    FireballSystem(flecs::world& world) {
        world.system<Fireball, Rx::Component::Transform>().kind(flecs::OnUpdate)
            .each([](flecs::entity e, Fireball& fireball, Rx::Component::Transform& transform) {
                    transform.translation += fireball.velocity * Rx::Time::deltaTime;
                }).add<LevelAsset>();
    


        world.system<Rx::Component::FlyingCamera>()
            .each([&](flecs::entity camEntity, Rx::Component::FlyingCamera& camera) {
            if(!Rx::Input::buttonLeft.pressed) {
                return; // Only create fireball if mouse button 1 is pressed
            }
            // Create a fireball entity
            auto fireball = world.entity();

            std::vector<Rx::Vertex::Color> cubeVertices(8);
            cubeVertices[0].position = glm::vec3(-1.f, -1.f, -1.f); // Front Bottom Left
            cubeVertices[0].color = glm::vec4(1.f, 0.f, 0.f, 0.1f);
            cubeVertices[1].position = glm::vec3(1.f, -1.f, -1.f);  // Front Bottom Right
            cubeVertices[1].color = glm::vec4(0.f, 1.f, 0.f, 0.1f);
            cubeVertices[2].position = glm::vec3(1.f, 1.f, -1.f);   // Front Top Right
            cubeVertices[2].color = glm::vec4(0.f, 0.f, 1.f, 0.1f);
            cubeVertices[3].position = glm::vec3(-1.f, 1.f, -1.f);  // Front Top Left
            cubeVertices[3].color = glm::vec4(1.f, 0.f, 1.f, 0.1f);
            cubeVertices[4].position = glm::vec3(-1.f, -1.f, 1.f);  // Back Bottom Left
            cubeVertices[4].color = glm::vec4(1.f, 1.f, 0.f, 0.1f);
            cubeVertices[5].position = glm::vec3(1.f, -1.f, 1.f);   // Back Bottom Right
            cubeVertices[5].color = glm::vec4(0.f, 1.f, 1.f, 0.1f);
            cubeVertices[6].position = glm::vec3(1.f, 1.f, 1.f);    // Back Top Right
            cubeVertices[6].color = glm::vec4(1.f, 1.f, 1.f, 0.1f);
            cubeVertices[7].position = glm::vec3(-1.f, 1.f, 1.f);   // Back Top Left
            cubeVertices[7].color = glm::vec4(0.f, 0.f, 0.f, 0.1f);

            std::vector<uint32_t> cubeIndices = {
                0, 1, 2, 2, 3, 0,    // Front face
                1, 5, 6, 6, 2, 1,    // Right face
                5, 4, 7, 7, 6, 5,    // Back face
                4, 0, 3, 3, 7, 4,    // Left face
                3, 2, 6, 6, 7, 3,    // Top face
                4, 5, 1, 1, 0, 4     // Bottom face
            };

            Rx::Component::Transform fireballTransform;
            fireballTransform.translation = camera.position;
            fireballTransform.scale = glm::vec3(1.f);
            fireballTransform.angle = 0.f;
            fireballTransform.axis = glm::vec3(1.f, 0.f, 0.f);

            float speed = 100.f;
            glm::vec3 velocity = camera.direction * speed;

            
            fireball.set<Rx::Component::ColorMesh>({ cubeVertices, cubeIndices });
            fireball.add<Rx::Component::VkColorMesh>();
            fireball.add<Rx::Component::VkColorModelDescriptorSet>();
            fireball.set<Rx::Component::Transform>(fireballTransform);
            fireball.set<Fireball>({ velocity });
            fireball.add<LevelAsset>();
            
            }).add<LevelAsset>();

        }
};




