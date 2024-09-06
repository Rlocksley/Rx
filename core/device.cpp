#include "device.hpp"
#include "physicalDevice.hpp"

namespace Neon
{
    namespace Core
    {

        void createDevice()
        {
            float priority = 1.f;
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &priority;

            VkDeviceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
            createInfo.queueCreateInfoCount = 1;
            createInfo.pQueueCreateInfos = &queueCreateInfo;
            createInfo.pEnabledFeatures = &vkPhysicalDeviceFeatures;
            createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()); 
            createInfo.ppEnabledExtensionNames = deviceExtensions.data();

            NEON_CHECK_VULKAN
            (vkCreateDevice
            (vkPhysicalDevice,
            &createInfo,
            nullptr,
            &vkDevice),
            "createDevice",
            "vkCreateDevice")

            #ifdef NEON_DEBUG
            NEON_LOGI("Device", "created", "")
            #endif
        }

        void destroyDevice()
        {
            vkDestroyDevice
            (vkDevice,
            nullptr);
        }
    }
}