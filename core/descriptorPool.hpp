#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Core
    {
        inline uint32_t maxNumberUniformBuffers{10000};
        inline uint32_t maxNumberStorageBuffers{10000};
        inline uint32_t maxNumberCombinedImageSamplers{10000};
        inline uint32_t maxNumberDescriptorSets{10000};
        
        inline VkDescriptorPool vkDescriptorPool;

        void createDescriptorPool();
        void destroyDescriptorPool();
    }
}