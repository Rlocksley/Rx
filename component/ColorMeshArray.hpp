#pragma once
#include "Global.hpp"
#include "Vertex.hpp"
namespace Rx
{

    namespace Component
    {
        struct ColorMeshArray
        {
            int firstIndex{0};
            int vertexOffset{0};
            std::vector<std::vector<Vertex::Color>> vertexArrays; 
            std::vector<std::vector<uint32_t>> indexArrays; 
            std::unordered_map<std::string, VkDrawIndexedIndirectCommand> meshNameToCommand;
            void addMesh(const std::string& name, const std::vector<Vertex::Color>& vertices, const std::vector<uint32_t>& indices);
        };
    }
}