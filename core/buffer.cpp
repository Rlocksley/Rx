#include "buffer.hpp"
#include "physicalDevice.hpp"
#include "device.hpp"
#include "allocator.hpp"

namespace Neon
{
    namespace Core
    {

        Buffer createBuffer
        (uint32_t numberElements, uint32_t elementSize,
        VkBufferUsageFlags usage)
        {
            Buffer buffer;
            buffer.numberElements = numberElements;
            buffer.elementSize = elementSize;

            VkBufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            createInfo.size = numberElements * elementSize;
            createInfo.usage = usage;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

            NEON_CHECK_VULKAN
            (vmaCreateBuffer
            (vmaAllocator,
            &createInfo,
            &allocInfo,
            &buffer.vkBuffer,
            &buffer.vmaAllocation,
            &buffer.vmaAllocationInfo),
            "createBuffer",
            "vmaCreateBuffer")

            return buffer;
        }

        void destroyBuffer(Buffer buffer)
        {
            vmaDestroyBuffer
            (vmaAllocator,
            buffer.vkBuffer,
            buffer.vmaAllocation);
        }

        BufferInterface createBufferInterface
        (uint32_t numberElements, uint32_t elementSize,
        VkBufferUsageFlags usage)
        {
            BufferInterface buffer;
            buffer.numberElements = numberElements;
            buffer.elementSize = elementSize;

            VkBufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            createInfo.size = numberElements * elementSize;
            createInfo.usage = usage;

            VmaAllocationCreateInfo allocInfo{};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT |
                            VMA_ALLOCATION_CREATE_MAPPED_BIT;

            NEON_CHECK_VULKAN
            (vmaCreateBuffer
            (vmaAllocator,
            &createInfo,
            &allocInfo,
            &buffer.vkBuffer,
            &buffer.vmaAllocation,
            &buffer.vmaAllocationInfo),
            "createBufferInterface",
            "vmaCreateBuffer")

            buffer.pMemory = buffer.vmaAllocationInfo.pMappedData;

            return buffer;
        }

        void destroyBufferInterface(BufferInterface buffer)
        {
            vmaDestroyBuffer
            (vmaAllocator,
            buffer.vkBuffer,
            buffer.vmaAllocation);
        }
/*
        Buffer createBuffer
        (uint32_t numberElements, uint32_t elementSize,
        VkBufferUsageFlags usage)
        {
            Buffer buffer;
            buffer.numberElements = numberElements;
            buffer.elementSize = elementSize;

            VkBufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            createInfo.size = numberElements * elementSize;
            createInfo.usage = usage;

            NEON_CHECK_VULKAN
            (vkCreateBuffer
            (vkDevice,
            &createInfo,
            nullptr,
            &buffer.vkBuffer),
            "createBuffer",
            "vkCreateBuffer")

            VkMemoryRequirements memRequ;
            vkGetBufferMemoryRequirements
            (vkDevice,
            buffer.vkBuffer,
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
            &buffer.vkDeviceMemory),
            "createBuffer",
            "vkAllocateMemory")

            NEON_CHECK_VULKAN
            (vkBindBufferMemory
            (vkDevice,
            buffer.vkBuffer,
            buffer.vkDeviceMemory,
            0),
            "createBuffer",
            "vkBindBufferMemory")

            return buffer;
        }

        void destroyBuffer(Buffer buffer)
        {
            vkFreeMemory
            (vkDevice,
            buffer.vkDeviceMemory,
            nullptr);

            vkDestroyBuffer
            (vkDevice,
            buffer.vkBuffer,
            nullptr);
        }

        BufferInterface createBufferInterface
        (uint32_t numberElements, uint32_t elementSize,
        VkBufferUsageFlags usage)
        {
            BufferInterface buffer;
            buffer.numberElements = numberElements;
            buffer.elementSize = elementSize;

            VkBufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            createInfo.size = numberElements * elementSize;
            createInfo.usage = usage;

            NEON_CHECK_VULKAN
            (vkCreateBuffer
            (vkDevice,
            &createInfo,
            nullptr,
            &buffer.vkBuffer),
            "createBufferInterface",
            "vkCreateBuffer")

            VkMemoryRequirements memRequ;
            vkGetBufferMemoryRequirements
            (vkDevice,
            buffer.vkBuffer,
            &memRequ);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequ.size;
            allocInfo.memoryTypeIndex = 
            findMemoryType
            (memRequ.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            vkPhysicalDevice);

            NEON_CHECK_VULKAN
            (vkAllocateMemory
            (vkDevice,
            &allocInfo,
            nullptr,
            &buffer.vkDeviceMemory),
            "createBufferInterface",
            "vkAllocateMemory")

            NEON_CHECK_VULKAN
            (vkBindBufferMemory
            (vkDevice,
            buffer.vkBuffer,
            buffer.vkDeviceMemory,
            0),
            "createBufferInterface",
            "vkBindBufferMemory")

            NEON_CHECK_VULKAN
            (vkMapMemory
            (vkDevice,
            buffer.vkDeviceMemory,
            0,
            buffer.numberElements*
            buffer.elementSize,
            0,
            &buffer.pMemory),
            "createBufferInterface",
            "vkMapMemory")

            return buffer;
        }

        void destroyBufferInterface(BufferInterface buffer)
        {
            vkFreeMemory
            (vkDevice,
            buffer.vkDeviceMemory,
            nullptr);

            vkDestroyBuffer
            (vkDevice,
            buffer.vkBuffer,
            nullptr);
        }
*/
    }
}