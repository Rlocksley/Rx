#include "VkShadowSkeletonModelDescriptorSet.hpp"
#include "Device.hpp"
#include "DescriptorPool.hpp"
#include "DescriptorSetLayouts.hpp"
#include "Mutex.hpp"
#include "Light.hpp"
#include "TransformInstance.hpp"
#include "NodeTransform.hpp"

namespace Rx
{
    namespace Component
    {
        void VkShadowSkeletonModelDescriptorSet_component_on_add(flecs::entity e, Rx::Component::VkTransformBuffer& transformBuffer, Rx::Component::VkSkeletonArrayBuffer& skeletonArrayBuffer, Rx::Component::VkShadowSkeletonModelDescriptorSet& graphics){
            // Create descriptor set for the graphics component
            graphics.createDescriptorSet(transformBuffer.buffer, skeletonArrayBuffer.buffer);
        }

        void VkShadowSkeletonModelDescriptorSet_component_on_remove(flecs::entity e, Rx::Component::VkShadowSkeletonModelDescriptorSet& graphics){
            // Destroy the descriptor set when the component is removed
            graphics.destroyDescriptorSet();
        }


        void VkShadowSkeletonModelDescriptorSet::createDescriptorSet(Core::BufferInterface& instanceBuffer, Core::BufferInterface& skeletonArrayBuffer){
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = Core::vkDescriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &Core::shadowSkeletonModelDescriptorSetLayout;
            
            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkAllocateDescriptorSets
            (Core::vkDevice,
            &allocInfo,
            &vkDescriptorSet),
            "VkShadowTextureModelDescriptorSet::createDescriptorSet",
            "vkAllocateDescriptorSets"))

            std::vector<VkDescriptorBufferInfo> bufferInfo(3);
            bufferInfo[0].buffer = Core::shadowSpotLightBuffer.vkBuffer;
            bufferInfo[0].offset = 0;
            bufferInfo[0].range = sizeof(Core::ShadowSpotLightBuffer);
            bufferInfo[1].buffer = instanceBuffer.vkBuffer;
            bufferInfo[1].offset = 0;
            bufferInfo[1].range = sizeof(TransformInstance) * instanceBuffer.numberElements;
            bufferInfo[2].buffer = skeletonArrayBuffer.vkBuffer;
            bufferInfo[2].offset = 0;
            bufferInfo[2].range = sizeof(NodeTransform) * skeletonArrayBuffer.numberElements; // Assuming max 256 mat4 bones per skeleton

            std::vector<VkWriteDescriptorSet> writeSet(3);
            writeSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[0].dstSet = vkDescriptorSet;
            writeSet[0].dstBinding = 0;
            writeSet[0].dstArrayElement = 0;
            writeSet[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeSet[0].descriptorCount = 1;
            writeSet[0].pBufferInfo = &bufferInfo[0];
            writeSet[0].pImageInfo = nullptr;
            writeSet[0].pTexelBufferView = nullptr;
            writeSet[0].pNext = nullptr;
            writeSet[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[1].dstSet = vkDescriptorSet;
            writeSet[1].dstBinding = 1;
            writeSet[1].dstArrayElement = 0;
            writeSet[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            writeSet[1].descriptorCount = 1;
            writeSet[1].pBufferInfo = &bufferInfo[1];
            writeSet[1].pImageInfo = nullptr;
            writeSet[1].pTexelBufferView = nullptr;
            writeSet[1].pNext = nullptr;
            writeSet[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[2].dstSet = vkDescriptorSet;
            writeSet[2].dstBinding = 2;
            writeSet[2].dstArrayElement = 0;
            writeSet[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            writeSet[2].descriptorCount = 1;
            writeSet[2].pBufferInfo = &bufferInfo[2];
            writeSet[2].pImageInfo = nullptr;
            writeSet[2].pTexelBufferView = nullptr;
            writeSet[2].pNext = nullptr;

            vkUpdateDescriptorSets(
                Core::vkDevice,
                static_cast<uint32_t>(writeSet.size()),
                writeSet.data(),
                0,
                nullptr
            );
        }

        void VkShadowSkeletonModelDescriptorSet::destroyDescriptorSet()
        {
            RX_VK_MUTEX(
            vkFreeDescriptorSets
            (Core::vkDevice,
            Core::vkDescriptorPool,
            1, &vkDescriptorSet);)
        }
    }
}