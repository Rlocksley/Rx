#include "window.hpp"

namespace Neon
{
    namespace Core
    {

        void createWindow()
        {
             glfwInit();
        
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            
            if(fullScreen)
            {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                windowWidth = static_cast<uint32_t>(mode->width);
                windowHeight = static_cast<uint32_t>(mode->height);

                if((pGLFWwindow = glfwCreateWindow
                (mode->width, mode->height, windowTitle.data(), monitor, nullptr)) == nullptr)
                {
                    NEON_LOGE("createWindow", "glfwCreateWindow", "failed")
                }
            }
            else
            {
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

                if((pGLFWwindow = glfwCreateWindow
                (static_cast<int>(windowWidth), static_cast<int>(windowHeight), windowTitle.data(), nullptr, nullptr)) == nullptr)
                {
                    NEON_LOGE("createWindow", "glfwCreateWindow", "failed")
                }
            }

            glfwSetInputMode(pGLFWwindow, GLFW_STICKY_KEYS, GLFW_TRUE);
            glfwSetInputMode(pGLFWwindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

            #ifdef NEON_DEBUG
            NEON_LOGI("Window", "created", "")
            #endif
        }
    

        void destroyWindow()
        {
            glfwDestroyWindow(pGLFWwindow);
        }

    }
}