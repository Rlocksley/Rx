#pragma once

#include "global.hpp"
#include "world.hpp"

namespace Neon
{
    namespace Component
    {
        struct Velocity : ECS::Component
        {
        public:
            Velocity(glm::vec3 velocity) :
            velocity(velocity){}

            Velocity operator+(Velocity& vel)
            {
                return Velocity(velocity+vel.velocity);
            }

        private:
            glm::vec3 velocity;

        public:
            void addVelocity(const glm::vec3 deltaVelocity)
            {
                velocity += deltaVelocity;
            }

            const glm::vec3& getVelocity() const 
            {
                return velocity;
            }

            void inversVelocity() 
            {
                velocity = -velocity;
            }


        };
    }
}