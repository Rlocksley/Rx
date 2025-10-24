#include "Core.hpp"
#include "Mutex.hpp"
#include "Image.hpp"

namespace Rx
{
namespace Core
{
    void createCore()
    {
        createWindow();
        Time::createTime();
        Input::createInput();

        createInstance();
        createSurface();
        pickPhysicalDevice();
        createDevice();
        getQueue();
        createAllocator();
        createDescriptorPool();


        Core::command = createCommand();

        Core::singleCommand = createSingleCommand();

        createEye();

        createSwapchain();
        createRenderPass();
        createFramebuffers();

        createLightBuffers();
        createShadowMapArray(10048, 10048, 16, VK_FORMAT_D32_SFLOAT);

        createDescriptorSetLayouts();
        createPipelineLayouts();
        createPipelines();
    }

    void destroyCore()
    {
        destroyPipelines();
        destroyPipelineLayouts();
        destroyDescriptorSetLayouts();

        destroyShadowMapArray();
        destroyLightBuffers();
        
        destroyFramebuffers();
        destroyRenderPass();
        destroySwapchain();

        destroyEye();

        destroySingleCommand(Core::singleCommand);   
        destroyCommand(Core::command);

        
        destroyDescriptorPool();
        destroyAllocator();
        destroyDevice();
        destroySurface();
        destroyInstance();

        destroyWindow();
    }

    bool updateCore()
    {
        Rx::Core::waitForCommand(Rx::Core::command);

        glfwPollEvents();
        
        Time::updateTime();
        Input::updateInput();

        return !glfwWindowShouldClose(pGLFWwindow);
    }

}
}