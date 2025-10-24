#include "Eye.hpp"
#include "Window.hpp"

namespace Rx
{
    namespace Core
    {
        
        void createEye()
        {
            eye = createBufferInterface
            (1,sizeof(Eye), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        }

        void destroyEye()
        {
            destroyBufferInterface
            (eye);
        }

        glm::mat4 createViewMatrix(const glm::vec3& cameraPosition, const glm::vec3& cameraDirection, const glm::vec3& upDirection)
        {
           glm::vec3 cameraRight = glm::normalize(glm::cross(upDirection, cameraDirection));
            glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));

            glm::mat4 viewMatrix(1.0f);
            viewMatrix[0][0] = cameraRight.x;
            viewMatrix[0][1] = cameraRight.y;
            viewMatrix[0][2] = cameraRight.z;
            viewMatrix[1][0] = cameraUp.x;
            viewMatrix[1][1] = cameraUp.y;
            viewMatrix[1][2] = cameraUp.z;
            viewMatrix[2][0] = -cameraDirection.x;
            viewMatrix[2][1] = -cameraDirection.y;
            viewMatrix[2][2] = -cameraDirection.z;
            viewMatrix[3][0] = cameraPosition.x;
            viewMatrix[3][1] = cameraPosition.y;
            viewMatrix[3][2] = cameraPosition.z;

            viewMatrix = glm::inverse(viewMatrix);

            return viewMatrix;
        }

        glm::mat4 createXMatrix()
        {
            glm::mat4 X = glm::mat4(1.f);
            X[1][1] = -1.f;
            X[2][2] = -1.f;
            return glm::inverse(X);
        }

        glm::mat4 createProjectionMatrix(float fov, float aspectRatio, float nearClip, float farClip) 
        {
            glm::mat4 projectionMatrix(1.0f);

            float tanHalfFOV = tan(fov / 2.0f);
            float depthRangeInv = 1.0f / (farClip - nearClip);

            projectionMatrix[0][0] = 1.0f / (aspectRatio * tanHalfFOV);
            projectionMatrix[0][1] = 0.0f;
            projectionMatrix[0][2] = 0.0f;
            projectionMatrix[0][3] = 0.0f;
            projectionMatrix[1][0] = 0.0f;
            projectionMatrix[1][1] = 1.0f / tanHalfFOV;
            projectionMatrix[1][2] = 0.0f;
            projectionMatrix[1][3] = 0.0f;

            projectionMatrix[2][0] = 0.0f;
            projectionMatrix[2][1] = 0.0f;
            projectionMatrix[2][2] = farClip * depthRangeInv; // Vulkan clip depth [0, 1]
            projectionMatrix[2][3] = 1.0f;

            projectionMatrix[3][0] = 0.0f;
            projectionMatrix[3][1] = 0.0f;
            projectionMatrix[3][2] = (-farClip * nearClip) * depthRangeInv;
            projectionMatrix[3][3] = 0.0f;

            return projectionMatrix;
        }

        void updateEye(glm::vec3 position, glm::vec3 direction, float fov, float nearClip, float farClip)
        {
            Eye _eye;
            _eye.position = glm::vec4(position, 1.0f);
            _eye.direction = glm::vec4(direction, 0.0f);
            _eye.projView = createProjectionMatrix(fov,static_cast<float>(windowWidth)/static_cast<float>(windowHeight),nearClip, farClip);
            _eye.projView *= createXMatrix();
            _eye.projView *= createViewMatrix(position, direction, glm::vec3(0.f,-1.f,0.f));
            memcpy(eye.pMemory, &_eye, sizeof(Eye));
        }

        std::pair<glm::mat4, glm::mat4> getEyeMatrices(glm::vec3 position, glm::vec3 direction, glm::vec3 up,  float fov, float nearClip, float farClip){
            glm::mat4 proj = createProjectionMatrix(fov, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), nearClip, farClip) * createXMatrix();
            glm::mat4 view = createViewMatrix(position, direction, -up);
            return { proj, view };
        }

        std::pair<glm::mat4, glm::mat4> getEyeMatrices(glm::vec3 position, glm::vec3 direction, glm::vec3 up,  float fov, float aspectRatio, float nearClip, float farClip){
            glm::mat4 proj = createProjectionMatrix(fov, aspectRatio, nearClip, farClip) * createXMatrix();
            glm::mat4 view = createViewMatrix(position, direction, -up);
            return { proj, view };
        }
    }
}