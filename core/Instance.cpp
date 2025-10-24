#include "Instance.hpp"

namespace Rx
{
namespace Core
{


#ifdef RX_DEBUG

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
        
        RX_CHECK_VULKAN
        (vkEnumerateInstanceLayerProperties
        (&layerCount, NULL),
        "checkValidationLayerSupport",
        "vkEnumerateInstanceLayerProperties")
        
        std::vector<VkLayerProperties> availableLayers(layerCount);
        
        RX_CHECK_VULKAN
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
                RX_LOGE("checkValidationLayerSupport", "no validationLayerSupport for", validationLayers[i])
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
            RX_LOGE("createVkDebugUtilsMessengerEXT", "vkGetInstanceProcAddr()", "failed");
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
            RX_LOGE("createVkDebugUtilsMessengerEXT", "vkCreateDebugUtilsMessengerEXT()", "failed");
        }

    }

    void destroyVkDebugUtilsMessenger()
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func =
        (PFN_vkDestroyDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugUtilsMessengerEXT");

        if(func == NULL)
        {
            RX_LOGE("destroyVkDebugUtilsMessengerEXT", "vkGetInstanceProcAddr()", "failed");
        }
        
        func
        (vkInstance,
        vkDebugUtilsMessengerEXT,
        NULL);
        
    }

#endif

    void createInstance()
    {
        #ifdef RX_DEBUG
        checkValidationLayerSupport();
        #endif

         VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.apiVersion = VK_API_VERSION_1_3;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pApplicationName = applicationName.data();
        appInfo.pEngineName = "RX";
        appInfo.pNext = NULL;

        VkInstanceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        createInfo.pNext = NULL;
        createInfo.pApplicationInfo = &appInfo;

        
        #ifdef RX_DEBUG

        uint32_t glfwExtensionsCount;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
        std::vector<const char*> instanceExtensions(glfwExtensionsCount+1);
        memcpy(instanceExtensions.data(), glfwExtensions, glfwExtensionsCount*sizeof(const char*));
        instanceExtensions[glfwExtensionsCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        //instanceExtensions[glfwExtensionsCount+1] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
        createInfo.enabledExtensionCount = instanceExtensions.size();
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

        RX_CHECK_VULKAN
        (vkCreateInstance
        (&createInfo, NULL,
        &vkInstance),
        "createInstance",
        "vkCreateInstance")

        #ifdef RX_DEBUG
        
        RX_LOGI("VkInstance", "created", "")
        
        createVkDebugUtilsMessenger();

        RX_LOGI("VkDebugUtilsMessengerEXT", "created", "")
        
        #endif
    }

    void destroyInstance()
    {
        #ifdef RX_DEBUG
        destroyVkDebugUtilsMessenger();
        #endif

        vkDestroyInstance
        (vkInstance,
        nullptr);
    }

}
}