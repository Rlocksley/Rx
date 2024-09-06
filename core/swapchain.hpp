#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Core
    {
        struct Swapchain
        {
            std::vector<VkImage> vkImages{};
            std::vector<VkImageView> vkImageViews{};
            VkSwapchainKHR vkSwapchainKHR;
            VkSemaphore vkSemaphore[2];
            uint32_t imageIndex;
        };

        inline Swapchain swapchain;

        void createSwapchain();
        void destroySwapchain();
        void getSwapchainImageIndex();
    }
}