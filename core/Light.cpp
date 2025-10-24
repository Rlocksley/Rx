#include "Light.hpp"

namespace Rx{
    namespace Core{

        void createLightBuffers(){
            pointLightBuffer = 
            createBufferInterface
            (1, sizeof(PointLightBuffer),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

            directionalLightBuffer =
            createBufferInterface
            (1, sizeof(DirectionalLightBuffer),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

            spotLightBuffer =
            createBufferInterface
            (1, sizeof(SpotLightBuffer),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

            shadowSpotLightBuffer =
            createBufferInterface
            (1, sizeof(ShadowSpotLightBuffer),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        }

        void destroyLightBuffers(){
            destroyBufferInterface(shadowSpotLightBuffer);
            destroyBufferInterface(pointLightBuffer);
            destroyBufferInterface(directionalLightBuffer);
            destroyBufferInterface(spotLightBuffer);
        }
    }
}