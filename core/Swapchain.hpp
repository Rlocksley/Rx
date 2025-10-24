#pragma once

#include "Global.hpp"

namespace Rx
{
    namespace Core
    {
        struct Swapchain
        {
            std::vector<VkImage> vkImages{};
            std::vector<VkImageView> vkImageViews{};
            VkSwapchainKHR vkSwapchainKHR;
            std::vector<VkSemaphore> imageAvailableSemaphores{};  // One per swapchain image
            std::vector<VkSemaphore> renderFinishedSemaphores{};  // One per swapchain image
            uint32_t imageIndex;
            uint32_t currentSemaphoreIndex{0};  // Which semaphore pair we're currently using
        };

        inline Swapchain swapchain;

        void createSwapchain();
        void destroySwapchain();
        void getSwapchainImageIndex();
    }
}