#include "VkSkeletonModelDescriptorSet.hpp"
#include "Mutex.hpp"
#include "Device.hpp"
#include "DescriptorPool.hpp"
#include "DescriptorSetLayouts.hpp"
#include "Eye.hpp"
#include "Light.hpp"
#include "TransformInstance.hpp"
#include "Material.hpp"
#include "Transform.hpp"
#include "NodeTransform.hpp"

namespace Rx{
    namespace Component{

        void vkSkeletonModelDescriptorSet_component_on_add(flecs::entity e, VkTextureArray& textureArray, VkTextureMaterialBuffer& textureMaterialBuffer, VkTransformBuffer& transformBuffer, VkSkeletonArrayBuffer& skeletonArrayBuffer, VkSkeletonModelDescriptorSet& descriptorSet){
            descriptorSet.createDescriptorSet(textureArray, textureMaterialBuffer, transformBuffer, skeletonArrayBuffer);
        }

        void vkSkeletonModelDescriptorSet_component_on_remove(flecs::entity e, VkSkeletonModelDescriptorSet& descriptorSet){
            descriptorSet.destroyDescriptorSet();
        }


        void VkSkeletonModelDescriptorSet::createDescriptorSet(VkTextureArray& textureArray, VkTextureMaterialBuffer& textureMaterialBuffer, VkTransformBuffer& transformBuffer, VkSkeletonArrayBuffer& skeletonArrayBuffer){
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = Core::vkDescriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &Core::skeletonModelDescriptorSetLayout;

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkAllocateDescriptorSets
            (Core::vkDevice,
            &allocInfo,
            &vkDescriptorSet),
            "VkSkeletonModelDescriptorSet::createDescriptorSet",
            "vkAllocateDescriptorSets"))

            std::vector<VkDescriptorBufferInfo> bufferInfo(7);
            bufferInfo[0].buffer = Core::eye.vkBuffer;
            bufferInfo[0].offset = 0;
            bufferInfo[0].range = sizeof(Core::Eye);
            bufferInfo[1].buffer = Core::pointLightBuffer.vkBuffer;
            bufferInfo[1].offset = 0;
            bufferInfo[1].range = sizeof(Core::PointLightBuffer);
            bufferInfo[2].buffer = Core::directionalLightBuffer.vkBuffer;
            bufferInfo[2].offset = 0;
            bufferInfo[2].range = sizeof(Core::DirectionalLightBuffer);
            bufferInfo[3].buffer = Core::shadowSpotLightBuffer.vkBuffer;
            bufferInfo[3].offset = 0;
            bufferInfo[3].range = sizeof(Core::ShadowSpotLightBuffer);
            bufferInfo[4].buffer = transformBuffer.buffer.vkBuffer;
            bufferInfo[4].offset = 0;
            bufferInfo[4].range = sizeof(TransformInstance) * transformBuffer.maxNumberTransforms;
            bufferInfo[5].buffer = textureMaterialBuffer.buffer.vkBuffer;
            bufferInfo[5].offset = 0;
            bufferInfo[5].range = sizeof(TextureMaterial) * textureMaterialBuffer.maxNumberMaterials;
            bufferInfo[6].buffer = skeletonArrayBuffer.buffer.vkBuffer;
            bufferInfo[6].offset = 0;
            bufferInfo[6].range = sizeof(NodeTransform) * skeletonArrayBuffer.maxNumberSkeletons * 256; // Assuming max 256 mat4 bones per skeleton

            VkDescriptorImageInfo shadowMapInfo{};
            shadowMapInfo.sampler = Core::shadowMapArray.vkSampler;
            shadowMapInfo.imageView = Core::shadowMapArray.vkImageView;
            shadowMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            
            std::vector<VkDescriptorImageInfo> imageInfos(textureArray.textures.size());
            for (size_t i = 0; i < textureArray.textures.size(); i++) {
                imageInfos[i].sampler = textureArray.textures[i].vkSampler;
                imageInfos[i].imageView = textureArray.textures[i].vkImageView;
                imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            }
            
            std::vector<VkWriteDescriptorSet> writeSet(9);
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
            writeSet[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeSet[1].descriptorCount = 1;
            writeSet[1].pBufferInfo = &bufferInfo[1];
            writeSet[1].pImageInfo = nullptr;
            writeSet[1].pTexelBufferView = nullptr;
            writeSet[1].pNext = nullptr;
            writeSet[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[2].dstSet = vkDescriptorSet;
            writeSet[2].dstBinding = 2;
            writeSet[2].dstArrayElement = 0;
            writeSet[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeSet[2].descriptorCount = 1;
            writeSet[2].pBufferInfo = &bufferInfo[2];
            writeSet[2].pImageInfo = nullptr;
            writeSet[2].pTexelBufferView = nullptr;
            writeSet[2].pNext = nullptr;
            // Binding 3: ShadowSpotLightBuffer (UBO)
            writeSet[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[3].dstSet = vkDescriptorSet;
            writeSet[3].dstBinding = 3;
            writeSet[3].dstArrayElement = 0;
            writeSet[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeSet[3].descriptorCount = 1;
            writeSet[3].pBufferInfo = &bufferInfo[3];
            writeSet[3].pImageInfo = nullptr;
            writeSet[3].pTexelBufferView = nullptr;
            writeSet[3].pNext = nullptr;
               // Binding 4: ShadowMapArray (Combined Image Sampler)
            writeSet[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[4].dstSet = vkDescriptorSet;
            writeSet[4].dstBinding = 4;
            writeSet[4].dstArrayElement = 0;
            writeSet[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeSet[4].descriptorCount = 1;
            writeSet[4].pBufferInfo = nullptr;
            writeSet[4].pImageInfo = &shadowMapInfo;
            writeSet[4].pTexelBufferView = nullptr;
            writeSet[4].pNext = nullptr;
            writeSet[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[5].dstSet = vkDescriptorSet;
            writeSet[5].dstBinding = 5;
            writeSet[5].dstArrayElement = 0;
            writeSet[5].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            writeSet[5].descriptorCount = 1;
            writeSet[5].pBufferInfo = &bufferInfo[4];
            writeSet[5].pImageInfo = nullptr;
            writeSet[5].pTexelBufferView = nullptr;
            writeSet[5].pNext = nullptr;
            writeSet[6].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[6].dstSet = vkDescriptorSet;
            writeSet[6].dstBinding = 6;
            writeSet[6].dstArrayElement = 0;
            writeSet[6].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            writeSet[6].descriptorCount = 1;
            writeSet[6].pBufferInfo = &bufferInfo[5];
            writeSet[6].pImageInfo = nullptr;
            writeSet[6].pTexelBufferView = nullptr;
            writeSet[6].pNext = nullptr;
            writeSet[7].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[7].dstSet = vkDescriptorSet;
            writeSet[7].dstBinding = 7;
            writeSet[7].dstArrayElement = 0;
            writeSet[7].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            writeSet[7].descriptorCount = 1;
            writeSet[7].pBufferInfo = &bufferInfo[6];
            writeSet[7].pImageInfo = nullptr;
            writeSet[7].pTexelBufferView = nullptr;
            writeSet[7].pNext = nullptr;
            writeSet[8].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[8].dstSet = vkDescriptorSet;
            writeSet[8].dstBinding = 8;
            writeSet[8].dstArrayElement = 0;
            writeSet[8].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeSet[8].descriptorCount = static_cast<uint32_t>(imageInfos.size());
            writeSet[8].pBufferInfo = nullptr;
            writeSet[8].pImageInfo = imageInfos.data();
            writeSet[8].pTexelBufferView = nullptr;
            writeSet[8].pNext = nullptr;
            
            RX_VK_MUTEX(
            vkUpdateDescriptorSets
            (Core::vkDevice, 
            static_cast<uint32_t>(writeSet.size()),
            writeSet.data(),
            0, nullptr));
        
        }

        void VkSkeletonModelDescriptorSet::destroyDescriptorSet() {
            vkFreeDescriptorSets(Core::vkDevice, Core::vkDescriptorPool, 1, &vkDescriptorSet);
        }
    }
}