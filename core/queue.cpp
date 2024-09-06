#include "queue.hpp"
#include "physicalDevice.hpp"
#include "device.hpp"
#include "swapchain.hpp"

namespace Neon
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

            #ifdef NEON_DEBUG
            NEON_LOGI("Queue", "got", "")
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

            NEON_CHECK_VULKAN
            (vkQueueSubmit
            (vkQueue,
            1,
            &submitInfo,
            command.vkFence),
            "submitSingleCommand",
            "vkQueueSubmit")

            NEON_CHECK_VULKAN
            (vkWaitForFences
            (vkDevice,1,
            &command.vkFence,
            VK_TRUE, UINT64_MAX),
            "submitSingleCommand",
            "vkWaitForFences")

            NEON_CHECK_VULKAN
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
            submitInfo.pWaitSemaphores = &swapchain.vkSemaphore[Neon::Core::commandIndex];
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &command.vkCommandBuffer;
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &command.vkSemaphore;
            
            NEON_CHECK_VULKAN
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
            presentInfo.pWaitSemaphores = &command.vkSemaphore;
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &swapchain.vkSwapchainKHR;
            presentInfo.pImageIndices = &swapchain.imageIndex;

            NEON_CHECK_VULKAN
            (vkQueuePresentKHR
            (vkQueue,
            &presentInfo),
            "presentGraphics",
            "vkQueuePresentKHR")
        }
    }
}