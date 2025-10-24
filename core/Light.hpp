#pragma once
#include "Buffer.hpp"
namespace Rx{
    namespace Core{
        struct PointLight{
            glm::vec4 position;
            glm::vec4 color;
            glm::vec4 intensity;
        };

        struct PointLightBuffer{
            glm::vec4 numberPointLights;
            PointLight lights[256];
        };

        struct  DirectionalLight{
            glm::vec4 direction;
            glm::vec4 color;
            glm::vec4 intensity;
        };

        struct DirectionalLightBuffer{
            glm::vec4 numberDirectionalLights;
            DirectionalLight lights[256];
        };

        struct SpotLight{
            glm::vec4 position;
            glm::vec4 direction;
            glm::vec4 color;
            glm::vec4 intensity;
            glm::vec4 cutoff;
        };

        struct SpotLightBuffer{
            int numberSpotLights;
            SpotLight lights[256];
        };

        struct ShadowSpotLight{
            glm::vec4 position;
            glm::vec4 direction;
            glm::vec4 color;
            glm::vec4 intensity;
            glm::mat4 lightSpaceMatrix;
        };

        struct ShadowSpotLightBuffer{
            glm::ivec4 numberShadowSpotLights;
            ShadowSpotLight lights[16];
        };

        inline BufferInterface pointLightBuffer;
        inline BufferInterface directionalLightBuffer;
        inline BufferInterface spotLightBuffer;
        inline BufferInterface shadowSpotLightBuffer;

        void createLightBuffers();
        void destroyLightBuffers();
    }
}