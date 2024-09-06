#include "flecs.h"
#include <iostream>
#include "component/colorGraphics.hpp"
#include "core/shader.hpp"
#include "component/colorMesh.hpp"
#include "component/mesh.hpp"
#include "core/buffer.hpp"
#include "core/command.hpp"
#include "component/transform.hpp"
#include "core/core.hpp"
#include "component/flyingCamera.hpp"
#include "core/window.hpp"
#include "core/input.hpp"

inline Neon::Core::Shader vertexShader("./shader/testShader3.vert.spv");
inline Neon::Core::Shader fragmentShader("./shader/testShader3.frag.spv");

void graphics_component_on_add(flecs::entity e, Neon::Component::ColorGraphics& graphics)
{
 
    graphics.createModelBuffer();
    graphics.createDescriptorSet();
    graphics.createGraphicsPipeline
    (vertexShader,
    fragmentShader);
}

void graphics_component_on_remove(flecs::entity e, Neon::Component::ColorGraphics& graphics)
{
    graphics.destroyGraphicsPipeline();
    graphics.destroyDescriptorSet();
    graphics.destroyModelBuffer();
}


void mesh_component_on_add(flecs::entity e, Neon::Component::ColorMesh& mesh)
{

    const Neon::Component::Mesh* pMesh = e.get<Neon::Component::Mesh>();


    Neon::Core::BufferInterface vertexInterface = 
    Neon::Core::createBufferInterface
    (pMesh->vertices.size(), sizeof(Neon::Vertex::Color),
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

    Neon::Core::BufferInterface indexInterface = 
    Neon::Core::createBufferInterface
    (pMesh->indices.size(), sizeof(uint32_t),
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

    mesh.vertexBuffer = 
    Neon::Core::createBuffer
    (pMesh->vertices.size(), sizeof(Neon::Vertex::Color),
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    mesh.indexBuffer = 
    Neon::Core::createBuffer
    (pMesh->indices.size(), sizeof(uint32_t),
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    memcpy(vertexInterface.pMemory, pMesh->vertices.data(), pMesh->vertices.size()*sizeof(Neon::Vertex::Color));
    memcpy(indexInterface.pMemory, pMesh->indices.data(), pMesh->indices.size()*sizeof(uint32_t));

    VkBufferCopy vertexCopy;
    vertexCopy.srcOffset = 0;
    vertexCopy.dstOffset = 0;
    vertexCopy.size = pMesh->vertices.size()*sizeof(Neon::Vertex::Color);

    VkBufferCopy indexCopy;
    indexCopy.srcOffset = 0;
    indexCopy.dstOffset = 0;
    indexCopy.size = pMesh->indices.size()* sizeof(uint32_t);

    Neon::Core::beginSingleCommand(Neon::Core::single_command);

    vkCmdCopyBuffer
    (Neon::Core::single_command.vkCommandBuffer,
    vertexInterface.vkBuffer,
    mesh.vertexBuffer.vkBuffer,
    1, &vertexCopy);

    vkCmdCopyBuffer
    (Neon::Core::single_command.vkCommandBuffer,
    indexInterface.vkBuffer,
    mesh.indexBuffer.vkBuffer,
    1, &indexCopy);

    Neon::Core::endSingleCommand(Neon::Core::single_command);
    Neon::Core::submitSingleCommand(Neon::Core::single_command);

    Neon::Core::destroyBufferInterface
    (indexInterface);
    Neon::Core::destroyBufferInterface
    (vertexInterface);
}

void mesh_component_on_remove(flecs::entity e, Neon::Component::ColorMesh& mesh)
{
    
    Neon::Core::destroyBuffer
    (mesh.indexBuffer);

    Neon::Core::destroyBuffer
    (mesh.vertexBuffer);

}


struct Graphics{

    Graphics(flecs::world& world){
        world.component<Neon::Component::ColorGraphics>("ColorGraphics")
        .on_add(graphics_component_on_add)
        .on_remove(graphics_component_on_remove);

        world.component<Neon::Component::ColorMesh>("ColorMesh")
        .on_add(mesh_component_on_add)
        .on_remove(mesh_component_on_remove);   


        world.component<Neon::Component::Mesh>("Mesh");
        world.component<Neon::Component::Transform>("Transform");

        world.component<Neon::Component::FlyingCamera>("FlyingCamera").set<Neon::Component::FlyingCamera>({
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec2(0.f, 0.f),
            10.f,
            0.01f,
            Neon::Core::windowHeight/static_cast<float>(Neon::Core::windowWidth),
            0.1f,
            1000.f
        });

        world.system<Neon::Component::FlyingCamera>().kind(flecs::OnUpdate).each([](flecs::entity e, Neon::Component::FlyingCamera& camera){
            camera.update();
        });


        world.system<Neon::Component::ColorGraphics, Neon::Component::ColorMesh, Neon::Component::Transform>().kind(flecs::OnUpdate).run([](flecs::iter& it){
            Neon::Core::beginCommand(Neon::Core::command[Neon::Core::commandIndex]);
            Neon::Core::beginRenderPass(Neon::Core::command[Neon::Core::commandIndex]);
            while(it.next()){
                auto graphics = it.field<Neon::Component::ColorGraphics>(0);
                auto meshes = it.field<Neon::Component::ColorMesh>(1);
                auto transforms = it.field<Neon::Component::Transform>(2);

            for(auto i : it)
            {
                graphics[i].setModelBuffer(transforms[i].getTransformMatrix());

                vkCmdBindPipeline
                (Neon::Core::command[Neon::Core::commandIndex].vkCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                graphics[i].getGraphicsPipeline());

                VkBuffer vertexBuffer = meshes[i].getVertexBuffer();
                VkDeviceSize offset[] = {0};
                vkCmdBindVertexBuffers
                (Neon::Core::command[Neon::Core::commandIndex].vkCommandBuffer,
                0,1,&vertexBuffer,
                offset);

                vkCmdBindIndexBuffer
                (Neon::Core::command[Neon::Core::commandIndex].vkCommandBuffer,
                meshes[i].getIndexBuffer(),
                0, VK_INDEX_TYPE_UINT32);

                VkDescriptorSet descriptorSet = graphics[i].getDescriptorSet();
                vkCmdBindDescriptorSets
                (Neon::Core::command[Neon::Core::commandIndex].vkCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                graphics[i].getPipelineLayout(),
                0,1,
                &descriptorSet,
                0, nullptr);

                vkCmdDrawIndexed
                (Neon::Core::command[Neon::Core::commandIndex].vkCommandBuffer,
                meshes[i].getNumberIndices(),1,0,0,0);
            }

        
        }

        Neon::Core::endRenderPass(Neon::Core::command[Neon::Core::commandIndex]);
        Neon::Core::endCommand(Neon::Core::command[Neon::Core::commandIndex]);
        Neon::Core::submitGraphicsCommand(Neon::Core::command[Neon::Core::commandIndex]);
        Neon::Core::presentGraphics(Neon::Core::command[Neon::Core::commandIndex]);
    });

    

}


};


struct Actors{
    Actors(flecs::world& world){
              std::vector<Neon::Vertex::Color> cubeVertices(8);
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

        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 20; j++){
                auto e = world.entity();
                e.set<Neon::Component::Mesh>({cubeVertices, cubeIndices});
                e.add<Neon::Component::ColorMesh>();
                e.add<Neon::Component::ColorGraphics>();
                e.set<Neon::Component::Transform>({glm::vec3(1.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(i*3.f, 0.f, j*3.f)});
        }
    }

    }
};


struct Move{
    Move(flecs::world& world){
        world.system<Neon::Component::Transform>().kind(flecs::OnUpdate).run([](flecs::iter& it){


            while(it.next()){
                auto transform = it.field<Neon::Component::Transform>(0);
                for(auto i : it){
                    transform[i].angle += 0.01f;
                    transform[i].translation = 10.f * glm::vec3(0.f,glm::sin(transform[i].angle+transform[i].translation[2]/10.0),0.f);
                }    
            }
        });
    }
};

int main(){

  

    Neon::Core::windowTitle = "RxWorld";
    Neon::Core::windowHeight = 600;
    Neon::Core::windowWidth = 1200;
    Neon::Core::vkSampleCountFlagBits = VK_SAMPLE_COUNT_4_BIT;
    Neon::Core::createCore();

    {

    flecs::world world;

    world.import<Graphics>();
    world.import<Actors>();
    world.import<Move>();
    
    

    while(Neon::Core::updateCore() && Neon::Input::keyEsc.down == false)
    {
        Neon::Core::getSwapchainImageIndex();

        world.progress();
    }
     vkDeviceWaitIdle(Neon::Core::vkDevice);
    }

    Neon::Core::destroyCore();
}
