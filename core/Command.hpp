#pragma once

#include "Global.hpp"

namespace Rx
{
    namespace Core
    {
        struct Command
        {
            VkCommandPool vkCommandPool;
            VkCommandBuffer vkCommandBuffer;
            VkFence vkFence;
        };

        struct SingleCommand
        {
            VkCommandPool vkCommandPool;
            VkCommandBuffer vkCommandBuffer;
            VkFence vkFence;
        };

        inline Rx::Core::SingleCommand singleCommand;
        inline Rx::Core::Command command;

        Command createCommand();
        void destroyCommand(Command command);

        void beginCommand(Command command);
        void endCommand(Command command);
        void waitForCommand(Command command);

        SingleCommand createSingleCommand();
        void destroySingleCommand(SingleCommand singleCommand);

        void beginSingleCommand(SingleCommand singleCommand);
        void endSingleCommand(SingleCommand singleCommand);
 
   }
}