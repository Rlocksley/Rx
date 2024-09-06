#pragma once


#define NEON_DEBUG

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


//C
#include <string.h>
#include <time.h>
//C++
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <queue>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <bitset>
#include <memory>
#include <thread>
#include <random>
#include <functional>
#include <typeinfo>
#include <algorithm>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/quaternion.hpp"

#define NEON_LOGI(X, Y, Z)\
{\
    printf("Info@NEON: %s :: %s :: %s\n", X, Y, Z);\
}

#define NEON_LOGE(X, Y, Z)\
{\
    printf("Error@NEON: %s :: %s :: %s\n", X, Y, Z);\
    exit(1);\
}

#define NEON_CHECK_VULKAN(VulkanExpression, Function, VulkanFunction)\
{\
    VkResult vkRes;\
    if((vkRes = VulkanExpression) != VK_SUCCESS)\
    {\
        printf("Error@NEON: %s :: %s\n", Function, VulkanFunction);\
        printf("Vulkan Error Code: %d\n", vkRes);\
        exit(1);\
    }\
}


#define NEON_ASSERT(Expression, Class, Funktion, Message)\
{\
    if(!(Expression))\
    {\
        NEON_LOGE(Class, Funktion, Message)\
    }\
}
