#pragma once

#include "../core/global.hpp"
#include "../core/queue.hpp"
#include "../core/command.hpp"
#include "../core/buffer.hpp"
#include "mesh.hpp"

namespace Neon
{

    namespace Component
    {
        struct ColorMesh
        {
        public:
     
            Core::Buffer vertexBuffer;
            Core::Buffer indexBuffer;

        public:
            const VkBuffer& getVertexBuffer()
            {
                return vertexBuffer.vkBuffer;
            }

            const VkBuffer& getIndexBuffer()
            {
                return indexBuffer.vkBuffer;
            }

            const uint32_t& getNumberIndices()
            {
                return indexBuffer.numberElements;    
            }
        };

    }
}