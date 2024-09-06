#include "command.hpp"
#include "physicalDevice.hpp"
#include "device.hpp"
#include "semaphore.hpp"
#include "fence.hpp"

namespace Neon
{
    namespace Core
    {
        Command createCommand()
        {
            Command command;

            VkCommandPoolCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            createInfo.queueFamilyIndex = queueFamilyIndex;

            NEON_CHECK_VULKAN
            (vkCreateCommandPool
            (vkDevice,
            &createInfo,
            nullptr,
            &command.vkCommandPool),
            "createCommand",
            "vkCreateCommandPool")


            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = command.vkCommandPool;
            allocInfo.commandBufferCount = 1;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            NEON_CHECK_VULKAN
            (vkAllocateCommandBuffers
            (vkDevice,
            &allocInfo,
            &command.vkCommandBuffer),
            "createCommand",
            "vkAllocateCommandBuffers")

            command.vkSemaphore = createSemaphore();

            command.vkFence = createFence(VK_FENCE_CREATE_SIGNALED_BIT);

            return command;
        }

        void destroyCommand(Command command)
        {
            destroyFence(command.vkFence);

            destroySemaphore(command.vkSemaphore);

            vkFreeCommandBuffers
            (vkDevice,
            command.vkCommandPool,
            1, &command.vkCommandBuffer);

            vkDestroyCommandPool
            (vkDevice,
            command.vkCommandPool,
            nullptr);
        }

        void waitForCommand(Command command)
        {
            NEON_CHECK_VULKAN
            (vkWaitForFences
            (vkDevice,
            1, &command.vkFence,
            VK_TRUE, UINT64_MAX),
            "waitForCommand",
            "vkWaitForFences")

            NEON_CHECK_VULKAN
            (vkResetFences
            (vkDevice,
            1, &command.vkFence),
            "waitForCommand",
            "vkResetFences")
        }

        
        void beginCommand(Command command)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            NEON_CHECK_VULKAN
            (vkBeginCommandBuffer
            (command.vkCommandBuffer,
            &beginInfo),
            "beginCommand",
            "vkBeginCommandBuffer")
        }

        void endCommand(Command command)
        {
            NEON_CHECK_VULKAN
            (vkEndCommandBuffer
            (command.vkCommandBuffer),
            "endCommand",
            "vkEndCommandBuffer")
        }

        
        SingleCommand createSingleCommand()
        {
            SingleCommand command;

            VkCommandPoolCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            createInfo.queueFamilyIndex = queueFamilyIndex;

            NEON_CHECK_VULKAN
            (vkCreateCommandPool
            (vkDevice,
            &createInfo,
            nullptr,
            &command.vkCommandPool),
            "createCommand",
            "vkCreateCommandPool")


            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = command.vkCommandPool;
            allocInfo.commandBufferCount = 1;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            NEON_CHECK_VULKAN
            (vkAllocateCommandBuffers
            (vkDevice,
            &allocInfo,
            &command.vkCommandBuffer),
            "createCommand",
            "vkAllocateCommandBuffers")

            command.vkFence = createFence(0);

            return command;
        }

        void destroySingleCommand(SingleCommand command)
        {
            destroyFence(command.vkFence);

            vkFreeCommandBuffers
            (vkDevice,
            command.vkCommandPool,
            1, &command.vkCommandBuffer);

            vkDestroyCommandPool
            (vkDevice,
            command.vkCommandPool,
            nullptr);
        }

        void beginSingleCommand(SingleCommand command)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            NEON_CHECK_VULKAN
            (vkBeginCommandBuffer
            (command.vkCommandBuffer,
            &beginInfo),
            "beginSingleCommand",
            "vkBeginCommandBuffer")
        }

        void endSingleCommand(SingleCommand command)
        {
            NEON_CHECK_VULKAN
            (vkEndCommandBuffer
            (command.vkCommandBuffer),
            "endSingleCommand",
            "vkEndCommandBuffer")
        }       
    }
}