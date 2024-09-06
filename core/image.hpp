#pragma once

#include "global.hpp"
#include "vma/vk_mem_alloc.h"

namespace Neon
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
    }
}