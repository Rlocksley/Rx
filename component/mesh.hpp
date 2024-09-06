#pragma once
#include <vector>
#include <glm/glm.hpp>
namespace Neon
{
    namespace Vertex
    {
        struct Color
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec4 color;
        };
    }


namespace Component{

struct Mesh{
    std::vector<Vertex::Color> vertices;
    std::vector<uint32_t> indices;
};

}


}