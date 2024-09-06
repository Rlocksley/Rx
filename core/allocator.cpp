#include "allocator.hpp"
#include "instance.hpp"
#include "physicalDevice.hpp"
#include "device.hpp"

#define VMA_IMPLEMENTATION
#include "vma/vk_mem_alloc.h"

namespace Neon
{
    namespace Core
    {
        void createAllocator()
        {
            VmaAllocatorCreateInfo createInfo{};
            createInfo.vulkanApiVersion = VK_API_VERSION_1_3;
            createInfo.instance = vkInstance;
            createInfo.physicalDevice = vkPhysicalDevice;
            createInfo.device = vkDevice;
            //createInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
            
            NEON_CHECK_VULKAN
            (vmaCreateAllocator
            (&createInfo,&vmaAllocator),
            "createAllocator",
            "vmaCreateAllocator");

            #ifdef NEON_DEBUG
            NEON_LOGI("Allocator", "created", "")
            #endif
        }

        void destroyAllocator()
        {
            vmaDestroyAllocator(vmaAllocator);
        }
    }
}