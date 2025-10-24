#pragma once

#include "Buffer.hpp"
#include "flecs.h"
#include "VkTransformBuffer.hpp"

namespace Rx
{
    namespace Component
    {
        struct VkShadowTextureModelDescriptorSet
        {
            VkDescriptorSet vkDescriptorSet;

            void createDescriptorSet(Core::BufferInterface& instanceBuffer);
            void destroyDescriptorSet();
        };

    void VkShadowTextureModelDescriptorSet_component_on_add(flecs::entity e, Rx::Component::VkTransformBuffer& transformBuffer, Rx::Component::VkShadowTextureModelDescriptorSet& graphics);
    void VkShadowTextureModelDescriptorSet_component_on_remove(flecs::entity e, Rx::Component::VkShadowTextureModelDescriptorSet& graphics);
}
}
