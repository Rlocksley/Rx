#pragma once

#include "Global.hpp"
#include "vma/vk_mem_alloc.h"

namespace Rx
{
    namespace Core
    {
        
        struct Image
        {
            uint32_t width;
            uint32_t height;
            VkImage vkImage;
            VkImageView vkImageView;
            VmaAllocation vmaAllocation;
        };
        
/*
       struct Image
        {
            uint32_t width;
            uint32_t height;
            VkImage vkImage;
            VkImageView vkImageView;
            VkDeviceMemory vkDeviceMemory;
        };
*/        
        Image createImage
        (uint32_t width, uint32_t height,
        VkFormat format,
        VkImageUsageFlags usage,
        VkSampleCountFlagBits samples,
        VkImageAspectFlags aspect);

        void destroyImage(Image image);

        struct TextureCPU{
            uint32_t width{ 0 };
            uint32_t height{ 0 };
            void* pixels{ nullptr };
            VkFormat vkFormat{};
        };

        struct Texture{
            uint32_t width;
            uint32_t height;
            VkImage vkImage;
            VmaAllocation vmaAllocation;
            VkImageView vkImageView;
            VkSampler vkSampler;
        };

        Texture createTexture(uint32_t width, uint32_t height, void* pixels);
        void destroyTexture(Texture& texture);


        struct ShadowMapArray{
            uint32_t width;
            uint32_t height;
            uint32_t count;
            VkImage vkImage;
            VmaAllocation vmaAllocation;
            VmaAllocationInfo vmaAllocationInfo;
            VkImageView vkImageView;
            VkSampler vkSampler;
            VkFormat format;
        };

        inline ShadowMapArray shadowMapArray;

        void createShadowMapArray(uint32_t width, uint32_t height, uint32_t count, VkFormat format);
        void destroyShadowMapArray();
    }
}