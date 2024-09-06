#pragma once

#include "../core/global.hpp"
#include "../core/buffer.hpp"
#include "../core/shader.hpp"
#include "colorGraphics.hpp"
#include "../core/physicalDevice.hpp"
#include "../core/device.hpp"
#include "../core/descriptorPool.hpp"
#include "../core/renderPass.hpp"
#include "../core/eye.hpp"
#include "colorMesh.hpp"
namespace Neon
{
    namespace ModelBuffer
    {
        struct Color
        {
            glm::mat4 transform;
        };
    };

    namespace Component
    {
        struct ColorGraphics
        {
        
        
        protected:
            bool destroyed{false};

        protected:
            Core::BufferInterface modelBuffer;
            VkDescriptorSetLayout vkDescriptorSetLayout; 
            VkDescriptorSet vkDescriptorSet;
            VkPipelineLayout vkPipelineLayout;
            VkPipeline vkPipeline;

        public:
            void createModelBuffer();
            void destroyModelBuffer();
            void createDescriptorSet();
            void destroyDescriptorSet();
            void createGraphicsPipeline
            (Core::Shader& vertexShader, 
            Core::Shader& fragmentShader);
            void destroyGraphicsPipeline();

        public:
            const VkDescriptorSet& getDescriptorSet()
            {
                return vkDescriptorSet;
            }

            const VkPipelineLayout& getPipelineLayout()
            {
                return vkPipelineLayout;
            }

            const VkPipeline& getGraphicsPipeline()
            {
                return vkPipeline;
            }

            void setModelBuffer(glm::mat4 transform)
            {
                ModelBuffer::Color buffer;
                buffer.transform = transform;

                memcpy(modelBuffer.pMemory, &buffer, sizeof(ModelBuffer::Color));
            }
        };
    }
}



namespace Neon
{
    namespace Component
    {
        

        void ColorGraphics::createModelBuffer()
        {
            modelBuffer = 
            Core::createBufferInterface
            (1, sizeof(ModelBuffer::Color),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        }

        void ColorGraphics::destroyModelBuffer()
        {
            Core::destroyBufferInterface(modelBuffer);
        }

        void ColorGraphics::createDescriptorSet()
        {

            std::vector<VkDescriptorSetLayoutBinding> bindings(2);
            bindings[0].binding = 0;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[0].pImmutableSamplers = nullptr;
            bindings[1].binding = 1;
            bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[1].descriptorCount = 1;
            bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            bindings[1].pImmutableSamplers = nullptr;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            NEON_CHECK_VULKAN
            (vkCreateDescriptorSetLayout
            (Core::vkDevice,
            &layoutInfo,
            nullptr,
            &vkDescriptorSetLayout),
            "ColorGraphics::createDescriptorSet",
            "vkCreateDescriptorSetLayout")

            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = Core::vkDescriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &vkDescriptorSetLayout;
            
            NEON_CHECK_VULKAN
            (vkAllocateDescriptorSets
            (Core::vkDevice,
            &allocInfo,
            &vkDescriptorSet),
            "ColorGraphics::createDescriptorSet",
            "vkAllocateDescriptorSets")

            std::vector<VkDescriptorBufferInfo> bufferInfo(2);
            bufferInfo[0].buffer = Core::eye.vkBuffer;
            bufferInfo[0].offset = 0;
            bufferInfo[0].range = sizeof(Core::Eye);
            bufferInfo[1].buffer = modelBuffer.vkBuffer;
            bufferInfo[1].offset = 0;
            bufferInfo[1].range = sizeof(ModelBuffer::Color);
            
            std::vector<VkWriteDescriptorSet> writeSet(2);
            writeSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[0].dstSet = vkDescriptorSet;
            writeSet[0].dstBinding = 0;
            writeSet[0].dstArrayElement = 0;
            writeSet[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeSet[0].descriptorCount = 1;
            writeSet[0].pBufferInfo = &bufferInfo[0];
            writeSet[0].pImageInfo = nullptr;
            writeSet[0].pTexelBufferView = nullptr;
            writeSet[0].pNext = nullptr;
            writeSet[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeSet[1].dstSet = vkDescriptorSet;
            writeSet[1].dstBinding = 1;
            writeSet[1].dstArrayElement = 0;
            writeSet[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeSet[1].descriptorCount = 1;
            writeSet[1].pBufferInfo = &bufferInfo[1];
            writeSet[1].pImageInfo = nullptr;
            writeSet[1].pTexelBufferView = nullptr;
            writeSet[1].pNext = nullptr;
            
            vkUpdateDescriptorSets
            (Core::vkDevice, 
            static_cast<uint32_t>(writeSet.size()),
            writeSet.data(),
            0, nullptr);
        }

        void ColorGraphics::destroyDescriptorSet()
        {
            vkFreeDescriptorSets
            (Core::vkDevice,
            Core::vkDescriptorPool,
            1, &vkDescriptorSet);

            vkDestroyDescriptorSetLayout
            (Core::vkDevice,
            vkDescriptorSetLayout,
            nullptr);
        }

        void ColorGraphics::createGraphicsPipeline(Core::Shader& vertexShader, Core::Shader& fragmentShader)
        {

            std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos(2);
            stageCreateInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[0].flags = 0;
            stageCreateInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
            stageCreateInfos[0].module = vertexShader.createShaderModule();
            stageCreateInfos[0].pName = "main";
            stageCreateInfos[0].pSpecializationInfo = nullptr;
            stageCreateInfos[0].pNext = nullptr;
            stageCreateInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[1].flags = 0;
            stageCreateInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            stageCreateInfos[1].module = fragmentShader.createShaderModule();
            stageCreateInfos[1].pName = "main";
            stageCreateInfos[1].pSpecializationInfo = nullptr;
            stageCreateInfos[1].pNext = nullptr;

            VkPipelineLayoutCreateInfo layoutCreateInfo{};
            layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            layoutCreateInfo.setLayoutCount = 1;
            layoutCreateInfo.pSetLayouts = &vkDescriptorSetLayout;
            layoutCreateInfo.pushConstantRangeCount = 0;
            layoutCreateInfo.pPushConstantRanges = nullptr;

            NEON_CHECK_VULKAN
            (vkCreatePipelineLayout
            (Core::vkDevice,
            &layoutCreateInfo,
            nullptr,
            &vkPipelineLayout),
            "createGraphicsPipeline",
            "vkCreatePipelineLayout")

            VkVertexInputBindingDescription vertexBindingDescr{};
            vertexBindingDescr.binding = 0;
            vertexBindingDescr.stride = sizeof(Vertex::Color);
            vertexBindingDescr.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            std::vector<VkVertexInputAttributeDescription> vertexAttributeDescr(3);
            vertexAttributeDescr[0].binding = 0;
            vertexAttributeDescr[0].location = 0;
            vertexAttributeDescr[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            vertexAttributeDescr[0].offset = offsetof(Vertex::Color,position);
            vertexAttributeDescr[1].binding = 0;
            vertexAttributeDescr[1].location = 1;
            vertexAttributeDescr[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            vertexAttributeDescr[1].offset = offsetof(Vertex::Color,normal);
            vertexAttributeDescr[2].binding = 0;
            vertexAttributeDescr[2].location = 2;
            vertexAttributeDescr[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            vertexAttributeDescr[2].offset = offsetof(Vertex::Color,color);
            
            VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
            vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
            vertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexBindingDescr;
            vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDescr.size());
            vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexAttributeDescr.data();
            
            VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
            inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport{};
            viewport.x = 0.f;
            viewport.y = 0.f;
            viewport.width = static_cast<float>(Core::vkExtent2D.width);
            viewport.height = static_cast<float>(Core::vkExtent2D.height);
            viewport.minDepth = 0.f;
            viewport.maxDepth = 1.f;

            VkRect2D rect{};
            rect.offset.x = 0;
            rect.offset.y = 0;
            rect.extent = Core::vkExtent2D;

            VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
            viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportStateCreateInfo.viewportCount = 1;
            viewportStateCreateInfo.pViewports = &viewport;
            viewportStateCreateInfo.scissorCount = 1;
            viewportStateCreateInfo.pScissors = &rect;

            VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
            rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizationStateCreateInfo.depthBiasClamp = VK_FALSE;
            rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
            rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
            rasterizationStateCreateInfo.lineWidth = 1.f;
            rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
            rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
            rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
            rasterizationStateCreateInfo.depthBiasConstantFactor = 0.f;
            rasterizationStateCreateInfo.depthBiasClamp = 0.f;
            rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.f;

            VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
            multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
            multisampleStateCreateInfo.rasterizationSamples = Core::vkSampleCountFlagBits;
            multisampleStateCreateInfo.minSampleShading = 1.f;
            multisampleStateCreateInfo.pSampleMask = nullptr;
            multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
            multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

            VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo{};
            depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
            depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
            depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
            depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
            depthStencilStateCreateInfo.minDepthBounds = 0.f;
            depthStencilStateCreateInfo.maxDepthBounds = 1.f;
            depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;

            VkPipelineColorBlendAttachmentState colorBlendAttachmentState{};
            colorBlendAttachmentState.colorWriteMask = 
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            colorBlendAttachmentState.blendEnable = VK_FALSE;
            colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
            colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
            colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
            colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

            VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{};
            colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
            colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
            colorBlendStateCreateInfo.attachmentCount = 1;
            colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
            colorBlendStateCreateInfo.blendConstants[0] = 0.f;
            colorBlendStateCreateInfo.blendConstants[1] = 0.f;
            colorBlendStateCreateInfo.blendConstants[2] = 0.f;
            colorBlendStateCreateInfo.blendConstants[3] = 0.f;

            VkGraphicsPipelineCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            createInfo.stageCount = 2;
            createInfo.pStages = stageCreateInfos.data();
            createInfo.pVertexInputState = &vertexInputStateCreateInfo;
            createInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
            createInfo.pTessellationState = nullptr;
            createInfo.pViewportState = &viewportStateCreateInfo;
            createInfo.pRasterizationState = &rasterizationStateCreateInfo;
            createInfo.pMultisampleState = &multisampleStateCreateInfo;
            createInfo.pDepthStencilState = &depthStencilStateCreateInfo;
            createInfo.pColorBlendState = &colorBlendStateCreateInfo;
            createInfo.pDynamicState = nullptr;
            createInfo.layout = vkPipelineLayout;
            createInfo.renderPass = Core::vkRenderPass;
            createInfo.subpass = 0;
            createInfo.basePipelineHandle = VK_NULL_HANDLE;
            createInfo.basePipelineIndex = -1;
            
            NEON_CHECK_VULKAN
            (vkCreateGraphicsPipelines
            (Core::vkDevice,
            VK_NULL_HANDLE,
            1,
            &createInfo,
            nullptr,
            &vkPipeline),
            "ColorGraphics::createPipeline",
            "vkCreateGraphicsPipelines")

            for(size_t i = 0; i < stageCreateInfos.size(); i++)
            {
                vkDestroyShaderModule
                (Core::vkDevice,
                stageCreateInfos[i].module,
                nullptr);
            }
        }

        void ColorGraphics::destroyGraphicsPipeline()
        {
            vkDestroyPipeline
            (Core::vkDevice,
            vkPipeline,
            nullptr);

            vkDestroyPipelineLayout
            (Core::vkDevice,
            vkPipelineLayout,
            nullptr);
        }
    }
}