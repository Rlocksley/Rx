#include "input.hpp"
#include "window.hpp"
#include "time.hpp"

namespace Neon
{
    namespace Input
    {

        void updateKeys()
        {
            keyA.down = (glfwGetKey(Core::pGLFWwindow, GLFW_KEY_A) == GLFW_PRESS);
            keyA.pressed = (keyA.down && keyA.lastPressed > keyPressTime);
            keyA.lastPressed = (keyA.lastPressed + Time::deltaTime) * !keyA.pressed;

            keyD.down = (glfwGetKey(Core::pGLFWwindow, GLFW_KEY_D) == GLFW_PRESS);
            keyD.pressed = (keyD.down && keyD.lastPressed > keyPressTime);
            keyD.lastPressed = (keyD.lastPressed + Time::deltaTime) * !keyD.pressed;

            keyS.down = (glfwGetKey(Core::pGLFWwindow, GLFW_KEY_S) == GLFW_PRESS);
            keyS.pressed = (keyS.down && keyS.lastPressed > keyPressTime);
            keyS.lastPressed = (keyS.lastPressed + Time::deltaTime) * !keyS.pressed;

            keyW.down = (glfwGetKey(Core::pGLFWwindow, GLFW_KEY_W) == GLFW_PRESS);
            keyW.pressed = (keyW.down && keyW.lastPressed > keyPressTime);
            keyW.lastPressed = (keyW.lastPressed + Time::deltaTime) * !keyW.pressed;

            keyX.down = (glfwGetKey(Core::pGLFWwindow, GLFW_KEY_X) == GLFW_PRESS);
            keyX.pressed = (keyX.down && keyX.lastPressed > keyPressTime);
            keyX.lastPressed = (keyX.lastPressed + Time::deltaTime) * !keyX.pressed;

            keyEsc.down = (glfwGetKey(Core::pGLFWwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS);
            keyEsc.pressed = (keyEsc.down && keyEsc.lastPressed > keyPressTime);
            keyEsc.lastPressed = (keyEsc.lastPressed + Time::deltaTime) * !keyEsc.pressed;
     
        }

        void updateButtons()
        {
            buttonLeft.down = (glfwGetMouseButton(Core::pGLFWwindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
            buttonLeft.pressed = (buttonLeft.down && buttonLeft.lastPressed > buttonPressTime);
            buttonLeft.lastPressed = (buttonLeft.lastPressed + Time::deltaTime) * buttonLeft.pressed;

            buttonRight.down = (glfwGetMouseButton(Core::pGLFWwindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
            buttonRight.pressed = (buttonRight.down && buttonRight.lastPressed > buttonPressTime);
            buttonRight.lastPressed = (buttonRight.lastPressed + Time::deltaTime) * buttonRight.pressed;

        }

        void createCursor()
        {
            glfwSetCursorPos
            (Core::pGLFWwindow,
            Core::windowWidth/2.f, Core::windowHeight);

            cursor.position = glm::vec2(Core::windowWidth/2.f, Core::windowHeight/2.f);

            cursor.deltaPosition = glm::vec2(0.f,0.f);
        }

        void updateCursor()
        {
            double posX, posY;
            glfwGetCursorPos(Core::pGLFWwindow, &posX, &posY);

            glm::vec2 lastPosition = cursor.position;

            cursor.position = 
            glm::vec2
            (static_cast<float>(posX),
            static_cast<float>(posY));

            cursor.deltaPosition =  cursor.position - lastPosition;
        }

        void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
        {
            scroll.scroll += static_cast<float>(yoffset);
            scroll.deltaScroll = static_cast<float>(yoffset);
        }

        void createScroll()
        {
            scroll.scroll = 0.f;
            scroll.deltaScroll = 0.f;

            glfwSetScrollCallback(Core::pGLFWwindow, scrollCallback);
        }

        void createInput()
        {
            createCursor();
            createScroll();
        }

        void updateInput()
        {
            updateKeys();
            updateButtons();
            updateCursor();
        }
    }
}