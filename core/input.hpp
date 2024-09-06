#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Input
    {
        struct Key
        {
            bool down;
            bool pressed;
            float lastPressed;
        };

        struct Button
        {
            bool down;
            bool pressed;
            float lastPressed;
        };

        struct Cursor
        {
            glm::vec2 position;
            glm::vec2 deltaPosition;
        };

        struct Scroll
        {
            float scroll;
            float deltaScroll;
        };

        inline float keyPressTime{0.2};

        inline Key keyA{false, false, 0.f};
        inline Key keyD{false, false, 0.f};
        inline Key keyS{false, false, 0.f};
        inline Key keyW{false, false, 0.f};
        inline Key keyX{false, false, 0.f};
        inline Key keyEsc{false, false, 0.f};
        inline float buttonPressTime{0.1};

        inline Button buttonLeft{false, false, 0.f};
        inline Button buttonRight{false, false, 0.f};

        inline Cursor cursor;

        inline Scroll scroll;

        void createInput();
        void updateInput();
    }
}