#include "Queue.hpp"
#include "PhysicalDevice.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"

namespace Rx
{
    namespace Core
    {
        void getQueue()
        {
            vkGetDeviceQueue
            (vkDevice,
            queueFamilyIndex,
            0,
            &vkQueue);

            #ifdef RX_DEBUG
            RX_LOGI("Queue", "got", "")
            #endif
        }

        void submitSingleCommand(SingleCommand command)
        {
            std::vector<VkPipelineStageFlags> stageFlags = 
            {
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
            };

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.pWaitDstStageMask = stageFlags.data();
            submitInfo.waitSemaphoreCount = 0;
            submitInfo.pWaitSemaphores = nullptr;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &command.vkCommandBuffer;
            submitInfo.signalSemaphoreCount = 0;
            submitInfo.pSignalSemaphores = nullptr;

            RX_CHECK_VULKAN
            (vkQueueSubmit
            (vkQueue,
            1,
            &submitInfo,
            command.vkFence),
            "submitSingleCommand",
            "vkQueueSubmit")

            RX_CHECK_VULKAN
            (vkWaitForFences
            (vkDevice,1,
            &command.vkFence,
            VK_TRUE, UINT64_MAX),
            "submitSingleCommand",
            "vkWaitForFences")

            RX_CHECK_VULKAN
            (vkResetFences
            (vkDevice,
            1,
            &command.vkFence),
            "submitSingleCommand",
            "vkResetFences")
        }

        void submitGraphicsCommand(Command command)
        {
            std::vector<VkPipelineStageFlags> stageFlags = 
            {
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
            };

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.pWaitDstStageMask = stageFlags.data();
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = &swapchain.imageAvailableSemaphores[swapchain.currentSemaphoreIndex];
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &command.vkCommandBuffer;
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &swapchain.renderFinishedSemaphores[swapchain.currentSemaphoreIndex];
            
            RX_CHECK_VULKAN
            (vkQueueSubmit
            (vkQueue,
            1,
            &submitInfo,
            command.vkFence),
            "submitGraphicsCommand",
            "vkQueueSubmit")
        }

        void presentGraphics(Command command)
        {
            VkPresentInfoKHR presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &swapchain.renderFinishedSemaphores[swapchain.currentSemaphoreIndex];
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &swapchain.vkSwapchainKHR;
            presentInfo.pImageIndices = &swapchain.imageIndex;

            RX_CHECK_VULKAN
            (vkQueuePresentKHR
            (vkQueue,
            &presentInfo),
            "presentGraphics",
            "vkQueuePresentKHR")
            
            // Cycle to next semaphore pair
            swapchain.currentSemaphoreIndex = (swapchain.currentSemaphoreIndex + 1) % swapchain.imageAvailableSemaphores.size();
        }
    }
}