#pragma once

#include "Global.hpp"

namespace Rx
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

        inline Shader colorMeshVertexShader("./shader/colorMeshVertexShader.vert.spv");
        inline Shader colorMeshFragmentShader("./shader/colorMeshFragmentShader.frag.spv");

        inline Shader colorMeshArrayVertexShader{"shader/colorMeshArrayVertexShader.vert.spv"};
        inline Shader colorMeshArrayFragmentShader{"shader/colorMeshArrayFragmentShader.frag.spv"};

        inline Shader instancedColorMeshVertexShader{"shader/instancedColorMeshVertexShader.vert.spv"};
        inline Shader instancedColorMeshFragmentShader{"shader/colorMeshArrayFragmentShader.frag.spv"};

        inline Shader textureModelVertexShader{"shader/TextureModelShader.vert.spv"};
        inline Shader textureModelFragmentShader{"shader/TextureModelShader.frag.spv"};

        inline Shader skeletonModelVertexShader{"shader/SkeletonModelShader.vert.spv"};
        inline Shader skeletonModelFragmentShader{"shader/SkeletonModelShader.frag.spv"};

        inline Shader skeletonModelComputeShader{"shader/SkeletonModelShader.comp.spv"};

        inline Shader shadowColorMeshVertexShader{"shader/ShadowColorMeshVertexShader.vert.spv"};

        inline Shader shadowColorModelArrayVertexShader{"shader/ShadowColorModelArrayVertexShader.vert.spv"};

        inline Shader shadowTextureModelVertexShader{"shader/ShadowTextureModelVertexShader.vert.spv"};

        inline Shader shadowSkeletonModelVertexShader{"shader/ShadowSkeletonModelVertexShader.vert.spv"};
    }
}