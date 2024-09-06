#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Core
    {
        //PhysicalDeviceInfo
        inline uint32_t queueFamilyIndex;
        inline VkPresentModeKHR vkPresentModeKHR;
        inline VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures;
        inline VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
        inline VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties;
        
        //SurfaceInfo
        inline VkSurfaceFormatKHR vkSurfaceFormatKHR;
        inline VkExtent2D vkExtent2D;
        inline VkSurfaceCapabilitiesKHR vkSurfaceCapabilitiesKHR;
        inline VkSampleCountFlagBits maxVkSampleCountFlagBits;
        inline VkSampleCountFlagBits vkSampleCountFlagBits;

        //Depth Format
        inline VkFormat depthVkFormat;

        //PhysicalDevice
        inline VkPhysicalDevice vkPhysicalDevice;

        void pickPhysicalDevice();

        uint32_t findMemoryType
        (uint32_t typeFilter, VkMemoryPropertyFlags properties,
        VkPhysicalDevice vkPhysicalDevice);
        
        VkFormat findSupportedFormat(std::vector<VkFormat>& vkFormats, const VkFormatFeatureFlags vkFormatFeatureFlags);
        
    }
}