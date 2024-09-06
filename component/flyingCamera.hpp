#pragma once

#include "../core/global.hpp"
#include "../core/time.hpp"
#include "../core/input.hpp"
#include "../core/eye.hpp"

namespace Neon
{
    namespace Component
    {
        struct FlyingCamera
        {
        
            glm::vec3 position;
            glm::vec2 angle;
            float speed;
            float rotationSpeed;
            float fov;
            float nearClip;
            float farClip;

        public:
            void update()
            {
                angle += Input::cursor.deltaPosition * (1.f*Input::buttonRight.down) * rotationSpeed;

                glm::vec3 direction =
                //glm::vec3 
                //(sin(angle[0])*cos(angle[1]),
                //sin(angle[1]),
                //cos(angle[0])*cos(angle[1]));

                (glm::angleAxis(angle[0], glm::vec3(0.0,-1.0,0.0))*glm::angleAxis(angle[1], glm::vec3(1.0,0.0,0.0))) *
                glm::vec3(0.0,0.0,-1.0);

                glm::vec3 deltaPosition = direction * (1.f*(Input::keyW.down-Input::keyS.down));

                glm::vec3 right = glm::cross(direction, glm::vec3(0.f,1.f,0.f));
                deltaPosition += right * (1.f*(Input::keyD.down-Input::keyA.down)) ;

                if(deltaPosition.length() > 0.01)
                {
                    position += deltaPosition * (1.f/deltaPosition.length()) * speed * Time::deltaTime;
                }

                Core::updateEye(position, direction, fov, nearClip, farClip);
            }
        };
    }
}