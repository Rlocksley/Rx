#pragma once

#include "Global.hpp"
#include "flecs.h"
#include "VkColorModelBuffer.hpp"

namespace Rx
{
    namespace Component
    {
        struct VkShadowColorModelDescriptorSet
        {
        
        protected:
            VkDescriptorSet vkDescriptorSet;
           
        public:
            void createDescriptorSet(Rx::Component::VkColorModelBuffer& buffer);
            void destroyDescriptorSet();
            
        public:
             const VkDescriptorSet& getDescriptorSet()
            {
                return vkDescriptorSet;
            }            
        };
    }
}




void shadowColorModelDescriptorSet_component_on_add(flecs::entity e,Rx::Component::VkColorModelBuffer& modelBuffer, Rx::Component::VkShadowColorModelDescriptorSet& graphics);

void shadowColorModelDescriptorSet_component_on_remove(flecs::entity e, Rx::Component::VkShadowColorModelDescriptorSet& graphics);


