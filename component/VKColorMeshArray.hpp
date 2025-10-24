#pragma once

#include "Global.hpp"
#include "flecs.h"
#include "ColorMeshArray.hpp"
#include "Buffer.hpp"
namespace Rx{
    namespace Component{
        struct VkColorMeshArray{

            Core::Buffer vertexBuffer;
            Core::Buffer indexBuffer;
        };



        void colorMeshArray_component_on_add(flecs::entity e, ColorMeshArray& meshArray, VkColorMeshArray& colorMeshArray);
        void colorMeshArray_component_on_remove(flecs::entity e, VkColorMeshArray& colorMeshArray);
    }
}