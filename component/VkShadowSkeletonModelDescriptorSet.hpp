#pragma once

#include "Buffer.hpp"
#include "flecs.h"
#include "VkTransformBuffer.hpp"
#include "VkSkeletonArrayBuffer.hpp"

namespace Rx
{
    namespace Component
    {
        struct VkShadowSkeletonModelDescriptorSet
        {
            VkDescriptorSet vkDescriptorSet;

            void createDescriptorSet(Core::BufferInterface& instanceBuffer, Core::BufferInterface& skeletonArrayBuffer);
            void destroyDescriptorSet();
        };

    void VkShadowSkeletonModelDescriptorSet_component_on_add(flecs::entity e, VkTransformBuffer& transformBuffer, VkSkeletonArrayBuffer& skeletonArrayBuffer, Rx::Component::VkShadowSkeletonModelDescriptorSet& graphics);
    void VkShadowSkeletonModelDescriptorSet_component_on_remove(flecs::entity e, Rx::Component::VkShadowSkeletonModelDescriptorSet& graphics);
}
}
