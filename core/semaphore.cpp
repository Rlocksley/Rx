#include "semaphore.hpp"
#include "device.hpp"

namespace Neon
{
    namespace Core
    {

        VkSemaphore createSemaphore()
        {
            VkSemaphore vkSemaphore;

            VkSemaphoreCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            NEON_CHECK_VULKAN
            (vkCreateSemaphore
            (vkDevice,
            &createInfo,
            nullptr,
            &vkSemaphore),
            "createSemaphore",
            "vkCreateSemaphore")

            return vkSemaphore;
        }

        void destroySemaphore(VkSemaphore semaphore)
        {
            vkDestroySemaphore
            (vkDevice,
            semaphore,
            nullptr);
        }
    }
}
