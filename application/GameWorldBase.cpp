#include "GameWorldBase.hpp"
#include "VkColorMesh.hpp"
#include "VkColorModelDescriptorSet.hpp"
#include "ColorMesh.hpp"
#include "Transform.hpp"
#include "MeshTransform.hpp"
#include "VkInstancedColorModelBuffer.hpp"
#include "VkColorMeshArray.hpp"
#include "ColorMeshArray.hpp"
#include "VkColorModelArrayDescriptorSet.hpp"
#include "VkIndirectBuffer.hpp"
#include "ColorModelInstance.hpp"
#include "VkInstancedColorModelDescriptorSet.hpp"
#include "PipelineLayouts.hpp"
#include "Pipelines.hpp"
#include "PointLight.hpp"
#include "DirectionalLight.hpp"
#include "VkTextureMeshArray.hpp"
#include "VkTextureModelDescriptorSet.hpp"
#include "VkTextureMeshArray.hpp"
#include "VkTextureMaterialBuffer.hpp"
#include "TransformInstance.hpp"
#include "SkeletonBuffer.hpp"
#include "AnimationStateMachine.hpp"
#include "VkSkeletonArrayBuffer.hpp"
#include "NodeTransform.hpp"
#include "VkSkeletonMeshArray.hpp"
#include "VkSkeletonModelDescriptorSet.hpp"
#include "VkSkeleton.hpp"
#include "VkKeyFrameArrayBuffer.hpp"
#include "VkSkeletonModelCompDescriptorSet.hpp"
#include "KeyFrameBuffer.hpp"
#include "Skeleton.hpp"
#include "CharacterCapsule.hpp"
#include "CharacterController.hpp"
#include "ShadowSpotLight.hpp"
#include "VkShadowColorModelDescriptorSet.hpp"
#include "VkShadowColorModelArrayDescriptorSet.hpp"
#include "Eye.hpp"
#include "VkShadowTextureModelDescriptorSet.hpp"
#include "VkShadowSkeletonModelDescriptorSet.hpp"

namespace Rx{

        void GameWorldBase::loadGlobal() {
            world.set_ctx(this);
            int thread_count = std::thread::hardware_concurrency() - 1;
            world.set_threads(thread_count);
            
            registerComponentsBase();
            registerObserversBase();

            game = world.entity("Game");
            game.add<RenderRunning>();

            registerGraphicsBase();

        };

    void GameWorldBase::registerComponentsBase() {
        
            world.component<RenderRunning>();
            world.component<Rx::Component::Transform>();

            world.component<Rx::Component::VkColorModelDescriptorSet>();
            world.component<Rx::Component::ColorMesh>();
            world.component<Rx::Component::VkColorMesh>();
            world.component<Rx::Component::Transform>();            

            world.component<Rx::Component::ColorMeshArray>();
            world.component<Rx::Component::VkColorMeshArray>();
            world.component<Rx::Component::VkInstancedColorModelBuffer>();
            world.component<Rx::Component::VkIndirectBuffer>();
            world.component<Rx::Component::VkColorModelArrayDescriptorSet>();
            world.component<VkDrawIndexedIndirectCommand>();

            world.component<Rx::Component::TextureMeshArray>();
            world.component<Rx::Component::VkTextureMeshArray>();
            world.component<Rx::Component::TextureArray>();
            world.component<Rx::Component::VkTextureArray>();
            world.component<Rx::Component::TextureMaterialBuffer>();
            world.component<Rx::Component::VkTextureMaterialBuffer>();
            world.component<Rx::Component::VkTransformBuffer>();
            world.component<Rx::Component::VkTextureModelDescriptorSet>();
            world.component<Rx::Component::IndirectBuffer>();

        }

        void GameWorldBase::registerObserversBase(){
            world.observer<Rx::Component::VkColorModelBuffer>()
            .event(flecs::OnAdd)
            .each(model_buffer_component_on_add);

            world.observer<Rx::Component::VkColorModelBuffer>()
            .event(flecs::OnRemove)
            .each(model_buffer_component_on_remove);

            world.observer<Rx::Component::VkColorModelBuffer, Rx::Component::VkColorModelDescriptorSet>()
            .event(flecs::OnAdd)
            .each(graphics_component_on_add);
            world.observer<Rx::Component::VkColorModelDescriptorSet>()
            .event(flecs::OnRemove)
            .each(graphics_component_on_remove);

            // Ensure shadow color model descriptor sets are created/destroyed properly
            world.observer<Rx::Component::VkColorModelBuffer, Rx::Component::VkShadowColorModelDescriptorSet>()
            .event(flecs::OnAdd)
            .each(shadowColorModelDescriptorSet_component_on_add);
            world.observer<Rx::Component::VkShadowColorModelDescriptorSet>()
            .event(flecs::OnRemove)
            .each(shadowColorModelDescriptorSet_component_on_remove);

            world.observer<Rx::Component::ColorMesh, Rx::Component::VkColorMesh>()
            .event(flecs::OnAdd)
            .each(mesh_component_on_add);

            world.observer<Rx::Component::VkColorMesh>()
            .event(flecs::OnRemove)
            .each(mesh_component_on_remove);

            world.observer<Rx::Component::ColorMeshArray, Rx::Component::VkColorMeshArray>()
            .event(flecs::OnAdd)
            .each(Rx::Component::colorMeshArray_component_on_add);

            world.observer<Rx::Component::VkColorMeshArray>()
            .event(flecs::OnRemove)
            .each(Rx::Component::colorMeshArray_component_on_remove);

            world.observer<Rx::Component::VkInstancedColorModelBuffer>()
            .event(flecs::OnSet)
            .each(Rx::Component::colorMeshInstanceBuffer_on_set);

            world.observer<Rx::Component::VkInstancedColorModelBuffer>()
            .event(flecs::OnRemove)
            .each(Rx::Component::colorMeshInstanceBuffer_on_remove);

            world.observer<Rx::Component::VkIndirectBuffer>()
            .event(flecs::OnSet)
            .each(Rx::Component::indirectBuffer_component_on_set);

            world.observer<Rx::Component::VkIndirectBuffer>()
            .event(flecs::OnRemove)
            .each(Rx::Component::indirectBuffer_component_on_remove);

            world.observer<Rx::Component::VkInstancedColorModelBuffer, Rx::Component::VkColorModelArrayDescriptorSet>()
            .event(flecs::OnAdd)
            .each(Rx::Component::colorArrayGraphics_component_on_add);

            world.observer<Rx::Component::VkColorModelArrayDescriptorSet>()
            .event(flecs::OnRemove)
            .each(Rx::Component::colorArrayGraphics_component_on_remove);

            world.observer<Rx::Component::VkInstancedColorModelBuffer, Rx::Component::VkShadowColorModelArrayDescriptorSet>()
            .event(flecs::OnAdd)
            .each(Rx::Component::VkShadowColorModelArrayDescriptorSet_component_on_add);

            world.observer<Rx::Component::VkShadowColorModelArrayDescriptorSet>()
            .event(flecs::OnRemove)
            .each(Rx::Component::VkShadowColorModelArrayDescriptorSet_component_on_remove);

            world.observer<Rx::Component::VkInstancedColorModelBuffer, Rx::Component::VkInstancedColorModelDescriptorSet>()
            .event(flecs::OnAdd)
            .each(Rx::Component::instancedColorGraphics_component_on_add);

            world.observer<Rx::Component::VkInstancedColorModelDescriptorSet>()
            .event(flecs::OnRemove)
            .each(Rx::Component::instancedColorGraphics_component_on_remove);

            world.observer<Rx::Component::TextureMeshArray, Rx::Component::VkTextureMeshArray>()
            .event(flecs::OnAdd)
            .each(Rx::Component::vkTextureMeshArray_component_on_add);

            world.observer<Rx::Component::VkTextureMeshArray>()
            .event(flecs::OnRemove)
            .each(Rx::Component::vkTextureMeshArray_component_on_remove);

            world.observer<Rx::Component::TextureArray, Rx::Component::VkTextureArray>()
            .event(flecs::OnAdd)
            .each(Rx::Component::vkTextureArray_component_on_add);

            world.observer<Rx::Component::VkTextureArray>()
            .event(flecs::OnRemove)
            .each(Rx::Component::vkTextureArray_component_on_remove);

            world.observer<Rx::Component::TextureMaterialBuffer, Rx::Component::VkTextureMaterialBuffer>()
            .event(flecs::OnAdd)
            .each(Rx::Component::textureMaterialBuffer_component_on_add);

            world.observer<Rx::Component::VkTextureMaterialBuffer>()
            .event(flecs::OnRemove)
            .each(Rx::Component::textureMaterialBuffer_component_on_remove);

            world.observer<Rx::Component::VkTransformBuffer>()
            .event(flecs::OnSet)
            .each(Rx::Component::transformBuffer_component_on_set);

            world.observer<Rx::Component::VkTransformBuffer>()
            .event(flecs::OnRemove)
            .each(Rx::Component::transformBuffer_component_on_remove);

            world.observer<Rx::Component::VkTextureArray, Rx::Component::VkTextureMaterialBuffer, Rx::Component::VkTransformBuffer, Rx::Component::VkTextureModelDescriptorSet>()
            .event(flecs::OnAdd)
            .each(Rx::Component::textureModelDescriptorSet_component_on_add);

            world.observer<Rx::Component::VkTransformBuffer, Rx::Component::VkShadowTextureModelDescriptorSet>()
            .event(flecs::OnAdd)
            .each(Rx::Component::VkShadowTextureModelDescriptorSet_component_on_add);

            world.observer<Rx::Component::VkShadowTextureModelDescriptorSet>()
            .event(flecs::OnRemove)
            .each(Rx::Component::VkShadowTextureModelDescriptorSet_component_on_remove);

            world.observer<Rx::Component::VkTextureModelDescriptorSet>()
            .event(flecs::OnRemove)
            .each(Rx::Component::textureModelDescriptorSet_component_on_remove);

            world.observer<Rx::Component::SkeletonMeshArray, Rx::Component::VkSkeletonMeshArray>()
            .event(flecs::OnAdd)
            .each(Rx::Component::vkSkeletonMeshArray_component_on_add);

            world.observer<Rx::Component::VkSkeletonMeshArray>()
            .event(flecs::OnRemove)
            .each(Rx::Component::vkSkeletonMeshArray_component_on_remove);

            world.observer<Rx::Component::VkSkeletonArrayBuffer>()
            .event(flecs::OnSet)
            .each(Rx::Component::vkSkeletonArrayBuffer_component_on_set);

            world.observer<Rx::Component::VkSkeletonArrayBuffer>()
            .event(flecs::OnRemove)
            .each(Rx::Component::vkSkeletonArrayBuffer_component_on_remove);

            world.observer<Rx::Component::VkTextureArray, Rx::Component::VkTextureMaterialBuffer, Rx::Component::VkTransformBuffer, Rx::Component::VkSkeletonArrayBuffer, Rx::Component::VkSkeletonModelDescriptorSet>()
            .event(flecs::OnAdd)
            .each(Rx::Component::vkSkeletonModelDescriptorSet_component_on_add);

            world.observer<Rx::Component::VkSkeletonModelDescriptorSet>()
            .event(flecs::OnRemove)
            .each(Rx::Component::vkSkeletonModelDescriptorSet_component_on_remove);

            world.observer<Rx::Component::VkTransformBuffer, Rx::Component::VkSkeletonArrayBuffer, Rx::Component::VkShadowSkeletonModelDescriptorSet>()
            .event(flecs::OnAdd)
            .each(Rx::Component::VkShadowSkeletonModelDescriptorSet_component_on_add);

            world.observer<Rx::Component::VkShadowSkeletonModelDescriptorSet>()
            .event(flecs::OnRemove)
            .each(Rx::Component::VkShadowSkeletonModelDescriptorSet_component_on_remove);

            world.observer<Rx::Component::Skeleton>()
            .event(flecs::OnSet)
            .each(Rx::Component::skeleton_on_component_set);

            world.observer<Rx::Component::Skeleton, Rx::Component::VkSkeleton>()
            .event(flecs::OnAdd)
            .each(Rx::Component::vkSkeleton_component_on_add);

            world.observer<Rx::Component::VkSkeleton>()
            .event(flecs::OnRemove)
            .each(Rx::Component::vkSkeleton_component_on_remove);

            world.observer<Rx::Component::VkKeyFrameArrayBuffer>()
            .event(flecs::OnSet)
            .each(Rx::Component::vkKeyFrameArrayBuffer_component_on_set);

            world.observer<Rx::Component::VkKeyFrameArrayBuffer>()
            .event(flecs::OnRemove)
            .each(Rx::Component::vkKeyFrameArrayBuffer_component_on_remove);

            world.observer<Rx::Component::VkSkeleton, Rx::Component::VkKeyFrameArrayBuffer, Rx::Component::VkSkeletonArrayBuffer, Rx::Component::VkSkeletonModelCompDescriptorSet>()
            .event(flecs::OnAdd)
            .each(Rx::Component::vkSkeletonModelCompDescriptorSet_component_on_add);

           
            world.observer<Rx::Component::VkSkeletonModelCompDescriptorSet>()
            .event(flecs::OnRemove)
            .each(Rx::Component::vkSkeletonModelCompDescriptorSet_component_on_remove);

            world.observer<Rx::Component::AnimationStateMachine, Rx::Component::CharacterController>()
            .event(flecs::OnAdd)
            .each(Rx::Component::CharacterController_on_add);

        }

        void GameWorldBase::registerGraphicsBase(){

            
            // Set up render entities
            postUpdate = world.entity("PostUpdate");
            postUpdate.depends_on(flecs::OnUpdate);
            preRender = world.entity("PreRender");
            preRender.depends_on(postUpdate);
            onRecordBegin = world.entity("OnRecordBegin");
            onRecordBegin.depends_on(preRender);
            onRecordBarrier1 = world.entity("OnRecordBarrier1");
            onRecordBarrier1.depends_on(onRecordBegin);
            onRecordComp = world.entity("OnRecordComp");
            onRecordComp.depends_on(onRecordBarrier1);
            onRecordBarrier2 = world.entity("OnRecordBarrier2");
            onRecordBarrier2.depends_on(onRecordComp);
            onRecordShadow = world.entity("OnRecordShadow");
            onRecordShadow.depends_on(onRecordBarrier2);
            onRecordRenderPassBegin = world.entity("OnRecordRenderPassBegin");
            onRecordRenderPassBegin.depends_on(onRecordShadow);
            onRecordRender = world.entity("OnRecordRender");
            onRecordRender.depends_on(onRecordRenderPassBegin);
            onRecordEnd = world.entity("OnRecordEnd");
            onRecordEnd.depends_on(onRecordRender);

            colorMeshArrayInstanceRelation = world.entity("ColorMeshArrayInstanceRelation");
            instancedColorMeshRelation = world.entity("InstancedColorMeshRelation");
            textureModelInstanceRelation = world.entity("TextureModelInstanceRelation");
            IsSkeletonOf = world.entity("IsSkeletonOf");
            IsSkeletonModelInstanceOf = world.entity("IsSkeletonModelInstanceOf");
            IsAnimationOf = world.entity("IsAnimationOf");
            IsNodeOf = world.entity("IsNodeOf");
            IsRootNodeOf = world.entity("IsRootNodeOf");
            IsChildNodeOf = world.entity("IsChildNodeOf");
            IsNodeOfChild = world.entity("IsNodeOfChild");

            world.system<Component::CharacterController, Component::CharacterCapsule, Component::AnimationStateMachine, Component::Transform>()
            .each([&](flecs::entity e, Component::CharacterController& cc, Component::CharacterCapsule& capsule, Component::AnimationStateMachine& stateMachine, Component::Transform& tf) {

                cc.update(capsule, stateMachine, tf);

            });

            world.system<Component::Transform, Component::PointLight>()
            .kind(postUpdate)
            .with<RenderRunning>().src(game)
            .run([&]
            (flecs::iter& it)
            {
                Core::PointLightBuffer& pointLightBuffer = *(Core::PointLightBuffer*)Core::pointLightBuffer.pMemory;
                uint32_t bufferIndex = 0;
                while(it.next()) {
                    uint32_t index = it.count();
                    auto transforms = it.field<Component::Transform>(0);
                    auto lights = it.field<Component::PointLight>(1);

                    for(auto i : it){
                        lights[i].copyTo(transforms[i],pointLightBuffer.lights[bufferIndex++]);
                    }
                }

                pointLightBuffer.numberPointLights[0] = bufferIndex;

            });

            world.system<Component::Transform, Component::DirectionalLight>()
            .kind(postUpdate)
            .with<RenderRunning>().src(game)
            .run([&]
            (flecs::iter& it)
            {
                Core::DirectionalLightBuffer& directionalLightBuffer = *(Core::DirectionalLightBuffer*)Core::directionalLightBuffer.pMemory;
                uint32_t bufferIndex = 0;
                while(it.next()) {
                    uint32_t index = it.count();
                    auto transforms = it.field<Component::Transform>(0);
                    auto lights = it.field<Component::DirectionalLight>(1);

                    for(auto i : it){
                        lights[i].copyTo(transforms[i], directionalLightBuffer.lights[bufferIndex++]);
                    }
                }

                directionalLightBuffer.numberDirectionalLights[0] = bufferIndex;
            });

            world.system<Component::Transform, Component::ShadowSpotLight>("ShadowSpotLightUpdate")
            .kind(preRender)
            .with<RenderRunning>().src(game)
            .run([&](flecs::iter& it) {
                Core::ShadowSpotLightBuffer& shadowBuffer = 
                    *(Core::ShadowSpotLightBuffer*)Core::shadowSpotLightBuffer.pMemory;
                
                uint32_t bufferIndex = 0;
                while(it.next()) {
                    auto transforms = it.field<Component::Transform>(0);
                    auto lights = it.field<Component::ShadowSpotLight>(1);

                    for(auto i : it) {
                        if (bufferIndex >= 16) break; // max 16 shadow spot lights
                        
                        // Extract position and forward direction from transform
                        glm::vec3 position = transforms[i].translation;
                        glm::vec3 forward = transforms[i].forward(); // -Z axis typically
                        glm::vec3 up = transforms[i].up();           // Y axis
                        
                        auto [proj, view] = Core::getEyeMatrices(position, forward, up, lights[i].fov, lights[i].aspectRatio, lights[i].nearPlane, lights[i].farPlane);

                        // Combine into light-space matrix
                        glm::mat4 lightSpaceMatrix = proj * view;
                        
                        // Fill buffer
                        shadowBuffer.lights[bufferIndex].position = glm::vec4(position, 1.f);
                        shadowBuffer.lights[bufferIndex].direction = glm::vec4(forward, 0.0f);
                        shadowBuffer.lights[bufferIndex].color = glm::vec4(lights[i].color, 1.0f);
                        shadowBuffer.lights[bufferIndex].intensity = glm::vec4(lights[i].intensity, 0.0f, 0.0f, 0.0f);
                        shadowBuffer.lights[bufferIndex].lightSpaceMatrix = lightSpaceMatrix;
                        
                        bufferIndex++;
                    }
                }
                
                shadowBuffer.numberShadowSpotLights = glm::ivec4(static_cast<int>(bufferIndex), 0, 0, 0);
            });

            world.system("IndirectBufferReset")
            .with<Rx::Component::VkIndirectBuffer>()
            .with<Rx::ShouldBeUpdated>()
            .kind(postUpdate)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) { // Use .run() instead of .each()

                while(it.next()) {
                    // This system iterates over all entities with the IndirectBuffer component.
                    auto indirectBuffers = it.field<Rx::Component::VkIndirectBuffer>(0);
                    for (auto i : it) {
                        indirectBuffers[i].numberCommands = 0; // Reset the command count for the indirect buffer.
                    }
                }
            });

            world.system("VkInstancedColorModelBufferReset")
                .with<Rx::Component::VkInstancedColorModelBuffer>()
                .with<Rx::ShouldBeUpdated>()
                .kind(postUpdate)
                .with<RenderRunning>().src(game)
                .run([](flecs::iter& it) { 
                    while(it.next()) {
                        // This system iterates over all entities with the VkInstancedColorModelBuffer component.
                        auto instanceBuffers = it.field<Rx::Component::VkInstancedColorModelBuffer>(0);
                        for (auto i : it) {
                            instanceBuffers[i].numberInstances = 0; // Reset the instance count for the color mesh instance buffer.
                        }
                    }
                });

            world.system("VkTransformBufferReset")
                .with<Rx::Component::VkTransformBuffer>()
                .with<Rx::ShouldBeUpdated>()
                .kind(postUpdate)
                .with<RenderRunning>().src(game)
                .run([](flecs::iter& it) { 
                    while(it.next()) {
                        // This system iterates over all entities with the VkTransformBuffer component.
                        auto transformBuffers = it.field<Rx::Component::VkTransformBuffer>(0);
                        for (auto i : it) {
                            transformBuffers[i].numberTransforms = 0; // Reset the transform count for the transform buffer.
                        }
                    }
                });

            world.system("InstancedColorModelUpdate")
                .with<Rx::Component::Transform>()
                .with<Rx::Component::Material>()
                .with(instancedColorMeshRelation, "$parent")
                .with<ShouldBeUpdated>().src("$parent") 
                .group_by(instancedColorMeshRelation)
                .kind(preRender)
                .with<RenderRunning>().src(game)
                .run([](flecs::iter& it) {

                    uint64_t group_id = 0;
                    uint32_t instance_index = 0;

                    while(it.next()) {
                         if(group_id != it.group_id()) {
                            group_id = it.group_id();
                            instance_index = 0; 
                         }

                        // This system iterates over all entities with the Transform and VkInstancedColorModelBuffer components.
                            flecs::entity parent = it.world().entity(it.group_id());

                            RX_ASSERT(parent.is_alive(), "VkInstancedColorModelBufferUpdate", "System", "Parent entity is not alive");
                            RX_ASSERT(parent.has<Rx::Component::VkInstancedColorModelBuffer>(), "VkInstancedColorModelBufferUpdate", "System", "Parent entity does not have VkInstancedColorModelBuffer component");
                            
                            auto& instance_comp = parent.get_mut<Rx::Component::VkInstancedColorModelBuffer>();
                            auto instance_capacity = instance_comp.maxNumberInstances;
                            Rx::Component::ColorModelInstance* instances = (Rx::Component::ColorModelInstance*) instance_comp.instanceBuffer.pMemory;
                        
                            auto transforms = it.field<Rx::Component::Transform>(0);
                            auto materials = it.field<Rx::Component::Material>(1);

                            for (auto i : it) {

                                if (instance_index >= instance_capacity) {
                                    std::cerr << "Error: buffer overflow for parent "
                                            << parent.name() << "!\n";
                                    break;
                                }
                                auto transform = transforms[i].getTransformMatrix();
                                instances[instance_index].transform = transform;
                                instances[instance_index].normalTransform = glm::transpose(glm::inverse(transform));
                                auto material = materials[i];
                                instances[instance_index].albedo = glm::vec4(material.albedo, 1.0f);
                                instances[instance_index].metalRough = glm::vec4(material.metallic, material.roughness, 0.0f, 0.0f);
                                instances[instance_index].emissive = glm::vec4(material.emissive, 1.0f);

                                instance_index++;
                            }
                            instance_comp.numberInstances = instance_index;

                            parent.remove<ShouldBeUpdated>(); // Remove ShouldBeUpdated component after processing.
                        }
                    });

            world.system("ColorMeshArrayUpdate")
                .with<Rx::Component::Transform>()
                .with<VkDrawIndexedIndirectCommand>() 
                .with<Rx::Component::Material>() // This is the command type we are interested in.
                // Find entities that have a (MeshArrayInstance, *) relationship.
                .with(colorMeshArrayInstanceRelation, flecs::Wildcard) // Group by the target of the MeshArrayInstance relationship.
                .group_by(colorMeshArrayInstanceRelation)
                .kind(preRender)
                .run([](flecs::iter& it) {
                    // This part is EXACTLY the same as before.
                    // The logic is decoupled from the specific relationship type.
                    uint64_t group_id = 0;
                    uint32_t draw_index = 0;

                    while(it.next()) {
                         if(group_id != it.group_id()) {
                            group_id = it.group_id();
                            draw_index = 0; 
                         }

                        flecs::entity parent = it.world().entity(it.group_id());

                        RX_ASSERT(parent.is_alive(), "ColorMeshArrayUpdate", "System", "Parent entity is not alive");
                        RX_ASSERT(parent.has<Rx::Component::VkIndirectBuffer>(), "ColorMeshArrayUpdate", "System", "Parent entity does not have IndirectBuffer component");
                        RX_ASSERT(parent.has<Rx::Component::VkInstancedColorModelBuffer>(), "ColorMeshArrayUpdate", "System", "Parent entity does not have VkInstancedColorModelBuffer component");

                        auto& buffer_comp = parent.get_mut<Rx::Component::VkIndirectBuffer>();
                        auto& instance_comp = parent.get_mut<Rx::Component::VkInstancedColorModelBuffer>();

                        VkDrawIndexedIndirectCommand* commands_dst = (VkDrawIndexedIndirectCommand*) buffer_comp.buffer.pMemory;
                        size_t buffer_capacity = buffer_comp.maxNumberCommands;

                        Rx::Component::ColorModelInstance* instances = (Rx::Component::ColorModelInstance*) instance_comp.instanceBuffer.pMemory;
                        size_t instance_capacity = instance_comp.maxNumberInstances;

                        const auto& transforms = it.field<const Rx::Component::Transform>(0);
                        const auto& src_command = it.field<const VkDrawIndexedIndirectCommand>(1);
                        const auto& materials = it.field<Rx::Component::Material>(2);

                        uint32_t draw_index = 0;
                        for (auto i : it) {
                            if (draw_index >= buffer_capacity || draw_index >= instance_capacity) {
                                std::cerr << "Error: buffer overflow for parent " 
                                        << parent.name() << "!\n";
                                break;
                            }
                            
                            commands_dst[draw_index] = src_command[i];
                            auto transform = transforms[i].getTransformMatrix();
                            instances[draw_index].transform = transform;
                            instances[draw_index].normalTransform = glm::transpose(glm::inverse(transform));
                            auto material = materials[i];
                            instances[draw_index].albedo = glm::vec4(material.albedo, 1.0f);
                            instances[draw_index].metalRough = glm::vec4(material.metallic, material.roughness, 0.0f, 0.0f);
                            instances[draw_index].emissive = glm::vec4(material.emissive, 1.0f);

                            draw_index++;
                        }
                        buffer_comp.numberCommands = draw_index;
                        instance_comp.numberInstances = draw_index;
                    }
                });

            world.system("TextureModelUpdate")
                .with<Rx::Component::Transform>()
                .with(textureModelInstanceRelation, "$parent")
                .with<ShouldBeUpdated>().src("$parent") 
                .group_by(textureModelInstanceRelation)
                .kind(preRender)
                .run([](flecs::iter& it) {
                    uint64_t group_id = 0;
                    uint32_t instanceIndex = 0;

                    flecs::entity parent;

                    while(it.next()) {
                         if(group_id != it.group_id()) {
                            if(group_id != 0) {
                                // Finalize the previous group before starting a new one
                                auto prev_parent = it.world().entity(group_id);
                                prev_parent.get_mut<Rx::Component::VkTransformBuffer>().numberTransforms = instanceIndex;
                                auto& indirectBuffer = prev_parent.get_mut<Rx::Component::IndirectBuffer>();
                                indirectBuffer.setInstanceCount(instanceIndex);
                                prev_parent.get_mut<Rx::Component::VkIndirectBuffer>().copyFrom(indirectBuffer);
                                prev_parent.remove<ShouldBeUpdated>(); 
                            }
                            group_id = it.group_id();
                            instanceIndex = 0; 
                         }

                        parent = it.world().entity(it.group_id());

                        RX_ASSERT(parent.is_alive(), "TextureModelUpdate", "System", "Parent entity is not alive");
                        RX_ASSERT(parent.has<Rx::Component::IndirectBuffer>(), "TextureModelUpdate", "System", "Parent entity does not have IndirectBuffer component");
                        RX_ASSERT(parent.has<Rx::Component::VkIndirectBuffer>(), "TextureModelUpdate", "System", "Parent entity does not have VkIndirectBuffer component");
                        RX_ASSERT(parent.has<Rx::Component::VkTransformBuffer>(), "TextureModelUpdate", "System", "Parent entity does not have VkTransformBuffer component");


                        auto& indirectBuffer = parent.get_mut<Rx::Component::IndirectBuffer>();
                        auto& vkIndirectBuffer = parent.get_mut<Rx::Component::VkIndirectBuffer>();
                        auto& transformBuffer = parent.get_mut<Rx::Component::VkTransformBuffer>();


                        Rx::Component::TransformInstance* transformInstances = (Rx::Component::TransformInstance*) transformBuffer.buffer.pMemory;
                        size_t transformCapacity = transformBuffer.maxNumberTransforms;

                        const auto& transforms = it.field<const Rx::Component::Transform>(0);
                        
                        
                        for (auto i : it) {
                            if (instanceIndex >= transformCapacity) {
                                std::cerr << "Error: buffer overflow for parent "
                                        << parent.name() << "!\n";
                                break;
                            }
                            
                            auto transform = transforms[i].getTransformMatrix();
                            transformInstances[instanceIndex].transform = transform;
                            transformInstances[instanceIndex].normalTransform = glm::transpose(glm::inverse(transform));
                            instanceIndex++;

                        }
                    }

                    if(group_id != 0) {
                        // Finalize the previous group before starting a new one
                        auto prev_parent = it.world().entity(group_id);
                        prev_parent.get_mut<Rx::Component::VkTransformBuffer>().numberTransforms = instanceIndex;
                        auto& indirectBuffer = prev_parent.get_mut<Rx::Component::IndirectBuffer>();
                        indirectBuffer.setInstanceCount(instanceIndex);
                        prev_parent.get_mut<Rx::Component::VkIndirectBuffer>().copyFrom(indirectBuffer);
                        prev_parent.remove<ShouldBeUpdated>(); 
                    }
                });

            world.system
            <Rx::Component::AnimationStateMachine,
            Rx::Component::KeyFrameBuffer>
            ("AnimationStateMachineUpdate")
			.multi_threaded(true)
            .kind(preRender)
            .each([&](flecs::entity entity, Rx::Component::AnimationStateMachine& stateMachine, Rx::Component::KeyFrameBuffer& keyFrameBuffer) {
                    stateMachine.update(entity, keyFrameBuffer);
            });

             world.system("SkeletonModelUpdate")
            .with<Rx::Component::Transform>()
            .with<Rx::Component::MeshTransform>()
            .with<Rx::Component::KeyFrameBuffer>()
            .with(IsSkeletonModelInstanceOf, "$parent")
            .with<ShouldBeUpdated>().src("$parent") 
            .group_by(IsSkeletonModelInstanceOf)
            .kind(preRender)
            .run([&](flecs::iter& it) {
                    uint64_t group_id = 0;
                    uint32_t instanceIndex = 0;

                    flecs::entity parent;

                    while(it.next()) {
                         if(group_id != it.group_id()) {
                            if(group_id != 0) {
                                auto prev_parent = it.world().entity(group_id);
                                prev_parent.get_mut<Rx::Component::VkTransformBuffer>().numberTransforms = instanceIndex;
                                prev_parent.get_mut<Rx::Component::VkSkeletonArrayBuffer>().numberSkeletons = instanceIndex;
                                auto& indirectBuffer = prev_parent.get_mut<Rx::Component::IndirectBuffer>();
                                indirectBuffer.setInstanceCount(instanceIndex);
                                prev_parent.get_mut<Rx::Component::VkIndirectBuffer>().copyFrom(indirectBuffer);
                            }
                            group_id = it.group_id();
                            instanceIndex = 0; 
                         }

                        parent = it.world().entity(it.group_id());

                        RX_ASSERT(parent.is_alive(), "TextureModelUpdate", "System", "Parent entity is not alive");
                        RX_ASSERT(parent.has<Rx::Component::VkTransformBuffer>(), "TextureModelUpdate", "System", "Parent entity does not have VkTransformBuffer component");
                        RX_ASSERT(parent.has<Rx::Component::IndirectBuffer>(), "TextureModelUpdate", "System", "Parent entity does not have IndirectBuffer component");
                        RX_ASSERT(parent.has<Rx::Component::VkIndirectBuffer>(), "TextureModelUpdate", "System", "Parent entity does not have VkIndirectBuffer component");


                        auto& transformBuffer = parent.get_mut<Rx::Component::VkTransformBuffer>();
                        auto& vkKeyFrameArrayBuffer = parent.get_mut<Rx::Component::VkKeyFrameArrayBuffer>();

                        uint32_t transformCapacity =  transformBuffer.maxNumberTransforms;

                        Rx::Component::TransformInstance* transformInstances = (Rx::Component::TransformInstance*) transformBuffer.buffer.pMemory;
                        Component::VkKeyFrame* pKeyFrames = static_cast<Component::VkKeyFrame*>(vkKeyFrameArrayBuffer.buffer.pMemory);


                        const auto& transforms = it.field<const Rx::Component::Transform>(0);
                        const auto& meshTransforms = it.field<const Rx::Component::MeshTransform>(1);
                        auto keyFrameBuffers = it.field<Rx::Component::KeyFrameBuffer>(2);

                        for (auto i : it) {
                            if (instanceIndex >= transformCapacity) {
                                std::cerr << "Error: buffer overflow for parent "
                                        << parent.name() << "!\n";
								std::cout << "Instance Index: " << instanceIndex << ", Transform Capacity: " << transformCapacity << std::endl;
                                RX_LOGE("SkeletonModelUpdate", "System", ("Buffer overflow for parent: " + std::string(parent.name())).c_str());
                            }

                            auto transform = transforms[i].getTransformMatrix() * meshTransforms[i].getTransformMatrix();
                            transformInstances[instanceIndex].transform = transform;
                            transformInstances[instanceIndex].normalTransform = glm::transpose(glm::inverse(transform));

                            memcpy(pKeyFrames + instanceIndex * 256, keyFrameBuffers[i].keyFrames.data(), keyFrameBuffers[i].keyFrames.size() * sizeof(Component::VkKeyFrame));

                            instanceIndex++;
                        }
                    }

                    if(group_id != 0) {
                        // Finalize the previous group before starting a new one
                        auto prev_parent = it.world().entity(group_id);
                        prev_parent.get_mut<Rx::Component::VkTransformBuffer>().numberTransforms = instanceIndex;
                        prev_parent.get_mut<Rx::Component::VkSkeletonArrayBuffer>().numberSkeletons = instanceIndex;
                        auto& indirectBuffer = prev_parent.get_mut<Rx::Component::IndirectBuffer>();
                        indirectBuffer.setInstanceCount(instanceIndex);
                        prev_parent.get_mut<Rx::Component::VkIndirectBuffer>().copyFrom(indirectBuffer);
                    }
                });


                /*world.system("SkeletonModelTransformUpdate")
                .with<Rx::Component::Transform>()
                .with(IsSkeletonModelInstanceOf, "$parent")
                .with<ShouldBeUpdated>().src("$parent") 
                .group_by(IsSkeletonModelInstanceOf)
                .kind(preRender)
                .run([&](flecs::iter& it) {
                    uint64_t group_id = 0;
                    uint32_t instanceIndex = 0;

                    flecs::entity parent;

                    while(it.next()) {
                         if(group_id != it.group_id()) {
                            if(group_id != 0) {
                                // Finalize the previous group before starting a new one
                                auto prev_parent = it.world().entity(group_id);
                                prev_parent.get_mut<Rx::Component::VkTransformBuffer>().numberTransforms = instanceIndex;
                            }
                            group_id = it.group_id();
                            instanceIndex = 0; 
                         }

                        parent = it.world().entity(it.group_id());

                        RX_ASSERT(parent.is_alive(), "TextureModelUpdate", "System", "Parent entity is not alive");
                        RX_ASSERT(parent.has<Rx::Component::VkTransformBuffer>(), "TextureModelUpdate", "System", "Parent entity does not have VkTransformBuffer component");


                        auto& transformBuffer = parent.get_mut<Rx::Component::VkTransformBuffer>();
            
                        Rx::Component::TransformInstance* transformInstances = (Rx::Component::TransformInstance*) transformBuffer.buffer.pMemory;
                        size_t transformCapacity = transformBuffer.maxNumberTransforms;


                        const auto& transforms = it.field<const Rx::Component::Transform>(0);
                        
                        for (auto i : it) {
                            if (instanceIndex >= transformCapacity) {
                                std::cerr << "Error: buffer overflow for parent "
                                        << parent.name() << "!\n";
                                break;
                            }
                            
                            auto transform = transforms[i].getTransformMatrix();
                            transformInstances[instanceIndex].transform = transform;
                            transformInstances[instanceIndex].normalTransform = glm::transpose(glm::inverse(transform));

                            instanceIndex++;
                        }
                    }

                    if(group_id != 0) {
                        // Finalize the previous group before starting a new one
                        auto prev_parent = it.world().entity(group_id);
                        prev_parent.get_mut<Rx::Component::VkTransformBuffer>().numberTransforms = instanceIndex;
                    }
                });*/

            /*world.system("SkeletonModelBoneTransformUpdate")
            .with<Rx::Component::Transform>()
            .with<Rx::Component::NodeIndex>()
            .with(IsNodeOf, "$parent")
            .group_by(IsNodeOf)
            .with<Rx::Component::SkeletonBuffer>().src("$parent")
            .with<Rx::Component::Transform>().src("$parent")
            .kind(preRender)
            .run([&](flecs::iter& it) {
                    uint64_t group_id = 0;
                    uint32_t instanceIndex = 0;

                    flecs::entity parent;
                    const Component::SkeletonBuffer* pSkeletonBuffer;
                    glm::mat4 transform;

                    while(it.next()) {
                        if(group_id != it.group_id()) {
                            group_id = it.group_id();
                            parent = it.world().entity(it.group_id());
                            RX_ASSERT(parent.is_alive(), "SkeletonModelBoneTransformUpdate", "System", "Parent entity is not alive");
                            RX_ASSERT(parent.has<Rx::Component::SkeletonBuffer>(), "SkeletonModelBoneTransformUpdate", "System", "Parent entity does not have SkeletonBuffer component");
                            RX_ASSERT(parent.has<Rx::Component::Transform>(), "SkeletonModelBoneTransformUpdate", "System", "Parent entity does not have Transform component");
                            pSkeletonBuffer = &parent.get<Rx::Component::SkeletonBuffer>();
                            transform = parent.get<Rx::Component::Transform>().getTransformMatrix();
                        }

                        auto transforms = it.field<Rx::Component::Transform>(0);
                        auto nodeIndices = it.field<const Rx::Component::NodeIndex>(1);

                        for (auto i : it) {
                            transforms[i].fromGlmMat4(transform * pSkeletonBuffer->transforms[nodeIndices[i].index].local);
                        }
                    }
                  
            });

            flecs::entity IsNodeOfSkeletonChild;
            struct SkeletonIndex{
                uint32_t index;
            };
            struct SkeletonIndexBuffer{
                std::vector<uint32_t> indices;
            };
            world.system("SkeletonModelNodeTransformUpdate")
            .with<Rx::Component::Transform>()
            .with<Rx::Component::NodeIndex>()
            .with<SkeletonIndex>()
            .with(IsNodeOfSkeletonChild, "$parent")
            .group_by(IsNodeOfSkeletonChild)
            .with<Rx::Component::VkSkeletonArrayBuffer>().src("$parent")
            .with<Rx::Component::VkTransformBuffer>().src("$parent")
            .with<SkeletonIndexBuffer>().src("$parent")
            .kind(preRender)
            .run([&](flecs::iter& it) {
                    uint64_t group_id = 0;
                    uint32_t instanceIndex = 0;

                    flecs::entity parent;
                    const Component::NodeTransform* pSkeletonArrayBuffer;
                    const Component::TransformInstance* pTransformBuffer;
                    const uint32_t* pSkeletonIndexBuffer;
                    glm::mat4 transform;

                    while(it.next()) {
                        if(group_id != it.group_id()) {
                            group_id = it.group_id();
                            parent = it.world().entity(it.group_id());
                            RX_ASSERT(parent.is_alive(), "SkeletonModelBoneTransformUpdate", "System", "Parent entity is not alive");
                            RX_ASSERT(parent.has<Rx::Component::VkSkeletonArrayBuffer>(), "SkeletonModelBoneTransformUpdate", "System", "Parent entity does not have VkSkeletonArrayBuffer component");
                            pSkeletonArrayBuffer = (Component::NodeTransform*) parent.get<Rx::Component::VkSkeletonArrayBuffer>().buffer.pMemory;
                            pTransformBuffer = (Component::TransformInstance*) parent.get<Rx::Component::VkTransformBuffer>().buffer.pMemory;
                            pSkeletonIndexBuffer = parent.get<SkeletonIndexBuffer>().indices.data();
                        }

                        auto transforms = it.field<Rx::Component::Transform>(0);
                        auto nodeIndices = it.field<const Rx::Component::NodeIndex>(1);
                        auto skeletonIndices = it.field<const SkeletonIndex>(2);

                        for (auto i : it) {
                            uint32_t skeletonIndex = pSkeletonIndexBuffer[skeletonIndices[i].index];
                            transforms[i].fromGlmMat4(pTransformBuffer[skeletonIndex].transform * pSkeletonArrayBuffer[skeletonIndex * 256 + nodeIndices[i].index].local);
                        }
                    }
            });*/

            world.system()
            .kind(onRecordBegin)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                 while(it.next()) {
                     Rx::Core::vulkanCommandMutex.lock();
                     Rx::Core::beginCommand(Rx::Core::command);
                 }
            });

            world.system()
            .kind(onRecordBarrier1)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                 while(it.next()) {
                    VkMemoryBarrier memoryBarrier = {};
                    memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
                    // Wait for the compute shader to finish writing to the buffer
                    memoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
                    // Make the buffer readable by the vertex shader
                    memoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                    vkCmdPipelineBarrier(
                        Rx::Core::command.vkCommandBuffer,
                        VK_PIPELINE_STAGE_HOST_BIT,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        0,
                        1, &memoryBarrier,
                        0, nullptr,
                        0, nullptr
                    );
                }
            });




            world.system<const Rx::Component::Skeleton, const Rx::Component::VkSkeletonModelCompDescriptorSet, const Component::VkTransformBuffer>("SkeletonModelNodeCalculationSystem")
            .kind(onRecordComp)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {

                vkCmdBindPipeline(
                    Rx::Core::command.vkCommandBuffer,
                    VK_PIPELINE_BIND_POINT_COMPUTE,
                    Rx::Core::skeletonModelCompPipeline
                );

                while (it.next()) {
                    auto skeletons = it.field<const Rx::Component::Skeleton>(0);
                    auto descriptorSets = it.field<const Rx::Component::VkSkeletonModelCompDescriptorSet>(1);
                    auto transformBuffers = it.field<const Component::VkTransformBuffer>(2);

                    for(auto i : it){
                        const auto& skeleton = skeletons[i];
                        const auto& descriptorSet = descriptorSets[i];
                        const auto& transformBuffer = transformBuffers[i];

                        vkCmdBindDescriptorSets(
                            Rx::Core::command.vkCommandBuffer,
                            VK_PIPELINE_BIND_POINT_COMPUTE,
                            Rx::Core::skeletonModelCompPipelineLayout,
                            0, 1,
                            &descriptorSet.vkDescriptorSet,
                            0, nullptr
                        );

                        struct PushConstantData {
                            int numberNodes;
                        } pushConstants;

                        pushConstants.numberNodes = skeleton.nodes.size();

                        vkCmdPushConstants(
                            Rx::Core::command.vkCommandBuffer,
                            Rx::Core::skeletonModelCompPipelineLayout,
                            VK_SHADER_STAGE_COMPUTE_BIT,
                            0,
                            sizeof(PushConstantData),
                            &pushConstants
                        );

                        // Dispatch one workgroup per skeleton instance
                        vkCmdDispatch(
                            Rx::Core::command.vkCommandBuffer,
                            transformBuffer.numberTransforms, // Group Count X
                            1,                              // Group Count Y
                            1                               // Group Count Z
                        );
                    }
                }                
            });

            world.system()
            .kind(onRecordBarrier2)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                while (it.next()) {

                    VkMemoryBarrier memoryBarrier = {};
                    memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
                    // Wait for the compute shader to finish writing to the buffer
                    memoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
                    // Make the buffer readable by the vertex shader
                    memoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                    vkCmdPipelineBarrier(
                        Rx::Core::command.vkCommandBuffer,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,  // Source stage
                        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, // Destination stage
                        0,
                        1, &memoryBarrier,
                        0, nullptr,
                        0, nullptr
                    );
                }
            });

            world.system()
            .kind(onRecordShadow)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                 while(it.next()) {
                    Core::ShadowSpotLightBuffer& shadowBuffer = 
                        *(Core::ShadowSpotLightBuffer*)Core::shadowSpotLightBuffer.pMemory;
                    
                    uint32_t activeLights = static_cast<uint32_t>(shadowBuffer.numberShadowSpotLights.x);
                    if (activeLights == 0) {
                        // One-time or per-frame init: transition all layers to SHADER_READ_ONLY_OPTIMAL
                        VkImageMemoryBarrier init{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
                        init.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                        init.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                        init.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; // or current known layout
                        init.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        init.srcAccessMask = 0;
                        init.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                        init.image = Core::shadowMapArray.vkImage;
                        init.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                        init.subresourceRange.baseMipLevel = 0;
                        init.subresourceRange.levelCount = 1;
                        init.subresourceRange.baseArrayLayer = 0;
                        init.subresourceRange.layerCount = Core::shadowMapArray.count; // Max layers

                        VkCommandBuffer cmd0 = Core::command.vkCommandBuffer;
                        vkCmdPipelineBarrier(cmd0,
                            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                            0, 0, nullptr, 0, nullptr, 1, &init);
                        // Do not return from inside an iterator loop; continue to allow ecs_iter_fini
                        continue;
                    }

                    VkCommandBuffer cmd = Core::command.vkCommandBuffer;

                    // Transition shadow map array to attachment layout
                    VkImageMemoryBarrier toDepth{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
                    toDepth.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                    toDepth.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                    toDepth.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    toDepth.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                    toDepth.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    toDepth.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                    toDepth.image = Core::shadowMapArray.vkImage;
                    toDepth.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                    toDepth.subresourceRange.baseMipLevel = 0;
                    toDepth.subresourceRange.levelCount = 1;
                    toDepth.subresourceRange.baseArrayLayer = 0;
                    toDepth.subresourceRange.layerCount = activeLights;

                    vkCmdPipelineBarrier(cmd,
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                        0, 0, nullptr, 0, nullptr, 1, &toDepth);


                    // Begin dynamic rendering (multiview)
                    VkRenderingAttachmentInfo depthAttachment{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
                    depthAttachment.imageView = Core::shadowMapArray.vkImageView;
                    depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
                    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
                    depthAttachment.clearValue.depthStencil = {1.0f, 0};

                    uint32_t viewMask = (1u << activeLights) - 1u;
                    VkRenderingInfo renderingInfo{VK_STRUCTURE_TYPE_RENDERING_INFO};
                    renderingInfo.renderArea = {{0, 0}, 
                        {Core::shadowMapArray.width, Core::shadowMapArray.height}};
                    renderingInfo.layerCount = 1; // multiview uses viewMask
                    renderingInfo.viewMask = viewMask;
                    renderingInfo.pDepthAttachment = &depthAttachment;

                    vkCmdBeginRendering(cmd, &renderingInfo);

                }   
            });

            world.system<Component::VkColorMesh, Component::VkShadowColorModelDescriptorSet>("ShadowColorMeshSystem")
            .kind(onRecordShadow)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                Core::ShadowSpotLightBuffer& shadowBuffer = 
                    *(Core::ShadowSpotLightBuffer*)Core::shadowSpotLightBuffer.pMemory;
                
                uint32_t activeLights = static_cast<uint32_t>(shadowBuffer.numberShadowSpotLights.x);

                if(activeLights == 0) {
                    while(it.next()){}
                    return; // Nothing to do
                }
                
                VkCommandBuffer cmd = Core::command.vkCommandBuffer;
                vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, 
                                  Core::shadowColorMeshPipeline[activeLights - 1]);

                // Render all shadow casters (each with its own descriptor set)
                while(it.next()) {
                    auto meshes = it.field<Component::VkColorMesh>(0);
                    auto descriptorSets = it.field<Component::VkShadowColorModelDescriptorSet>(1);

                    for(auto i : it) {
                        // Bind unique descriptor set for this mesh (contains Model buffer + Lights buffer)
                        vkCmdBindDescriptorSets(cmd,
                            VK_PIPELINE_BIND_POINT_GRAPHICS, 
                            Core::shadowColorMeshPipelineLayout,
                            0, 1, &descriptorSets[i].getDescriptorSet(), 
                            0, nullptr);

                        // Bind vertex/index buffers
                        VkDeviceSize offset = 0;
                        vkCmdBindVertexBuffers(cmd, 0, 1, &meshes[i].vertexBuffer.vkBuffer, &offset);
                        vkCmdBindIndexBuffer(cmd, meshes[i].indexBuffer.vkBuffer, 0, VK_INDEX_TYPE_UINT32);
                        
                        // Draw
                        vkCmdDrawIndexed(cmd, meshes[i].getNumberIndices(), 1, 0, 0, 0);
                    }
                }

               
            });

            world.system<Rx::Component::VkColorMeshArray, Rx::Component::VkIndirectBuffer, Rx::Component::VkShadowColorModelArrayDescriptorSet>()
            .kind(onRecordShadow)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                 Core::ShadowSpotLightBuffer& shadowBuffer = 
                    *(Core::ShadowSpotLightBuffer*)Core::shadowSpotLightBuffer.pMemory;
                uint32_t activeLights = static_cast<uint32_t>(shadowBuffer.numberShadowSpotLights.x);
                
                if(activeLights == 0) {
                    while(it.next()){}
                    return; // Nothing to do
                }
                
                vkCmdBindPipeline
                (Rx::Core::command.vkCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                Rx::Core::shadowColorModelArrayPipeline[activeLights - 1]);
                                
                while(it.next()) {
                    auto colorMeshArray = it.field<Rx::Component::VkColorMeshArray>(0);
                    auto indirectBuffer = it.field<Rx::Component::VkIndirectBuffer>(1);
                    auto graphics = it.field<Rx::Component::VkShadowColorModelArrayDescriptorSet>(2);

                    for( auto i : it) {
                    
                        vkCmdBindDescriptorSets
                        (Rx::Core::command.vkCommandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        Rx::Core::shadowColorModelArrayPipelineLayout,
                        0, 1,
                        &graphics[i].vkDescriptorSet, 0, nullptr);

                    
                        VkDeviceSize offset[] = { 0 };    
                        vkCmdBindVertexBuffers
                        (Rx::Core::command.vkCommandBuffer,
                        0, 1, 
                        &colorMeshArray[i].vertexBuffer.vkBuffer, offset);

                        vkCmdBindIndexBuffer
                        (Rx::Core::command.vkCommandBuffer,
                        colorMeshArray[i].indexBuffer.vkBuffer,
                        0, VK_INDEX_TYPE_UINT32);

                        vkCmdDrawIndexedIndirect
                        (Rx::Core::command.vkCommandBuffer,
                        indirectBuffer[i].buffer.vkBuffer, 0,
                        indirectBuffer[i].numberCommands, 
                        sizeof(VkDrawIndexedIndirectCommand));
                    }
                }
            });

            world.system<Rx::Component::VkTextureMeshArray, Rx::Component::VkIndirectBuffer, Rx::Component::VkShadowTextureModelDescriptorSet>()
            .kind(onRecordShadow)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                 Core::ShadowSpotLightBuffer& shadowBuffer = 
                    *(Core::ShadowSpotLightBuffer*)Core::shadowSpotLightBuffer.pMemory;
                uint32_t activeLights = static_cast<uint32_t>(shadowBuffer.numberShadowSpotLights.x);
                
                if(activeLights == 0) {
                    while(it.next()){}
                    return; // Nothing to do
                }
                
                vkCmdBindPipeline
                (Rx::Core::command.vkCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                Rx::Core::shadowTextureModelPipeline[activeLights - 1]);
                                
                while(it.next()) {
                    auto colorMeshArray = it.field<Rx::Component::VkTextureMeshArray>(0);
                    auto indirectBuffer = it.field<Rx::Component::VkIndirectBuffer>(1);
                    auto graphics = it.field<Rx::Component::VkShadowTextureModelDescriptorSet>(2);

                    for( auto i : it) {
                    
                        vkCmdBindDescriptorSets
                        (Rx::Core::command.vkCommandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        Rx::Core::shadowTextureModelPipelineLayout,
                        0, 1,
                        &graphics[i].vkDescriptorSet, 0, nullptr);

                    
                        VkDeviceSize offset[] = { 0 };    
                        vkCmdBindVertexBuffers
                        (Rx::Core::command.vkCommandBuffer,
                        0, 1, 
                        &colorMeshArray[i].vertexBuffer.vkBuffer, offset);

                        vkCmdBindIndexBuffer
                        (Rx::Core::command.vkCommandBuffer,
                        colorMeshArray[i].indexBuffer.vkBuffer,
                        0, VK_INDEX_TYPE_UINT32);

                        vkCmdDrawIndexedIndirect
                        (Rx::Core::command.vkCommandBuffer,
                        indirectBuffer[i].buffer.vkBuffer, 0,
                        indirectBuffer[i].numberCommands, 
                        sizeof(VkDrawIndexedIndirectCommand));
                    }
                }
            });

            world.system<Rx::Component::VkSkeletonMeshArray,  Rx::Component::VkShadowSkeletonModelDescriptorSet, Rx::Component::VkIndirectBuffer>()
            .kind(onRecordShadow)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                 Core::ShadowSpotLightBuffer& shadowBuffer = 
                    *(Core::ShadowSpotLightBuffer*)Core::shadowSpotLightBuffer.pMemory;
                uint32_t activeLights = static_cast<uint32_t>(shadowBuffer.numberShadowSpotLights.x);
                
                if(activeLights == 0) {
                    while(it.next()){}
                    return; // Nothing to do
                }

                vkCmdBindPipeline
                (Rx::Core::command.vkCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                Rx::Core::shadowSkeletonModelPipeline[activeLights - 1]);

                while(it.next()) {
                    auto skeletonMeshes = it.field<Rx::Component::VkSkeletonMeshArray>(0);
                    auto descriptorSets = it.field<Rx::Component::VkShadowSkeletonModelDescriptorSet>(1);
                    auto indirectBuffers = it.field<Rx::Component::VkIndirectBuffer>(2);

                    for( auto i : it) {
                     
                        vkCmdBindDescriptorSets
                        (Rx::Core::command.vkCommandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        Rx::Core::shadowSkeletonModelPipelineLayout,
                        0, 1,
                        &descriptorSets[i].vkDescriptorSet, 0, nullptr);

                        VkDeviceSize offset[] = { 0 };    
                        vkCmdBindVertexBuffers
                        (Rx::Core::command.vkCommandBuffer,
                        0, 1, 
                        &skeletonMeshes[i].vertexBuffer.vkBuffer, offset);

                        vkCmdBindIndexBuffer
                        (Rx::Core::command.vkCommandBuffer,
                        skeletonMeshes[i].indexBuffer.vkBuffer,
                        0, VK_INDEX_TYPE_UINT32);

                        vkCmdDrawIndexedIndirect
                        (Rx::Core::command.vkCommandBuffer,
                        indirectBuffers[i].buffer.vkBuffer,
                        0, indirectBuffers[i].numberCommands,
                        sizeof(VkDrawIndexedIndirectCommand));
                    }
                }
            });


            world.system()
            .kind(onRecordShadow)
            .run([](flecs::iter& it) {

                Core::ShadowSpotLightBuffer& shadowBuffer = 
                *(Core::ShadowSpotLightBuffer*)Core::shadowSpotLightBuffer.pMemory;
                uint32_t activeLights = static_cast<uint32_t>(shadowBuffer.numberShadowSpotLights.x);
            
                 if(activeLights == 0) {
                    while(it.next()){}
                    return; // Nothing to do
                }
               
                while(it.next()) {
                
                    VkCommandBuffer cmd = Core::command.vkCommandBuffer;
                    vkCmdEndRendering(cmd);

                    // Transition to read-only for main pass
                    VkImageMemoryBarrier toRead{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
                    toRead.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                    toRead.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                    toRead.oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                    toRead.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    toRead.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                    toRead.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    toRead.image = Core::shadowMapArray.vkImage;
                    toRead.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                    toRead.subresourceRange.baseMipLevel = 0;
                    toRead.subresourceRange.levelCount = 1;
                    toRead.subresourceRange.baseArrayLayer = 0;
                    toRead.subresourceRange.layerCount = activeLights;

                    vkCmdPipelineBarrier(cmd,
                        VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                        0, 0, nullptr, 0, nullptr, 1, &toRead);
                }
            });

            world.system()
            .kind(onRecordRenderPassBegin)
            .run([](flecs::iter& it) {
                while (it.next()) {
                    Rx::Core::beginRenderPass(Rx::Core::command);
                }
            });

            world.system<Rx::Component::VkColorModelDescriptorSet, Rx::Component::VkColorModelBuffer, Rx::Component::VkColorMesh, Rx::Component::Material, Rx::Component::Transform>()
            .kind(onRecordRender)
           // .with<ShouldBeRendered>()
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {    
            
                vkCmdBindPipeline
                (Rx::Core::command.vkCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                Rx::Core::colorMeshPipeline);

                while (it.next()) {
                    auto descriptorSet = it.field<Rx::Component::VkColorModelDescriptorSet>(0);
                    auto modelBuffers = it.field<Rx::Component::VkColorModelBuffer>(1);
                    auto meshes = it.field<Rx::Component::VkColorMesh>(2);
                    auto materials = it.field<Rx::Component::Material>(3);
                    auto transforms = it.field<Rx::Component::Transform>(4);

                    for (auto i : it)
                    {

                        modelBuffers[i].setModelBuffer(transforms[i].getTransformMatrix(), materials[i]);

                        VkDeviceSize offset[] = { 0 };    
                        vkCmdBindVertexBuffers
                        (Rx::Core::command.vkCommandBuffer,
                        0, 1, 
                        &meshes[i].getVertexBuffer(), offset);
                    
                        
                        vkCmdBindIndexBuffer
                        (Rx::Core::command.vkCommandBuffer,
                        meshes[i].getIndexBuffer(),
                        0, VK_INDEX_TYPE_UINT32);
                    
                        vkCmdBindDescriptorSets
                        (Rx::Core::command.vkCommandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        Rx::Core::colorMeshPipelineLayout,
                        0, 1,
                        &descriptorSet[i].getDescriptorSet(),
                        0, nullptr);
                    
                        vkCmdDrawIndexed
                        (Rx::Core::command.vkCommandBuffer,
                        meshes[i].getNumberIndices(), 
                        1, 0, 0, 0);
                    }

                }
            
            });
            
            world.system<Rx::Component::VkColorMeshArray, Rx::Component::VkIndirectBuffer, Rx::Component::VkColorModelArrayDescriptorSet>()
            .kind(onRecordRender)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {

                vkCmdBindPipeline
                (Rx::Core::command.vkCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                Rx::Core::colorMeshArrayPipeline);

                while(it.next()) {
                    auto colorMeshArray = it.field<Rx::Component::VkColorMeshArray>(0);
                    auto indirectBuffer = it.field<Rx::Component::VkIndirectBuffer>(1);
                    auto graphics = it.field<Rx::Component::VkColorModelArrayDescriptorSet>(2);

                    for( auto i : it) {
                     
                        vkCmdBindDescriptorSets
                        (Rx::Core::command.vkCommandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        Rx::Core::colorMeshArrayPipelineLayout,
                        0, 1,
                        &graphics[i].vkDescriptorSet, 0, nullptr);

                    
                        VkDeviceSize offset[] = { 0 };    
                        vkCmdBindVertexBuffers
                        (Rx::Core::command.vkCommandBuffer,
                        0, 1, 
                        &colorMeshArray[i].vertexBuffer.vkBuffer, offset);

                        vkCmdBindIndexBuffer
                        (Rx::Core::command.vkCommandBuffer,
                        colorMeshArray[i].indexBuffer.vkBuffer,
                        0, VK_INDEX_TYPE_UINT32);

                        vkCmdDrawIndexedIndirect
                        (Rx::Core::command.vkCommandBuffer,
                        indirectBuffer[i].buffer.vkBuffer, 0,
                        indirectBuffer[i].numberCommands, 
                        sizeof(VkDrawIndexedIndirectCommand));
                    }
                }
            });

            world.system<Rx::Component::VkColorMesh, Rx::Component::VkInstancedColorModelBuffer,Rx::Component::VkInstancedColorModelDescriptorSet>()
            .kind(onRecordRender)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                vkCmdBindPipeline
                (Rx::Core::command.vkCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                Rx::Core::instancedColorMeshPipeline);

                while(it.next()) {
                    auto colorMesh = it.field<Rx::Component::VkColorMesh>(0);
                    auto instanceBuffer = it.field<Rx::Component::VkInstancedColorModelBuffer>(1);
                    auto descriptorSet = it.field<Rx::Component::VkInstancedColorModelDescriptorSet>(2);

                    for( auto i : it) {
                     
                        vkCmdBindDescriptorSets
                        (Rx::Core::command.vkCommandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        Rx::Core::instancedColorMeshPipelineLayout,
                        0, 1,
                        &descriptorSet[i].vkDescriptorSet, 0, nullptr);

                        VkDeviceSize offset[] = { 0 };    
                        vkCmdBindVertexBuffers
                        (Rx::Core::command.vkCommandBuffer,
                        0, 1, 
                        &colorMesh[i].vertexBuffer.vkBuffer, offset);

                        vkCmdBindIndexBuffer
                        (Rx::Core::command.vkCommandBuffer,
                        colorMesh[i].indexBuffer.vkBuffer,
                        0, VK_INDEX_TYPE_UINT32);

                        vkCmdDrawIndexed
                        (Rx::Core::command.vkCommandBuffer,
                        colorMesh[i].getNumberIndices(),
                        instanceBuffer[i].numberInstances,
                        0, 0, 0);
                    }
                }
            });

            world.system<Rx::Component::VkTextureMeshArray, Rx::Component::VkTextureModelDescriptorSet, Rx::Component::VkIndirectBuffer>()
            .kind(onRecordRender)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {

                
                
                vkCmdBindPipeline
                (Rx::Core::command.vkCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                Rx::Core::textureModelPipeline);

                while(it.next()) {
                    auto textureMeshes = it.field<Rx::Component::VkTextureMeshArray>(0);
                    auto descriptorSets = it.field<Rx::Component::VkTextureModelDescriptorSet>(1);
                    auto indirectBuffers = it.field<Rx::Component::VkIndirectBuffer>(2);

                    for( auto i : it) {
                     
                        vkCmdBindDescriptorSets
                        (Rx::Core::command.vkCommandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        Rx::Core::textureModelPipelineLayout,
                        0, 1,
                        &descriptorSets[i].vkDescriptorSet, 0, nullptr);

                        VkDeviceSize offset[] = { 0 };    
                        vkCmdBindVertexBuffers
                        (Rx::Core::command.vkCommandBuffer,
                        0, 1, 
                        &textureMeshes[i].vertexBuffer.vkBuffer, offset);

                        vkCmdBindIndexBuffer
                        (Rx::Core::command.vkCommandBuffer,
                        textureMeshes[i].indexBuffer.vkBuffer,
                        0, VK_INDEX_TYPE_UINT32);

                        vkCmdDrawIndexedIndirect
                        (Rx::Core::command.vkCommandBuffer,
                        indirectBuffers[i].buffer.vkBuffer,
                        0, indirectBuffers[i].numberCommands,
                        sizeof(VkDrawIndexedIndirectCommand));
                    }
                }

              });

            world.system<Rx::Component::VkSkeletonMeshArray,  Rx::Component::VkSkeletonModelDescriptorSet, Rx::Component::VkIndirectBuffer>()
            .kind(onRecordRender)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                vkCmdBindPipeline
                (Rx::Core::command.vkCommandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                Rx::Core::skeletonModelPipeline);

                while(it.next()) {
                    auto skeletonMeshes = it.field<Rx::Component::VkSkeletonMeshArray>(0);
                    auto descriptorSets = it.field<Rx::Component::VkSkeletonModelDescriptorSet>(1);
                    auto indirectBuffers = it.field<Rx::Component::VkIndirectBuffer>(2);

                    for( auto i : it) {
                     
                        vkCmdBindDescriptorSets
                        (Rx::Core::command.vkCommandBuffer,
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        Rx::Core::skeletonModelPipelineLayout,
                        0, 1,
                        &descriptorSets[i].vkDescriptorSet, 0, nullptr);

                        VkDeviceSize offset[] = { 0 };    
                        vkCmdBindVertexBuffers
                        (Rx::Core::command.vkCommandBuffer,
                        0, 1, 
                        &skeletonMeshes[i].vertexBuffer.vkBuffer, offset);

                        vkCmdBindIndexBuffer
                        (Rx::Core::command.vkCommandBuffer,
                        skeletonMeshes[i].indexBuffer.vkBuffer,
                        0, VK_INDEX_TYPE_UINT32);

                        vkCmdDrawIndexedIndirect
                        (Rx::Core::command.vkCommandBuffer,
                        indirectBuffers[i].buffer.vkBuffer,
                        0, indirectBuffers[i].numberCommands,
                        sizeof(VkDrawIndexedIndirectCommand));
                    }
                }
            });

            world.system()
            .kind(onRecordEnd)
            .with<RenderRunning>().src(game)
            .run([](flecs::iter& it) {
                while(it.next()) {
                    Rx::Core::endRenderPass(Rx::Core::command);
                    Rx::Core::endCommand(Rx::Core::command);
                    Rx::Core::submitGraphicsCommand(Rx::Core::command);
                    Rx::Core::presentGraphics(Rx::Core::command);
                    Rx::Core::vulkanCommandMutex.unlock();
                }
            });
            

        }

}