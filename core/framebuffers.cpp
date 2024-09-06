#include "framebuffers.hpp"
#include "physicalDevice.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "renderPass.hpp"

namespace Neon
{
    namespace Core
    {

        void createFramebuffers()
        {

            framebuffers.colorImage = 
            createImage
            (vkExtent2D.width, vkExtent2D.height,
            vkSurfaceFormatKHR.format,
            VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            vkSampleCountFlagBits,
            VK_IMAGE_ASPECT_COLOR_BIT);


            framebuffers.depthImage = 
            createImage
            (vkExtent2D.width, vkExtent2D.height,
            depthVkFormat,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            vkSampleCountFlagBits,
            VK_IMAGE_ASPECT_DEPTH_BIT);

            framebuffers.vkFramebuffers.resize(swapchain.vkImages.size());

            for(size_t i = 0; i < framebuffers.vkFramebuffers.size(); i++)
            {
                std::vector<VkImageView> attachments = 
                {
                    framebuffers.colorImage.vkImageView,
                    framebuffers.depthImage.vkImageView,
                    swapchain.vkImageViews[i]
                };

                VkFramebufferCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                createInfo.renderPass = vkRenderPass;
                createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
                createInfo.pAttachments = attachments.data();
                createInfo.width = vkExtent2D.width;
                createInfo.height = vkExtent2D.height;
                createInfo.layers = 1;

                NEON_CHECK_VULKAN
                (vkCreateFramebuffer
                (vkDevice,
                &createInfo,
                nullptr,
                &framebuffers.vkFramebuffers[i]),
                "createFramebuffers",
                "vkCreateFramebuffer")
            }

            #ifdef NEON_DEBUG
            NEON_LOGI("Framebuffers", "created", "")
            #endif
        }
        
        void destroyFramebuffers()
        {
            for(size_t i = 0; i < framebuffers.vkFramebuffers.size(); i++)
            {
                vkDestroyFramebuffer
                (vkDevice,
                framebuffers.vkFramebuffers[i],
                nullptr);
            }

            framebuffers.vkFramebuffers.clear();

            destroyImage(framebuffers.depthImage);
            destroyImage(framebuffers.colorImage);
        }
    }
}