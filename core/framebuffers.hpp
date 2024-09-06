#pragma once

#include "global.hpp"
#include "image.hpp"

namespace Neon
{
    namespace Core
    {
        struct Framebuffers
        {
            Image colorImage;
            Image depthImage;
            std::vector<VkFramebuffer> vkFramebuffers{};
        };

        inline Framebuffers framebuffers;

        void createFramebuffers();
        void destroyFramebuffers();
    }
}