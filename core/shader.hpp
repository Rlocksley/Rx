#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Core
    {
        struct Shader
        {
        public:
            Shader(std::string filePath);
            ~Shader()  = default;

        private:
            std::vector<char> code{};

        public:
            VkShaderModule createShaderModule();
        };
    }
}