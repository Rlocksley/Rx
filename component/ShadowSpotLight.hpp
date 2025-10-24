#pragma once

#include "Global.hpp"

namespace Rx{
    namespace Component{

        struct ShadowSpotLight {
            glm::vec3 color{1.0f, 1.0f, 1.0f};
            float intensity{1.0f};
            float fov{3.14f/4.f};        // field of view in radians
            float nearPlane{0.1f};
            float farPlane{100.0f};
            float aspectRatio{1.0f}; // typically 1.0 for square shadow maps
        };
    }
}