

#pragma once

#include "Global.hpp"
#include "flecs.h"
#include "Level.hpp"
#include "TextureMeshArray.hpp"
#include "VkTextureMeshArray.hpp"
#include "TextureArray.hpp"
#include "VkTextureArray.hpp"
#include "TextureMaterialBuffer.hpp"
#include "VkTextureMaterialBuffer.hpp"
#include "VkTransformBuffer.hpp"
#include "VkTextureModelDescriptorSet.hpp"
#include "Shape.hpp"
#include "LoadTexture.hpp"
#include "IndirectBuffer.hpp"
#include "VkIndirectBuffer.hpp"
#include "Transform.hpp"
#include "DirectionalLight.hpp"
#include "GameWorldBase.hpp"
#include "GameWorld.hpp"
#include "ColorMesh.hpp"
#include "VkColorMesh.hpp"
#include "VkColorModelBuffer.hpp"
#include "VkColorModelDescriptorSet.hpp"

struct Actors;
class RxLevel : public Rx::Level
{
public:
     // Constructor
        RxLevel() = default;

        // Destructor
        virtual ~RxLevel() = default;

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


struct Actors{

    Actors(flecs::world& world){

        world.module<Actors>();

        auto light = world.entity("Light");
        light.set<Rx::Component::DirectionalLight>({
            glm::vec3(1.f, 1.f, 1.f),  // Color
            100.f,                     // Intensity
        });
        light.set<Rx::Component::Transform>({
            glm::vec3(1.f,1.f,1.f), // Scale
            -3.14/4.f,                     // Angle
            glm::normalize(glm::vec3(1.f, 1.f, 0.f)), // Axis
            glm::vec3(0.f, 0.f, 0.f)  //
        });

        Rx::Shape::ColorCube cube(glm::vec3(100.f, 1.f, 100.f), glm::vec4(0.f, 1.f, 0.f,1.f));

        //Rx::Shape::ColorSphere sphere(1.f, 16, 16, glm::vec3(1.f, 0.f, 0.f));


        auto e = world.entity();
        e.set<Rx::Component::ColorMesh>({ cube.getVertices(), cube.getIndices() });
        e.add<Rx::Component::VkColorMesh>();
        e.add<Rx::Component::VkColorModelBuffer>();
        e.add<Rx::Component::VkColorModelDescriptorSet>();
        e.set<Rx::Component::Transform>({ glm::vec3(1.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, -2.f, 0.f) });
        e.set<Rx::Component::Material>({ glm::vec4(1.f, 0.f, 0.f, 1.f), 0.5f, 0.5f, glm::vec3(0.f) });
   

        auto asset = world.entity("TextureModelAsset");

        Rx::Component::TextureMeshArray meshArray;
        Rx::Shape::TextureCube cube0(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.f, 5.f, 0.f));
        Rx::Shape::TextureSphere sphere0(1, 20, 20, glm::vec3(0.0f));
        meshArray.addMesh("Cube0", cube0.getVertices(), cube0.getIndices());
        meshArray.addMesh("Sphere0", sphere0.getVertices(), sphere0.getIndices());
        asset.set<Rx::Component::TextureMeshArray>(meshArray);
        asset.add<Rx::Component::VkTextureMeshArray>();

        Rx::Component::IndirectBuffer indirectBuffer;
        indirectBuffer.addCommand(meshArray.getCommand("Cube0"));
        indirectBuffer.addCommand(meshArray.getCommand("Sphere0"));
        indirectBuffer.setInstanceCount(0);
        asset.set<Rx::Component::IndirectBuffer>(indirectBuffer);

        Rx::Component::VkIndirectBuffer vkIndirectBuffer;
        vkIndirectBuffer.maxNumberCommands = 2; 
        vkIndirectBuffer.numberCommands = 0;
        asset.set<Rx::Component::VkIndirectBuffer>(vkIndirectBuffer);

        Rx::Component::TextureArray textureArray;
        textureArray.addTexture("CubeTexture", Rx::Asset::loadTexture("asset/fire.jpg"));
        textureArray.addTexture("SphereTexture", Rx::Asset::loadTexture("asset/water.jpg"));
        asset.set<Rx::Component::TextureArray>(textureArray);
        asset.add<Rx::Component::VkTextureArray>();
        
        Rx::Component::TextureMaterialBuffer textureMaterialBuffer;
        textureMaterialBuffer.addMaterial(textureArray.getTextureIndex("CubeTexture"), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
        textureMaterialBuffer.addMaterial(textureArray.getTextureIndex("SphereTexture"), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
        asset.set<Rx::Component::TextureMaterialBuffer>(textureMaterialBuffer);
        asset.add<Rx::Component::VkTextureMaterialBuffer>();

        Rx::Component::VkTransformBuffer transformBuffer;
        transformBuffer.maxNumberTransforms = 1024;
        transformBuffer.numberTransforms = 0;
        asset.set<Rx::Component::VkTransformBuffer>(transformBuffer);

        asset.add<Rx::Component::VkTextureModelDescriptorSet>();
        asset.add<Rx::ShouldBeUpdated>();

        auto rel = world.lookup("TextureModelInstanceRelation");

        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                auto e = world.entity();
                e.add(rel, asset);
                e.set<Rx::Component::Transform>({ glm::vec3(1.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(i * 2.f, 0.f, j * 2.f) });
                e.add<LevelAsset>();
            }
        }
    }
};