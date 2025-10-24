#include "Image.hpp"
#include "PhysicalDevice.hpp"
#include "Device.hpp"
#include "Allocator.hpp"
#include "Command.hpp"
#include "Mutex.hpp"
#include "Buffer.hpp"
#include "Queue.hpp"

namespace Rx
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

            RX_CHECK_VULKAN
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

            RX_CHECK_VULKAN
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

        Texture createTexture(uint32_t width, uint32_t height, void* pixels){
            Texture image;
            image.width = width;
            image.height = height;

            VkImageCreateInfo createInfo;
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            createInfo.flags = 0;
            createInfo.imageType = VK_IMAGE_TYPE_2D;
            createInfo.extent.width = width;
            createInfo.extent.height = height;
            createInfo.extent.depth = 1;
            createInfo.mipLevels = 1;
            createInfo.arrayLayers = 1;
            createInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
            createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            createInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = NULL;
            createInfo.pNext = NULL;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
            allocInfo.priority = 1.f;

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vmaCreateImage
            (vmaAllocator,
            &createInfo,
            &allocInfo,
            &image.vkImage,
            &image.vmaAllocation,
            nullptr),
            "createImage",
            "vmaCreateImage"))

            VkImageViewCreateInfo ivCreateInfo;
            ivCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            ivCreateInfo.flags = 0;
            ivCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            ivCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
            ivCreateInfo.image = image.vkImage;
            ivCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            ivCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            ivCreateInfo.subresourceRange.baseMipLevel = 0;
            ivCreateInfo.subresourceRange.levelCount = 1;
            ivCreateInfo.subresourceRange.baseArrayLayer = 0;
            ivCreateInfo.subresourceRange.layerCount = 1;
            ivCreateInfo.pNext = NULL;

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateImageView
            (vkDevice,
            &ivCreateInfo, NULL,
            &image.vkImageView),
            "createImage",
            "vkCreateImageView"))
            
            VkSamplerCreateInfo samplerCreateInfo;
            samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerCreateInfo.flags = 0;
            samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
            samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
            samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerCreateInfo.anisotropyEnable = VK_TRUE;
            samplerCreateInfo.maxAnisotropy = vkPhysicalDeviceProperties.limits.maxSamplerAnisotropy;
            samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
            samplerCreateInfo.compareEnable = VK_FALSE;
            samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerCreateInfo.mipLodBias = 0.f;
            samplerCreateInfo.minLod = 0.f;
            samplerCreateInfo.maxLod = 0.f;
            samplerCreateInfo.pNext = NULL;

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateSampler
            (vkDevice,
            &samplerCreateInfo, 
            NULL,
            &image.vkSampler),
            "createImage",
            "vkCreateSampler"))

            BufferInterface copyBuffer =
            createBufferInterface
            (width * height, 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            memcpy(copyBuffer.pMemory, pixels, width * height * 4);

            VkImageMemoryBarrier barrier0;
            barrier0.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier0.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier0.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier0.srcAccessMask = 0;
            barrier0.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier0.image = image.vkImage;
            barrier0.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier0.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier0.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier0.subresourceRange.baseMipLevel = 0;
            barrier0.subresourceRange.levelCount = 1;
            barrier0.subresourceRange.baseArrayLayer = 0;
            barrier0.subresourceRange.layerCount = 1;
            barrier0.pNext = NULL;

            VkBufferImageCopy copy;
            copy.bufferOffset = 0;
            copy.bufferRowLength = 0;
            copy.bufferImageHeight = 0;
            copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copy.imageSubresource.mipLevel = 0;
            copy.imageSubresource.baseArrayLayer = 0;
            copy.imageSubresource.layerCount = 1;
            copy.imageOffset.x = 0;
            copy.imageOffset.y = 0;
            copy.imageOffset.z = 0;
            copy.imageExtent.width = width;
            copy.imageExtent.height = height;
            copy.imageExtent.depth = 1;

            VkImageMemoryBarrier barrier1;
            barrier1.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier1.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier1.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier1.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier1.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            barrier1.image = image.vkImage;
            barrier1.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier1.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier1.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier1.subresourceRange.baseMipLevel = 0;
            barrier1.subresourceRange.levelCount = 1;
            barrier1.subresourceRange.baseArrayLayer = 0;
            barrier1.subresourceRange.layerCount = 1;
            barrier1.pNext = NULL;

            RX_VK_MUTEX(
            beginSingleCommand(singleCommand);

            vkCmdPipelineBarrier
            (singleCommand.vkCommandBuffer,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0, NULL,
            0, NULL,
            1, &barrier0);

            vkCmdCopyBufferToImage
            (singleCommand.vkCommandBuffer,
            copyBuffer.vkBuffer,
            image.vkImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &copy);

            vkCmdPipelineBarrier
            (singleCommand.vkCommandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0,
            0, NULL,
            0, NULL,
            1, &barrier1);

            endSingleCommand(singleCommand);
            submitSingleCommand(singleCommand);)

            destroyBufferInterface(copyBuffer);

            return image;

        }

        void destroyTexture(Texture& texture){
            RX_VK_MUTEX(
            vkDestroySampler(vkDevice, texture.vkSampler, nullptr);
            vkDestroyImageView(vkDevice, texture.vkImageView, nullptr);
            vmaDestroyImage(vmaAllocator, texture.vkImage, texture.vmaAllocation);)
        }

        void createShadowMapArray(uint32_t width, uint32_t height, uint32_t count, VkFormat format){
            shadowMapArray.width = width;
            shadowMapArray.height = height;
            shadowMapArray.count = count;
            shadowMapArray.format = format;

            // Create the Vulkan image
            VkImageCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            createInfo.imageType = VK_IMAGE_TYPE_2D;
            createInfo.extent.width = width;
            createInfo.extent.height = height;
            createInfo.extent.depth = 1;
            createInfo.mipLevels = 1;
            createInfo.arrayLayers = count;
            createInfo.format = format;
            createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            createInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            createInfo.samples = VK_SAMPLE_COUNT_1_BIT;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
            allocInfo.priority = 1.f;

            RX_CHECK_VULKAN
            (vmaCreateImage
            (vmaAllocator,
            &createInfo,
            &allocInfo,
            &shadowMapArray.vkImage,
            &shadowMapArray.vmaAllocation,
            &shadowMapArray.vmaAllocationInfo),
            "createShadowMapArray",
            "vkCreateImage")

           

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = shadowMapArray.vkImage;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
            viewInfo.format = format;
            viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = count;

            RX_CHECK_VULKAN
            (vkCreateImageView(vkDevice, &viewInfo, nullptr, &shadowMapArray.vkImageView),
            "createShadowMapArray",
            "vkCreateImageView")

            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_NEAREST;
            samplerInfo.minFilter = VK_FILTER_NEAREST;
            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_TRUE;
            samplerInfo.compareOp = VK_COMPARE_OP_LESS;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

            RX_CHECK_VULKAN
            (vkCreateSampler(vkDevice, &samplerInfo, nullptr, &shadowMapArray.vkSampler),
            "createShadowMapArray",
            "vkCreateSampler")
        }

        void destroyShadowMapArray(){
            vkDestroySampler(vkDevice, shadowMapArray.vkSampler, nullptr);
            vkDestroyImageView(vkDevice, shadowMapArray.vkImageView, nullptr);
            vmaDestroyImage(vmaAllocator, shadowMapArray.vkImage, shadowMapArray.vmaAllocation);
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

            RX_CHECK_VULKAN
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

            RX_CHECK_VULKAN
            (vkAllocateMemory
            (vkDevice,
            &allocInfo,
            nullptr,
            &image.vkDeviceMemory),
            "createImage",
            "vkAllocateMemory")

            RX_CHECK_VULKAN
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

            RX_CHECK_VULKAN
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