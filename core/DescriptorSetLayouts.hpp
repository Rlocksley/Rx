#pragma once

#include "Global.hpp"

namespace Rx
{
    namespace Core
    {

        
        void createDescriptorSetLayouts();
        void destroyDescriptorSetLayouts();



        inline VkDescriptorSetLayout colorMeshDescriptorSetLayout;
        void createColorMeshDescriptorSetLayout();
        void destroyColorMeshDescriptorSetLayout();
        
        inline VkDescriptorSetLayout colorMeshArrayDescriptorSetLayout;
        void createColorMeshArrayDescriptorSetLayout();
        void destroyColorMeshArrayDescriptorSetLayout();

        inline VkDescriptorSetLayout instancedColorMeshDescriptorSetLayout;
        void createInstancedColorMeshDescriptorSetLayout();
        void destroyInstancedColorMeshDescriptorSetLayout();

        inline VkDescriptorSetLayout textureModelDescriptorSetLayout;
        void createTextureModelDescriptorSetLayout();
        void destroyTextureModelDescriptorSetLayout();
        
        inline VkDescriptorSetLayout skeletonModelDescriptorSetLayout;
        void createSkeletonModelDescriptorSetLayout();
        void destroySkeletonModelDescriptorSetLayout();
        
        inline VkDescriptorSetLayout skeletonModelCompDescriptorSetLayout;
        void createSkeletonModelCompDescriptorSetLayout();
        void destroySkeletonModelCompDescriptorSetLayout();
        
        inline VkDescriptorSetLayout shadowColorMeshDescriptorSetLayout;
        void createShadowColorMeshDescriptorSetLayout();
        void destroyShadowColorMeshDescriptorSetLayout();

        inline VkDescriptorSetLayout shadowColorModelArrayDescriptorSetLayout;
        void createShadowColorModelArrayDescriptorSetLayout();
        void destroyShadowColorModelArrayDescriptorSetLayout();

        inline VkDescriptorSetLayout shadowTextureModelDescriptorSetLayout;
        void createShadowTextureModelDescriptorSetLayout();
        void destroyShadowTextureModelDescriptorSetLayout();

        inline VkDescriptorSetLayout shadowSkeletonModelDescriptorSetLayout;
        void createShadowSkeletonModelDescriptorSetLayout();
        void destroyShadowSkeletonModelDescriptorSetLayout();

    }
}
