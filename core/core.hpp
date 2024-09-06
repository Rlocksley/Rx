#pragma once

#include "window.hpp"
#include "time.hpp"
#include "input.hpp"

#include "instance.hpp"
#include "surface.hpp"
#include "physicalDevice.hpp"
#include "device.hpp"
#include "queue.hpp"
#include "allocator.hpp"
#include "descriptorPool.hpp"

#include "image.hpp"
#include "buffer.hpp"

#include "eye.hpp"

#include "semaphore.hpp"
#include "fence.hpp"
#include "command.hpp"

#include "swapchain.hpp"
#include "renderPass.hpp"
#include "framebuffers.hpp"

namespace Neon
{
    namespace Core
    {
        void createCore();
        void destroyCore();
        bool updateCore();
    }
}