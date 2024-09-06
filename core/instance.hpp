#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Core
    {
        inline std::string applicationName;
        inline VkInstance vkInstance;

        #ifdef NEON_DEBUG

        // write down which validationLayers you want
        inline std::vector<const char*> validationLayers = 
        {
            //"VK_LAYER_MESA_device_select",
            //"VK_LAYER_KHRONOS_profiles",
            //"VK_LAYER_KHRONOS_shader_object",
            //"VK_LAYER_INTEL_nullhw",
            //"VK_LAYER_LUNARG_gfxreconstruct",
            //"VK_LAYER_LUNARG_api_dump",
            //"VK_LAYER_LUNARG_monitor",
            //"VK_LAYER_MESA_overlay",
            //"VK_LAYER_KHRONOS_synchronization2",
            //"VK_LAYER_LUNARG_screenshot",
            "VK_LAYER_KHRONOS_validation"
        };

        inline VkDebugUtilsMessengerEXT vkDebugUtilsMessengerEXT;

        #endif

        void createInstance();
    
        void destroyInstance();

    
    }
}