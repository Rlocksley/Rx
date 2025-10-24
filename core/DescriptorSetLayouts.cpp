#include "DescriptorSetLayouts.hpp"
#include "Device.hpp"
#include "Mutex.hpp"

namespace Rx
{
    namespace Core
    {
        void createDescriptorSetLayouts()
        {
            createColorMeshDescriptorSetLayout();
            createColorMeshArrayDescriptorSetLayout();
            createInstancedColorMeshDescriptorSetLayout();
            createTextureModelDescriptorSetLayout();
            createSkeletonModelDescriptorSetLayout();
            createSkeletonModelCompDescriptorSetLayout();
            createShadowColorMeshDescriptorSetLayout();
            createShadowColorModelArrayDescriptorSetLayout();
            createShadowTextureModelDescriptorSetLayout();
            createShadowSkeletonModelDescriptorSetLayout();
        }

        void destroyDescriptorSetLayouts()
        {
            destroyShadowSkeletonModelDescriptorSetLayout();
            destroyShadowTextureModelDescriptorSetLayout();
            destroyShadowColorModelArrayDescriptorSetLayout();
            destroyShadowColorMeshDescriptorSetLayout();
            destroySkeletonModelCompDescriptorSetLayout();
            destroySkeletonModelDescriptorSetLayout();
            destroyTextureModelDescriptorSetLayout();
            destroyInstancedColorMeshDescriptorSetLayout();
            destroyColorMeshArrayDescriptorSetLayout();
            destroyColorMeshDescriptorSetLayout();
        }


        void createColorMeshDescriptorSetLayout()
        {
            std::vector<VkDescriptorSetLayoutBinding> bindings(6);
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[1].pImmutableSamplers = nullptr;
            bindings[2].binding = 2;
            bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[2].descriptorCount = 1;
            bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[2].pImmutableSamplers = nullptr;
            bindings[3].binding = 3;
            bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[3].descriptorCount = 1;
            bindings[3].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[3].pImmutableSamplers = nullptr;
            bindings[4].binding = 4;
            bindings[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            bindings[4].descriptorCount = 1;
            bindings[4].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[4].pImmutableSamplers = nullptr;
            bindings[5].binding = 5;
            bindings[5].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[5].descriptorCount = 1;
            bindings[5].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[5].pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (Core::vkDevice,
            &layoutInfo,
            nullptr,
            &colorMeshDescriptorSetLayout),
            "VkColorModelDescriptorSet::createDescriptorSet",
            "vkCreateDescriptorSetLayout"))
            
        }


        void destroyColorMeshDescriptorSetLayout()
        {
            RX_VK_MUTEX(
                vkDestroyDescriptorSetLayout(vkDevice, colorMeshDescriptorSetLayout, nullptr);
            )
        }

        void createColorMeshArrayDescriptorSetLayout()
        {
            std::vector<VkDescriptorSetLayoutBinding> bindings(6);
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[1].pImmutableSamplers = nullptr;
            bindings[2].binding = 2;
            bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[2].descriptorCount = 1;
            bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[2].pImmutableSamplers = nullptr;
            bindings[3].binding = 3;
            bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[3].descriptorCount = 1;
            bindings[3].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[3].pImmutableSamplers = nullptr;
            bindings[4].binding = 4;
            bindings[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            bindings[4].descriptorCount = 1;
            bindings[4].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[4].pImmutableSamplers = nullptr;
            bindings[5].binding = 5;
            bindings[5].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[5].descriptorCount = 1;
            bindings[5].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[5].pImmutableSamplers = nullptr;
            
            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (vkDevice, 
            &layoutInfo, 
            nullptr, 
            &colorMeshArrayDescriptorSetLayout),
            "createColorMeshArrayDescriptorSetLayout", 
            "vkCreateDescriptorSetLayout"
            ))
        }

        void destroyColorMeshArrayDescriptorSetLayout()
        {
            RX_VK_MUTEX(
                vkDestroyDescriptorSetLayout(vkDevice, colorMeshArrayDescriptorSetLayout, nullptr);
            )
        }



        void createInstancedColorMeshDescriptorSetLayout()
        {
            std::vector<VkDescriptorSetLayoutBinding> bindings(4);
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[1].pImmutableSamplers = nullptr;
            bindings[2].binding = 2;
            bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[2].descriptorCount = 1;
            bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[2].pImmutableSamplers = nullptr;
            bindings[3].binding = 3;
            bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[3].descriptorCount = 1;
            bindings[3].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[3].pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (vkDevice, 
            &layoutInfo, 
            nullptr, 
            &instancedColorMeshDescriptorSetLayout),
            "createInstancedColorMeshDescriptorSetLayout", 
            "vkCreateDescriptorSetLayout"))
        }

        void destroyInstancedColorMeshDescriptorSetLayout()
        {
            RX_VK_MUTEX(
            vkDestroyDescriptorSetLayout
            (vkDevice, 
            instancedColorMeshDescriptorSetLayout,
            nullptr);)
        }


        void createTextureModelDescriptorSetLayout(){
            std::vector<VkDescriptorSetLayoutBinding> bindings(8);
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[1].pImmutableSamplers = nullptr;
            bindings[2].binding = 2;
            bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[2].descriptorCount = 1;
            bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[2].pImmutableSamplers = nullptr;
            bindings[3].binding = 3;
            bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[3].descriptorCount = 1;
            bindings[3].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[3].pImmutableSamplers = nullptr;
            bindings[4].binding = 4;
            bindings[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            bindings[4].descriptorCount = 1;
            bindings[4].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[4].pImmutableSamplers = nullptr;
            bindings[5].binding = 5;
            bindings[5].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[5].descriptorCount = 1;
            bindings[5].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[5].pImmutableSamplers = nullptr;
            bindings[6].binding = 6;
            bindings[6].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[6].descriptorCount = 1;
            bindings[6].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[6].pImmutableSamplers = nullptr;
            bindings[7].binding = 7;
            bindings[7].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            bindings[7].descriptorCount = 128;
            bindings[7].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[7].pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (vkDevice, 
            &layoutInfo, 
            nullptr, 
            &textureModelDescriptorSetLayout),
            "createTextureModelDescriptorSetLayout", 
            "vkCreateDescriptorSetLayout"))
        }

        void destroyTextureModelDescriptorSetLayout(){
            RX_VK_MUTEX(
            vkDestroyDescriptorSetLayout
            (vkDevice, 
            textureModelDescriptorSetLayout,
            nullptr);)
        }

        void createSkeletonModelDescriptorSetLayout(){
            std::vector<VkDescriptorSetLayoutBinding> bindings(9);
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[1].pImmutableSamplers = nullptr;
            bindings[2].binding = 2;
            bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[2].descriptorCount = 1;
            bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[2].pImmutableSamplers = nullptr;
            bindings[3].binding = 3;
            bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[3].descriptorCount = 1;
            bindings[3].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[3].pImmutableSamplers = nullptr;
            bindings[4].binding = 4;
            bindings[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            bindings[4].descriptorCount = 1;
            bindings[4].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[4].pImmutableSamplers = nullptr;
            bindings[5].binding = 5;
            bindings[5].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[5].descriptorCount = 1;
            bindings[5].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[5].pImmutableSamplers = nullptr;
            bindings[6].binding = 6;
            bindings[6].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[6].descriptorCount = 1;
            bindings[6].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[6].pImmutableSamplers = nullptr;
            bindings[7].binding = 7;
            bindings[7].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[7].descriptorCount = 1;
            bindings[7].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[7].pImmutableSamplers = nullptr;
            bindings[8].binding = 8;
            bindings[8].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            bindings[8].descriptorCount = 128;
            bindings[8].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings[8].pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (vkDevice, 
            &layoutInfo, 
            nullptr, 
            &skeletonModelDescriptorSetLayout),
            "createSkeletonModelDescriptorSetLayout", 
            "vkCreateDescriptorSetLayout"))
        
        }

        void destroySkeletonModelDescriptorSetLayout(){
            RX_VK_MUTEX(
            vkDestroyDescriptorSetLayout
            (vkDevice, 
            skeletonModelDescriptorSetLayout,
            nullptr);)
        }

        void createSkeletonModelCompDescriptorSetLayout(){
            std::vector<VkDescriptorSetLayoutBinding> bindings(3);
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
            bindings[1].pImmutableSamplers = nullptr;
            bindings[2].binding = 2;
            bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[2].descriptorCount = 1;
            bindings[2].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
            bindings[2].pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (vkDevice, 
            &layoutInfo, 
            nullptr, 
            &skeletonModelCompDescriptorSetLayout),
            "createSkeletonModelCompDescriptorSetLayout", 
            "vkCreateDescriptorSetLayout"))
        
        }

        void destroySkeletonModelCompDescriptorSetLayout(){
            RX_VK_MUTEX(
            vkDestroyDescriptorSetLayout
            (vkDevice, 
            skeletonModelCompDescriptorSetLayout,
            nullptr);)
        }


        void createShadowColorMeshDescriptorSetLayout(){
            std::vector<VkDescriptorSetLayoutBinding> bindings(2);
            // Binding 0: ShadowSpotLightBuffer
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            // Binding 1: ModelBuffer
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[1].pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (Core::vkDevice,
            &layoutInfo,
            nullptr,
            &shadowColorMeshDescriptorSetLayout),
            "VkColorModelDescriptorSet::createDescriptorSet",
            "vkCreateDescriptorSetLayout"))
        }

        void destroyShadowColorMeshDescriptorSetLayout(){
            RX_VK_MUTEX(
            vkDestroyDescriptorSetLayout
            (vkDevice, 
            shadowColorMeshDescriptorSetLayout,
            nullptr);)
        
        }

        void createShadowColorModelArrayDescriptorSetLayout(){
            std::vector<VkDescriptorSetLayoutBinding> bindings(2);
            // Binding 0: ShadowSpotLightBuffer
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            // Binding 1: ModelBuffer
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[1].pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (Core::vkDevice,
            &layoutInfo,
            nullptr,
            &shadowColorModelArrayDescriptorSetLayout),
            "VkColorModelDescriptorSet::createDescriptorSet",
            "vkCreateDescriptorSetLayout"))
        
        }

        void destroyShadowColorModelArrayDescriptorSetLayout(){
            RX_VK_MUTEX(
            vkDestroyDescriptorSetLayout
            (vkDevice, 
            shadowColorModelArrayDescriptorSetLayout,
            nullptr);)

        }

        void createShadowTextureModelDescriptorSetLayout(){
            std::vector<VkDescriptorSetLayoutBinding> bindings(2);
            // Binding 0: ShadowSpotLightBuffer
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            // Binding 1: ModelBuffer
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[1].pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (Core::vkDevice,
            &layoutInfo,
            nullptr,
            &shadowTextureModelDescriptorSetLayout),
            "VkColorModelDescriptorSet::createDescriptorSet",
            "vkCreateDescriptorSetLayout"))
        
        }

        void destroyShadowTextureModelDescriptorSetLayout(){
            RX_VK_MUTEX(
            vkDestroyDescriptorSetLayout
            (vkDevice, 
            shadowTextureModelDescriptorSetLayout,
            nullptr);)
        }

        
        void createShadowSkeletonModelDescriptorSetLayout(){
            std::vector<VkDescriptorSetLayoutBinding> bindings(3);
            // Binding 0: ShadowSpotLightBuffer
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[1].pImmutableSamplers = nullptr;
            bindings[2].binding = 2;
            bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            bindings[2].descriptorCount = 1;
            bindings[2].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[2].pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (Core::vkDevice,
            &layoutInfo,
            nullptr,
            &shadowSkeletonModelDescriptorSetLayout),
            "createShadowSkeletonModelDescriptorSetLayout",
            "vkCreateDescriptorSetLayout"))
        
        }

        void destroyShadowSkeletonModelDescriptorSetLayout(){
            RX_VK_MUTEX(
            vkDestroyDescriptorSetLayout
            (vkDevice, 
            shadowSkeletonModelDescriptorSetLayout,
            nullptr);)
        }
    }
} // namespace Rx::Core