#pragma once

#include "global.hpp"
#include "vma/vk_mem_alloc.h"

namespace Neon
{
    namespace Core
    {

        struct Buffer
        {
            uint32_t numberElements;
            uint32_t elementSize;
            VkBuffer vkBuffer;
            VmaAllocation vmaAllocation;
            VmaAllocationInfo vmaAllocationInfo;
        };

        struct BufferInterface
        {
            uint32_t numberElements;
            uint32_t elementSize;
            VkBuffer vkBuffer;
            VmaAllocation vmaAllocation;
            VmaAllocationInfo vmaAllocationInfo;
            void* pMemory;
        };

/*        
        struct Buffer
        {
            uint32_t numberElements;
            uint32_t elementSize;
            VkBuffer vkBuffer;
            VkDeviceMemory vkDeviceMemory;
        };

        struct BufferInterface
        {
            uint32_t numberElements;
            uint32_t elementSize;
            VkBuffer vkBuffer;
            VkDeviceMemory vkDeviceMemory;
            void* pMemory;
        };
*/
        Buffer createBuffer
        (uint32_t numberElements, uint32_t elementSize,
        VkBufferUsageFlags usage);
        void destroyBuffer(Buffer buffer);

        BufferInterface createBufferInterface
        (uint32_t numberElements, uint32_t elementSize,
        VkBufferUsageFlags usage);
        void destroyBufferInterface(BufferInterface buffer);
    }
}