#pragma once

#include "Window.hpp"
#include "Time.hpp"
#include "Input.hpp"

#include "Instance.hpp"
#include "Surface.hpp"
#include "PhysicalDevice.hpp"
#include "Device.hpp"
#include "Queue.hpp"
#include "Allocator.hpp"
#include "DescriptorPool.hpp"

#include "Image.hpp"
#include "Buffer.hpp"

#include "Eye.hpp"

#include "Semaphore.hpp"
#include "Fence.hpp"
#include "Command.hpp"

#include "Swapchain.hpp"
#include "RenderPass.hpp"
#include "Framebuffers.hpp"

#include "Light.hpp"

#include "DescriptorSetLayouts.hpp"
#include "PipelineLayouts.hpp"
#include "Pipelines.hpp"



namespace Rx
{
    namespace Core
    {
        void createCore();
        void destroyCore();
        bool updateCore();
    }
}