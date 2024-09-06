#include "instance.hpp"

namespace Neon
{
namespace Core
{


#ifdef NEON_DEBUG

    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
    (VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* callBackData,
    void* userData)
    {
        (void) (severity);
        (void) (type);
        (void) (userData);

        printf("%s\n", callBackData->pMessage);

        return VK_FALSE;
    }

    void checkValidationLayerSupport()
    {
        uint32_t layerCount;
        
        NEON_CHECK_VULKAN
        (vkEnumerateInstanceLayerProperties
        (&layerCount, NULL),
        "checkValidationLayerSupport",
        "vkEnumerateInstanceLayerProperties")
        
        std::vector<VkLayerProperties> availableLayers(layerCount);
        
        NEON_CHECK_VULKAN
        (vkEnumerateInstanceLayerProperties
        (&layerCount, availableLayers.data()),
        "checkValidationLayerSupport",
        "vkEnumerateInstanceLayerProperties")

        for(size_t i = 0; i < validationLayers.size(); i++)
        {
            bool layerFound = false;
            for(size_t j = 0; j < availableLayers.size(); j++)
            {
                printf("%s\n",availableLayers[j].layerName);
                if(strcmp(validationLayers[i], availableLayers[j].layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if(!layerFound)
            {
                NEON_LOGE("checkValidationLayerSupport", "no validationLayerSupport for", validationLayers[i])
            }
        }
    }

    void createVkDebugUtilsMessenger()
    {
        PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(vkInstance,"vkCreateDebugUtilsMessengerEXT");

        if(func == NULL)
        {
            NEON_LOGE("createVkDebugUtilsMessengerEXT", "vkGetInstanceProcAddr()", "failed");
        }

        VkDebugUtilsMessengerCreateInfoEXT vkDebugUtilsMessengerCreateInfoEXT;
        vkDebugUtilsMessengerCreateInfoEXT.sType =
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        vkDebugUtilsMessengerCreateInfoEXT.flags = 0;
        vkDebugUtilsMessengerCreateInfoEXT.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        vkDebugUtilsMessengerCreateInfoEXT.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        vkDebugUtilsMessengerCreateInfoEXT.pfnUserCallback =
        &debugCallback;
        vkDebugUtilsMessengerCreateInfoEXT.pNext = NULL;

        if(func
        (vkInstance,
        &vkDebugUtilsMessengerCreateInfoEXT,
        NULL,
        &vkDebugUtilsMessengerEXT) != VK_SUCCESS)
        {
            NEON_LOGE("createVkDebugUtilsMessengerEXT", "vkCreateDebugUtilsMessengerEXT()", "failed");
        }

    }

    void destroyVkDebugUtilsMessenger()
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func =
        (PFN_vkDestroyDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugUtilsMessengerEXT");

        if(func == NULL)
        {
            NEON_LOGE("destroyVkDebugUtilsMessengerEXT", "vkGetInstanceProcAddr()", "failed");
        }
        
        func
        (vkInstance,
        vkDebugUtilsMessengerEXT,
        NULL);
        
    }

#endif

    void createInstance()
    {
        #ifdef NEON_DEBUG
        checkValidationLayerSupport();
        #endif

         VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.apiVersion = VK_API_VERSION_1_3;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pApplicationName = applicationName.data();
        appInfo.pEngineName = "NEON";
        appInfo.pNext = NULL;

        VkInstanceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.pApplicationInfo = &appInfo;

        
        #ifdef NEON_DEBUG

        uint32_t glfwExtensionsCount;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
        std::vector<const char*> instanceExtensions(glfwExtensionsCount+1);
        memcpy(instanceExtensions.data(), glfwExtensions, glfwExtensionsCount*sizeof(const char*));
        instanceExtensions[glfwExtensionsCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        createInfo.enabledExtensionCount = glfwExtensionsCount+1;
        createInfo.ppEnabledExtensionNames = instanceExtensions.data();

        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        VkDebugUtilsMessengerCreateInfoEXT vkDebugUtilsMessengerCreateInfoEXT;
        vkDebugUtilsMessengerCreateInfoEXT.sType =
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        vkDebugUtilsMessengerCreateInfoEXT.flags = 0;
        vkDebugUtilsMessengerCreateInfoEXT.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        vkDebugUtilsMessengerCreateInfoEXT.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        vkDebugUtilsMessengerCreateInfoEXT.pfnUserCallback =
        &debugCallback;
        vkDebugUtilsMessengerCreateInfoEXT.pNext = NULL;

        createInfo.pNext = &vkDebugUtilsMessengerCreateInfoEXT;
        
        #else

        uint32_t glfwExtensionsCount;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
        createInfo.enabledExtensionCount = glfwExtensionsCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = NULL;
        createInfo.pNext = NULL;

        #endif

        NEON_CHECK_VULKAN
        (vkCreateInstance
        (&createInfo, NULL,
        &vkInstance),
        "createInstance",
        "vkCreateInstance")

        #ifdef NEON_DEBUG
        
        NEON_LOGI("VkInstance", "created", "")
        
        createVkDebugUtilsMessenger();

        NEON_LOGI("VkDebugUtilsMessengerEXT", "created", "")
        
        #endif
    }

    void destroyInstance()
    {
        #ifdef NEON_DEBUG
        destroyVkDebugUtilsMessenger();
        #endif

        vkDestroyInstance
        (vkInstance,
        nullptr);
    }

}
}