#include "renderPass.hpp"
#include "physicalDevice.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "framebuffers.hpp"

namespace Neon
{
    namespace Core
    {
        void createRenderPass()
        {
            VkAttachmentDescription colorDescr{};
            colorDescr.format = vkSurfaceFormatKHR.format;
            colorDescr.samples = vkSampleCountFlagBits;
            colorDescr.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorDescr.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorDescr.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorDescr.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorDescr.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorDescr.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkAttachmentReference colorRef{};
            colorRef.attachment = 0;
            colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


            VkAttachmentDescription depthDescr{};
            depthDescr.format = depthVkFormat;
            depthDescr.samples = vkSampleCountFlagBits;
            depthDescr.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depthDescr.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthDescr.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depthDescr.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthDescr.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depthDescr.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentReference depthRef{};
            depthRef.attachment = 1;
            depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription colorAttachmentResolve{};
            colorAttachmentResolve.format = vkSurfaceFormatKHR.format;
            colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference colorAttachmentResolveRef{};
            colorAttachmentResolveRef.attachment = 2;
            colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpassDescr{};
            subpassDescr.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpassDescr.inputAttachmentCount = 0;
            subpassDescr.pInputAttachments = nullptr;
            subpassDescr.colorAttachmentCount = 1;
            subpassDescr.pColorAttachments = &colorRef;
            subpassDescr.pResolveAttachments = &colorAttachmentResolveRef;
            subpassDescr.pDepthStencilAttachment = &depthRef;
            subpassDescr.preserveAttachmentCount = 0;
            subpassDescr.pPreserveAttachments = nullptr;

            VkSubpassDependency subpassDep{};
            subpassDep.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpassDep.dstSubpass = 0;
            subpassDep.srcStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            subpassDep.dstStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            subpassDep.srcAccessMask = 0;
            subpassDep.dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            subpassDep.dependencyFlags = 0;

            std::vector<VkAttachmentDescription> descr = 
            {
                colorDescr,
                depthDescr,
                colorAttachmentResolve
            };

            VkRenderPassCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            createInfo.attachmentCount = static_cast<uint32_t>(descr.size());
            createInfo.pAttachments = descr.data();
            createInfo.subpassCount = 1;
            createInfo.pSubpasses = &subpassDescr;
            createInfo.dependencyCount = 1;
            createInfo.pDependencies = &subpassDep;

            NEON_CHECK_VULKAN
            (vkCreateRenderPass
            (vkDevice,
            &createInfo,
            NULL,
            &vkRenderPass),
            "createRenderPass",
            "vkCreateRenderPass")

            #ifdef NEON_DEBUG
                NEON_LOGI("RenderPass", "created", "")
            #endif
        }

        void destroyRenderPass()
        {
            vkDestroyRenderPass
            (vkDevice,
            vkRenderPass,
            nullptr);
        }


        void beginRenderPass(Command command)
        {
            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
            clearValues[1].depthStencil = {1.0f, 0};
            
            VkRenderPassBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            beginInfo.renderPass = vkRenderPass;
            beginInfo.framebuffer = framebuffers.vkFramebuffers[swapchain.imageIndex];
            beginInfo.renderArea.offset.x = 0;
            beginInfo.renderArea.offset.y = 0;
            beginInfo.renderArea.extent = vkExtent2D;
            beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            beginInfo.pClearValues = clearValues.data();
        
            vkCmdBeginRenderPass
            (command.vkCommandBuffer,
            &beginInfo,
            VK_SUBPASS_CONTENTS_INLINE);
        }

        void endRenderPass(Command command)
        {
            vkCmdEndRenderPass
            (command.vkCommandBuffer);
        }
    }
}