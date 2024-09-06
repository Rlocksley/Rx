#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Core
    {
        VkFence createFence(VkFenceCreateFlags flags);
        void destroyFence(VkFence fence);
    }
}