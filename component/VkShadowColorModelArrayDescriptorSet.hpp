#pragma once

#include "Buffer.hpp"
#include "flecs.h"
#include "VkInstancedColorModelBuffer.hpp"

namespace Rx
{
    namespace Component
    {
        struct VkShadowColorModelArrayDescriptorSet
        {
            VkDescriptorSet vkDescriptorSet; // Descriptor set for Vulkan

            void createDescriptorSet(Core::BufferInterface& instanceBuffer);
            void destroyDescriptorSet();
        };

    void VkShadowColorModelArrayDescriptorSet_component_on_add(flecs::entity e, Rx::Component::VkInstancedColorModelBuffer& colorArray, Rx::Component::VkShadowColorModelArrayDescriptorSet& graphics);
    void VkShadowColorModelArrayDescriptorSet_component_on_remove(flecs::entity e, Rx::Component::VkShadowColorModelArrayDescriptorSet& graphics);
}
}
