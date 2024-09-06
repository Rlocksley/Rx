#include "fence.hpp"
#include "device.hpp"

namespace Neon
{
    namespace Core
    {

        VkFence createFence(VkFenceCreateFlags flags)
        {
            VkFence vkFence;

            VkFenceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            createInfo.flags = flags;

            NEON_CHECK_VULKAN
            (vkCreateFence
            (vkDevice,
            &createInfo,
            nullptr,
            &vkFence),
            "createFence",
            "vkCreateFence")

            return vkFence;
        }   

        void destroyFence(VkFence fence)
        {
            vkDestroyFence
            (vkDevice,
            fence,
            nullptr);
        }
    }
}