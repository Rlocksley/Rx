#include "surface.hpp"
#include "window.hpp"
#include "instance.hpp"

namespace Neon
{
namespace Core
{
    void createSurface()
    {
        NEON_CHECK_VULKAN
        (glfwCreateWindowSurface
        (vkInstance,
        pGLFWwindow,
        nullptr,
        &vkSurfaceKHR),
        "createSurface",
        "glfwCreateWindowSurface")

        #ifdef NEON_DEBUG
        NEON_LOGI("VkSurfaceKHR" , "created", "")
        #endif
    }

    void destroySurface()
    {
        vkDestroySurfaceKHR
        (vkInstance,
        vkSurfaceKHR,
        nullptr);
    }
}
}