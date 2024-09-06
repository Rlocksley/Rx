#pragma once

#include "global.hpp"


namespace Neon
{
    namespace Core
    {
        
        inline std::string windowTitle;
        inline uint32_t windowWidth;
        inline uint32_t windowHeight;
        inline bool fullScreen;
        inline GLFWwindow* pGLFWwindow;

        void createWindow();
        void destroyWindow();
   }
}