#pragma once
#include "GameWorld.hpp"
#include "Level.hpp"
#include "ShadowSpotLight.hpp"
#include "VkColorModelBuffer.hpp"
#include "ColorMesh.hpp"
#include "VkColorMesh.hpp"
#include "ColorMeshArray.hpp"
#include "VkColorMeshArray.hpp"
#include "VkColorModelDescriptorSet.hpp"
#include "VkColorModelArrayDescriptorSet.hpp"
#include "Material.hpp"
#include "Shape.hpp"
#include "VkShadowColorModelDescriptorSet.hpp"
#include "VkShadowColorModelArrayDescriptorSet.hpp"
#include "Transform.hpp"
#include "Time.hpp"
#include "VkIndirectBuffer.hpp"
#define random(lower, upper) ((static_cast<float>(rand())/static_cast<float>(RAND_MAX))*((upper)-(lower)) + (lower))

struct Actors;
struct FireballSystem;
struct LevelManager;
namespace Rx {

    class RxLevel1 : public Rx::Level {
    public:
        // Constructor
        RxLevel1() = default;

        // Destructor
        virtual ~RxLevel1() = default;

        // Called when the level is loaded
        virtual void onLoad(flecs::world& world) override {
            // Implement level-specific loading logic here
                world.import<Actors>();
        }

        // Called when the level is unloaded
        virtual void onUnload(flecs::world& world) override {
            // Implement level-specific unloading logic here
        }
    };

} // namespace Rx

struct ToMove {};

struct Actors {
 Actors(flecs::world& world) {
        world.module<Actors>();

        struct Corner{
            int x = 0;
            int y = 0;
        };
        std::vector<Corner> corners = { { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };

        for(int i = 0; i < corners.size(); i++) {
            auto light = world.entity();
            light.set<Rx::Component::ShadowSpotLight>({
                glm::vec3(1.f, 1.f, 1.f),  // Color
                20000.f,                    // Intensity
                3.14159265f/2.5f,          // ~72° FOV to cover full grid from above
                10.f,                      // Near plane (tighter for precision)
                1000.0f,                    // Far plane (fits scene extents)
                1.0f                       // Aspect ratio (square shadow map)
            });

            // Place the light above the scene and look at the center. Use a non-colinear up vector to avoid degeneracy.
            light.set<Rx::Component::Transform>
            (Rx::Component::Transform::lookAt
            (glm::vec3(corners[i].x * 100.f, 100.f, corners[i].y * 100.f),   // position above center
            glm::vec3(0.f, 0.f, 0.f),     // target at center
            glm::vec3(0.f, 1.f, 0.f)));  // up vector (not parallel to forward)
        }
        //light.set<Rx::Component::Transform>({ glm::vec3(1.f), -3.14/1.5f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 10.f, 0.f) });

        Rx::Shape::ColorSphere sphere(1.f, 16, 16, glm::vec3(0.f, 1.f, 0.f));
        Rx::Shape::ColorCube cube(glm::vec3(1.f, 1.f, 1.f), glm::vec4(1.f, 1.f, 1.f,1.f));
        Rx::Shape::ColorCube floorCube(glm::vec3(100.f, 1.f, 100.f), glm::vec4(1.f, 0.f, 1.f, 1.f));

        //Rx::Shape::ColorSphere sphere(1.f, 16, 16, glm::vec3(1.f, 0.f, 0.f));
        auto floor = world.entity();
        floor.set<Rx::Component::ColorMesh>({ floorCube.getVertices(), floorCube.getIndices() });
        floor.add<Rx::Component::VkColorMesh>();
        floor.add<Rx::Component::VkColorModelBuffer>();
        floor.add<Rx::Component::VkColorModelDescriptorSet>();
        floor.add<Rx::Component::VkShadowColorModelDescriptorSet>();
        floor.set<Rx::Component::Transform>({ glm::vec3(1.f), 0.f, glm::vec3(0.f, 1.f, 0.f), 
        glm::vec3(0.f, 0.f, 0.f) });
        floor.set<Rx::Component::Material>({ glm::vec4(1.f, 0.f, 0.f, 1.f), 5/10.f, 5/10.f, glm::vec3(0.f) });

        auto batchRenderEntity = world.entity("BatchRender");
        Rx::Component::ColorMeshArray meshArray;
        meshArray.addMesh("Cube", cube.getVertices(), cube.getIndices());
        meshArray.addMesh("Sphere", sphere.getVertices(), sphere.getIndices());
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
        batchRenderEntity.add<Rx::Component::VkShadowColorModelArrayDescriptorSet>();

        auto rel = world.lookup("ColorMeshArrayInstanceRelation");
        const auto& commands = batchRenderEntity.get<Rx::Component::ColorMeshArray>().meshNameToCommand;

        for(int i = -5 ; i < 5; i++){
            for(int j = -5; j < 5; j++){
                for(int k = 0; k < 10; k++){
                    auto e = world.entity();
                    e.add(rel, batchRenderEntity);
                    e.set<VkDrawIndexedIndirectCommand>(commands.at("Cube"));
                    e.set<Rx::Component::Transform>({ glm::vec3(1.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f,4.f,0.f) + glm::vec3(i * 9.f,  k * 9.f, j * 9.f) });
                    e.set<Rx::Component::Material>({ glm::vec4(1.f, 0.f, 0.f, 1.f), i/10.f, j/10.f, glm::vec3(0.f) });
                    e.add<ToMove>();
                }
            }
        }

        // for(int i = -5 ; i < 5; i++){
        //     for(int j = -5; j < 5; j++){
        //         for(int k = -5; k < 5; k++){
        //             auto e = world.entity();
        //             e.add(rel, batchRenderEntity);
        //             e.set<VkDrawIndexedIndirectCommand>(commands.at("Sphere"));
        //             e.set<Rx::Component::Transform>({ glm::vec3(1.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f,4.f,0.f) + glm::vec3(i * 7.f, 0.f, j * 7.f) });
        //             e.set<Rx::Component::Material>({ glm::vec4(1.f, 0.f, 0.f, 1.f), i/10.f, j/10.f, glm::vec3(0.f) });
        //             e.add<ToMove>();
        //         }
        //     }
        // }

        world.system<Rx::Component::Transform, const ToMove>("MoveMeshes")
            .with<ToMove>()
            .kind(flecs::OnUpdate)
            .each([](flecs::entity e, Rx::Component::Transform& transform, const ToMove& move) {
                    transform.angle += Rx::Time::deltaTime;
                });
    }
};




