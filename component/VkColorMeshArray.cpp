#include "VkColorMeshArray.hpp"

#include "Buffer.hpp"
#include "Mutex.hpp"
#include "Command.hpp"
#include "Queue.hpp"

namespace Rx{
    namespace Component{
        void colorMeshArray_component_on_add(flecs::entity e, ColorMeshArray& meshArray, VkColorMeshArray& colorMeshArray)
        {
             // Calculate total sizes for collection buffers
            size_t totalVertices = 0;
            size_t totalIndices = 0;
            
            for (const auto& vertexArray : meshArray.vertexArrays)
            {
                totalVertices += vertexArray.size();
            }
            
            for (const auto& indexArray : meshArray.indexArrays)
            {
                totalIndices += indexArray.size();
            }
            
            RX_ASSERT(totalVertices > 0, "ColorMeshArray", "colorMeshArray_component_on_add", "Total vertices must be greater than 0");
            RX_ASSERT(totalIndices > 0, "ColorMeshArray", "colorMeshArray_component_on_add", "Total indices must be greater than 0");
            
            // Create staging buffers for vertex and index data
            Rx::Core::BufferInterface vertexInterface = 
                Rx::Core::createBufferInterface(totalVertices, sizeof(Rx::Vertex::Color),
                    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
                    
            Rx::Core::BufferInterface indexInterface = 
                Rx::Core::createBufferInterface(totalIndices, sizeof(uint32_t),
                    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            
            // Create GPU buffers for the collection
            colorMeshArray.vertexBuffer = Rx::Core::createBuffer(totalVertices, sizeof(Rx::Vertex::Color),
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
                
            colorMeshArray.indexBuffer = Rx::Core::createBuffer(totalIndices, sizeof(uint32_t),
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            
            // Copy vertex data to staging buffer
            size_t vertexOffset = 0;
            char* vertexDst = static_cast<char*>(vertexInterface.pMemory);
            
            for (const auto& vertexArray : meshArray.vertexArrays)
            {
                size_t arraySize = vertexArray.size() * sizeof(Rx::Vertex::Color);
                memcpy(vertexDst + vertexOffset, vertexArray.data(), arraySize);
                vertexOffset += arraySize;
            }
            
            // Copy index data to staging buffer
            size_t indexOffset = 0;
            char* indexDst = static_cast<char*>(indexInterface.pMemory);
            
            for (const auto& indexArray : meshArray.indexArrays)
            {
                size_t arraySize = indexArray.size() * sizeof(uint32_t);
                memcpy(indexDst + indexOffset, indexArray.data(), arraySize);
                indexOffset += arraySize;
            }
            
            // Copy from staging buffers to GPU buffers
            VkBufferCopy vertexCopy;
            vertexCopy.srcOffset = 0;
            vertexCopy.dstOffset = 0;
            vertexCopy.size = totalVertices * sizeof(Rx::Vertex::Color);
            
            VkBufferCopy indexCopy;
            indexCopy.srcOffset = 0;
            indexCopy.dstOffset = 0;
            indexCopy.size = totalIndices * sizeof(uint32_t);
            
            // Execute the copy commands
            RX_VK_MUTEX(
                Rx::Core::beginSingleCommand(Rx::Core::singleCommand);
                vkCmdCopyBuffer(Rx::Core::singleCommand.vkCommandBuffer,
                    vertexInterface.vkBuffer,
                    colorMeshArray.vertexBuffer.vkBuffer,
                    1, &vertexCopy);
                vkCmdCopyBuffer(Rx::Core::singleCommand.vkCommandBuffer,
                    indexInterface.vkBuffer,
                    colorMeshArray.indexBuffer.vkBuffer,
                    1, &indexCopy);
                Rx::Core::endSingleCommand(Rx::Core::singleCommand);
                Rx::Core::submitSingleCommand(Rx::Core::singleCommand);
            )
              
            // Clean up staging buffers
            Rx::Core::destroyBufferInterface(vertexInterface);
            Rx::Core::destroyBufferInterface(indexInterface);
        }

        void colorMeshArray_component_on_remove(flecs::entity e, VkColorMeshArray& colorMeshArray)
        {
            // Cleanup GPU resources
            Rx::Core::destroyBuffer(colorMeshArray.vertexBuffer);
            Rx::Core::destroyBuffer(colorMeshArray.indexBuffer);
        }
    }
}