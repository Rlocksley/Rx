#include "image.hpp"
#include "physicalDevice.hpp"
#include "device.hpp"
#include "allocator.hpp"

namespace Neon
{
    namespace Core
    {

        Image createImage
        (uint32_t width, uint32_t height,
        VkFormat format,
        VkImageUsageFlags usage,
        VkSampleCountFlagBits samples,
        VkImageAspectFlags aspect)
        {
            Image image;
            image.width = width;
            image.height = height;

            VkImageCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            createInfo.imageType = VK_IMAGE_TYPE_2D;
            createInfo.extent.width = width;
            createInfo.extent.height = height;
            createInfo.extent.depth = 1;
            createInfo.mipLevels = 1;
            createInfo.arrayLayers = 1;
            createInfo.format = format;
            createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            createInfo.usage = usage;
            createInfo.samples = samples;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
            allocInfo.priority = 1.f;

            NEON_CHECK_VULKAN
            (vmaCreateImage
            (vmaAllocator,
            &createInfo,
            &allocInfo,
            &image.vkImage,
            &image.vmaAllocation,
            nullptr),
            "createImage",
            "vmaCreateImage")

            VkImageViewCreateInfo ivCreateInfo{};
            ivCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            ivCreateInfo.image = image.vkImage;
            ivCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            ivCreateInfo.format = format;
            ivCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.subresourceRange.aspectMask = aspect;
            ivCreateInfo.subresourceRange.baseMipLevel = 0;
            ivCreateInfo.subresourceRange.levelCount = 1;
            ivCreateInfo.subresourceRange.baseArrayLayer = 0;
            ivCreateInfo.subresourceRange.layerCount = 1;

            NEON_CHECK_VULKAN
            (vkCreateImageView
            (vkDevice,
            &ivCreateInfo,
            nullptr,
            &image.vkImageView),
            "createImage",
            "vkCreateImageView")

            return image;
        }

        void destroyImage(Image image)
        {
            vkDestroyImageView
            (vkDevice,
            image.vkImageView,
            nullptr);

            vmaDestroyImage
            (vmaAllocator,
            image.vkImage,
            image.vmaAllocation);
        }

/*        

        Image createImage
        (uint32_t width, uint32_t height,
        VkFormat format,
        VkImageUsageFlags usage,
        VkSampleCountFlagBits samples,
        VkImageAspectFlags aspect)
        {
            Image image;
            image.width = width;
            image.height = height;

            VkImageCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            createInfo.imageType = VK_IMAGE_TYPE_2D;
            createInfo.extent.width = width;
            createInfo.extent.height = height;
            createInfo.extent.depth = 1;
            createInfo.mipLevels = 1;
            createInfo.arrayLayers = 1;
            createInfo.format = format;
            createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            createInfo.usage = usage;
            createInfo.samples = samples;

            NEON_CHECK_VULKAN
            (vkCreateImage
            (vkDevice,
            &createInfo,
            nullptr,
            &image.vkImage),
            "createImage",
            "vkCreateImage")

            VkMemoryRequirements memRequ;
            vkGetImageMemoryRequirements
            (vkDevice,
            image.vkImage,
            &memRequ);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequ.size;
            allocInfo.memoryTypeIndex = 
            findMemoryType
            (memRequ.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            vkPhysicalDevice);

            NEON_CHECK_VULKAN
            (vkAllocateMemory
            (vkDevice,
            &allocInfo,
            nullptr,
            &image.vkDeviceMemory),
            "createImage",
            "vkAllocateMemory")

            NEON_CHECK_VULKAN
            (vkBindImageMemory
            (vkDevice,
            image.vkImage,
            image.vkDeviceMemory,
            0),
            "createImage",
            "vkBindImageMemory")

            VkImageViewCreateInfo ivCreateInfo{};
            ivCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            ivCreateInfo.image = image.vkImage;
            ivCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            ivCreateInfo.format = format;
            ivCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.subresourceRange.aspectMask = aspect;
            ivCreateInfo.subresourceRange.baseMipLevel = 0;
            ivCreateInfo.subresourceRange.levelCount = 1;
            ivCreateInfo.subresourceRange.baseArrayLayer = 0;
            ivCreateInfo.subresourceRange.layerCount = 1;

            NEON_CHECK_VULKAN
            (vkCreateImageView
            (vkDevice,
            &ivCreateInfo,
            nullptr,
            &image.vkImageView),
            "createImage",
            "vkCreateImageView")

            return image;
        }

        void destroyImage(Image image)
        {
            vkDestroyImageView
            (vkDevice,
            image.vkImageView,
            nullptr);

            vkFreeMemory
            (vkDevice,
            image.vkDeviceMemory,
            nullptr);

            vkDestroyImage
            (vkDevice,
            image.vkImage,
            nullptr);
        }
*/
    }
}