#pragma once

#include "Global.hpp"


namespace Rx
{
    namespace Core
    {
        void createPipelines();
        void destroyPipelines();

        inline VkPipeline colorMeshPipeline;
        void createColorMeshPipeline();
        void destroyColorMeshPipeline();

        inline VkPipeline colorMeshArrayPipeline;
        void createColorMeshArrayPipeline();
        void destroyColorMeshArrayPipeline();

        inline VkPipeline instancedColorMeshPipeline;
        void createInstancedColorMeshPipeline();
        void destroyInstancedColorMeshPipeline();

        inline VkPipeline textureModelPipeline;
        void createTextureModelPipeline();
        void destroyTextureModelPipeline();

        inline VkPipeline skeletonModelPipeline;
        void createSkeletonModelPipeline();
        void destroySkeletonModelPipeline();

        inline VkPipeline skeletonModelCompPipeline;
        void createSkeletonModelCompPipeline();
        void destroySkeletonModelCompPipeline();

        inline VkPipeline shadowColorMeshPipeline[16];
        void createShadowColorMeshPipeline();
        void destroyShadowColorMeshPipeline();

        inline VkPipeline shadowColorModelArrayPipeline[16];
        void createShadowColorModelArrayPipeline();
        void destroyShadowColorModelArrayPipeline();
        
        inline VkPipeline shadowTextureModelPipeline[16];
        void createShadowTextureModelPipeline();
        void destroyShadowTextureModelPipeline();

        inline VkPipeline shadowSkeletonModelPipeline[16];
        void createShadowSkeletonModelPipeline();
        void destroyShadowSkeletonModelPipeline();
    }
}