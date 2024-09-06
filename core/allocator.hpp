#pragma once

#include "global.hpp"
#include "vma/vk_mem_alloc.h"

namespace Neon
{
    namespace Core
    {
        inline VmaAllocator vmaAllocator;

        void createAllocator();
        void destroyAllocator();
    }
}