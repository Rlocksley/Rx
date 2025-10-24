#pragma once
#include "Global.hpp"

namespace Rx
{
    namespace Core
    {
 
        void createPipelineLayouts();
        void destroyPipelineLayouts();

        inline VkPipelineLayout colorMeshPipelineLayout;
        void createColorMeshPipelineLayout();
        void destroyColorMeshPipelineLayout();

        inline VkPipelineLayout colorMeshArrayPipelineLayout;
        void createColorMeshArrayPipelineLayout();
        void destroyColorMeshArrayPipelineLayout();

        inline VkPipelineLayout instancedColorMeshPipelineLayout;
        void createInstancedColorMeshPipelineLayout();
        void destroyInstancedColorMeshPipelineLayout();

        inline VkPipelineLayout textureModelPipelineLayout;
        void createTextureModelPipelineLayout();
        void destroyTextureModelPipelineLayout();

        inline VkPipelineLayout skeletonModelPipelineLayout;
        void createSkeletonModelPipelineLayout();
        void destroySkeletonModelPipelineLayout();

        inline VkPipelineLayout skeletonModelCompPipelineLayout;
        void createSkeletonModelCompPipelineLayout();
        void destroySkeletonModelCompPipelineLayout();

        inline VkPipelineLayout shadowColorMeshPipelineLayout;
        void createShadowColorMeshPipelineLayout();
        void destroyShadowColorMeshPipelineLayout();

        inline VkPipelineLayout shadowColorModelArrayPipelineLayout;
        void createShadowColorModelArrayPipelineLayout();
        void destroyShadowColorModelArrayPipelineLayout();

        inline VkPipelineLayout shadowTextureModelPipelineLayout;
        void createShadowTextureModelPipelineLayout();
        void destroyShadowTextureModelPipelineLayout();
     
        inline VkPipelineLayout shadowSkeletonModelPipelineLayout;
        void createShadowSkeletonModelPipelineLayout();
        void destroyShadowSkeletonModelPipelineLayout();
    }
}