#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Core
    {
        VkSemaphore createSemaphore();
        void destroySemaphore(VkSemaphore semaphore);
    }
}