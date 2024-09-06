#pragma once

#include "global.hpp"

namespace Neon
{
    namespace Time
    {
        inline float startTime;
        inline float time;
        inline float deltaTime;
        inline float minDeltaTime{0.0001};
        inline float maxDeltaTime{0.03};

        void createTime();
        void updateTime();
    }
}