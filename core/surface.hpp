#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Core
    {
        inline VkSurfaceKHR vkSurfaceKHR;

        void createSurface();
        void destroySurface();
    }
}