#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Core
    {
        inline std::vector<const char*> deviceExtensions
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };


        inline VkDevice vkDevice;

        void createDevice();
        void destroyDevice();
    }
}