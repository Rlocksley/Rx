#include "Pipelines.hpp"
#include "PipelineLayouts.hpp"
#include "Mutex.hpp"
#include "Device.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "PhysicalDevice.hpp"
#include "RenderPass.hpp"
#include "Image.hpp"

namespace Rx
{
    namespace Core
    {
        void createPipelines()
        {
            createColorMeshPipeline();
            createColorMeshArrayPipeline();
            //createInstancedColorMeshPipeline();
            createTextureModelPipeline();
            createSkeletonModelPipeline();
            createSkeletonModelCompPipeline();
            createShadowColorMeshPipeline();
            createShadowColorModelArrayPipeline();
            createShadowTextureModelPipeline();
            createShadowSkeletonModelPipeline();
        }

        void destroyPipelines()
        {
            destroyShadowSkeletonModelPipeline();
            destroyShadowTextureModelPipeline();
            destroyShadowColorModelArrayPipeline();
            destroyShadowColorMeshPipeline();
            destroySkeletonModelCompPipeline();
            destroySkeletonModelPipeline();
            destroyTextureModelPipeline();
            //destroyInstancedColorMeshPipeline();
            destroyColorMeshArrayPipeline();
            destroyColorMeshPipeline();
        }

        void createColorMeshArrayPipeline(){
            std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos(2);
            stageCreateInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[0].flags = 0;
            stageCreateInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
            stageCreateInfos[0].module = colorMeshArrayVertexShader.createShaderModule();
            stageCreateInfos[0].pName = "main";
            stageCreateInfos[0].pSpecializationInfo = nullptr;
            stageCreateInfos[0].pNext = nullptr;
            stageCreateInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[1].flags = 0;
            stageCreateInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            stageCreateInfos[1].module = colorMeshArrayFragmentShader.createShaderModule();
            stageCreateInfos[1].pName = "main";
            stageCreateInfos[1].pSpecializationInfo = nullptr;
            stageCreateInfos[1].pNext = nullptr;


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
            createInfo.layout = colorMeshArrayPipelineLayout;
            createInfo.renderPass = Core::vkRenderPass;
            createInfo.subpass = 0;
            createInfo.basePipelineHandle = VK_NULL_HANDLE;
            createInfo.basePipelineIndex = -1;
            
            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateGraphicsPipelines
            (Core::vkDevice,
            VK_NULL_HANDLE,
            1,
            &createInfo,
            nullptr,
            &colorMeshArrayPipeline),
            "createColorMeshArrayPipeline",
            "vkCreateGraphicsPipelines"))

            RX_VK_MUTEX(
            for(size_t i = 0; i < stageCreateInfos.size(); i++)
            {
                vkDestroyShaderModule
                (Core::vkDevice,
                stageCreateInfos[i].module,
                nullptr);
            })
        }

        void destroyColorMeshArrayPipeline(){
            RX_VK_MUTEX(
                vkDestroyPipeline(vkDevice, colorMeshArrayPipeline, nullptr);
            )
        }

        
        void createColorMeshPipeline(){
            std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos(2);
            stageCreateInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[0].flags = 0;
            stageCreateInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
            stageCreateInfos[0].module = colorMeshVertexShader.createShaderModule();
            stageCreateInfos[0].pName = "main";
            stageCreateInfos[0].pSpecializationInfo = nullptr;
            stageCreateInfos[0].pNext = nullptr;
            stageCreateInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[1].flags = 0;
            stageCreateInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            stageCreateInfos[1].module = colorMeshFragmentShader.createShaderModule();
            stageCreateInfos[1].pName = "main";
            stageCreateInfos[1].pSpecializationInfo = nullptr;
            stageCreateInfos[1].pNext = nullptr;


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
            createInfo.layout = colorMeshPipelineLayout;
            createInfo.renderPass = Core::vkRenderPass;
            createInfo.subpass = 0;
            createInfo.basePipelineHandle = VK_NULL_HANDLE;
            createInfo.basePipelineIndex = -1;
            
            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateGraphicsPipelines
            (Core::vkDevice,
            VK_NULL_HANDLE,
            1,
            &createInfo,
            nullptr,
            &colorMeshPipeline),
            "VkColorModelDescriptorSet::createPipeline",
            "vkCreateGraphicsPipelines"))

            RX_VK_MUTEX(
            for(size_t i = 0; i < stageCreateInfos.size(); i++)
            {
                vkDestroyShaderModule
                (Core::vkDevice,
                stageCreateInfos[i].module,
                nullptr);
            })
        }

        void destroyColorMeshPipeline(){
            RX_VK_MUTEX(
                vkDestroyPipeline(vkDevice, colorMeshPipeline, nullptr);
            )
        }


        void createInstancedColorMeshPipeline(){
            std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos(2);
            stageCreateInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[0].flags = 0;
            stageCreateInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
            stageCreateInfos[0].module = instancedColorMeshVertexShader.createShaderModule();
            stageCreateInfos[0].pName = "main";
            stageCreateInfos[0].pSpecializationInfo = nullptr;
            stageCreateInfos[0].pNext = nullptr;
            stageCreateInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[1].flags = 0;
            stageCreateInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            stageCreateInfos[1].module = instancedColorMeshFragmentShader.createShaderModule();
            stageCreateInfos[1].pName = "main";
            stageCreateInfos[1].pSpecializationInfo = nullptr;
            stageCreateInfos[1].pNext = nullptr;


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
            createInfo.layout = instancedColorMeshPipelineLayout;
            createInfo.renderPass = Core::vkRenderPass;
            createInfo.subpass = 0;
            createInfo.basePipelineHandle = VK_NULL_HANDLE;
            createInfo.basePipelineIndex = -1;
            
            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateGraphicsPipelines
            (Core::vkDevice,
            VK_NULL_HANDLE,
            1,
            &createInfo,
            nullptr,
            &instancedColorMeshPipeline),
            "VkColorModelDescriptorSet::createPipeline",
            "vkCreateGraphicsPipelines"))

            RX_VK_MUTEX(
            for(size_t i = 0; i < stageCreateInfos.size(); i++)
            {
                vkDestroyShaderModule
                (Core::vkDevice,
                stageCreateInfos[i].module,
                nullptr);
            }) 
       }

       void destroyInstancedColorMeshPipeline(){
            RX_VK_MUTEX(
                vkDestroyPipeline(vkDevice, instancedColorMeshPipeline, nullptr);
            )
        }


        void createTextureModelPipeline(){
            std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos(2);
            stageCreateInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[0].flags = 0;
            stageCreateInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
            stageCreateInfos[0].module = textureModelVertexShader.createShaderModule();
            stageCreateInfos[0].pName = "main";
            stageCreateInfos[0].pSpecializationInfo = nullptr;
            stageCreateInfos[0].pNext = nullptr;
            stageCreateInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[1].flags = 0;
            stageCreateInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            stageCreateInfos[1].module = textureModelFragmentShader.createShaderModule();
            stageCreateInfos[1].pName = "main";
            stageCreateInfos[1].pSpecializationInfo = nullptr;
            stageCreateInfos[1].pNext = nullptr;


            VkVertexInputBindingDescription vertexBindingDescr{};
            vertexBindingDescr.binding = 0;
            vertexBindingDescr.stride = sizeof(Vertex::Texture);
            vertexBindingDescr.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            std::vector<VkVertexInputAttributeDescription> vertexAttributeDescr(3);
            vertexAttributeDescr[0].binding = 0;
            vertexAttributeDescr[0].location = 0;
            vertexAttributeDescr[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            vertexAttributeDescr[0].offset = offsetof(Vertex::Texture,position);
            vertexAttributeDescr[1].binding = 0;
            vertexAttributeDescr[1].location = 1;
            vertexAttributeDescr[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            vertexAttributeDescr[1].offset = offsetof(Vertex::Texture,normal);
            vertexAttributeDescr[2].binding = 0;
            vertexAttributeDescr[2].location = 2;
            vertexAttributeDescr[2].format = VK_FORMAT_R32G32_SFLOAT;
            vertexAttributeDescr[2].offset = offsetof(Vertex::Texture,texCoord);

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
            colorBlendAttachmentState.blendEnable = VK_TRUE;
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
            createInfo.layout = textureModelPipelineLayout;
            createInfo.renderPass = Core::vkRenderPass;
            createInfo.subpass = 0;
            createInfo.basePipelineHandle = VK_NULL_HANDLE;
            createInfo.basePipelineIndex = -1;
            
            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateGraphicsPipelines
            (Core::vkDevice,
            VK_NULL_HANDLE,
            1,
            &createInfo,
            nullptr,
            &textureModelPipeline),
            "createTextureModelPipeline",
            "vkCreateGraphicsPipelines"))

            RX_VK_MUTEX(
            for(size_t i = 0; i < stageCreateInfos.size(); i++)
            {
                vkDestroyShaderModule
                (Core::vkDevice,
                stageCreateInfos[i].module,
                nullptr);
            })
        }

        void destroyTextureModelPipeline(){
            RX_VK_MUTEX(
                vkDestroyPipeline(vkDevice, textureModelPipeline, nullptr);
            )
        }


        
        void createSkeletonModelPipeline(){
            std::vector<VkPipelineShaderStageCreateInfo> stageCreateInfos(2);
            stageCreateInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[0].flags = 0;
            stageCreateInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
            stageCreateInfos[0].module = Core::skeletonModelVertexShader.createShaderModule();
            stageCreateInfos[0].pName = "main";
            stageCreateInfos[0].pSpecializationInfo = nullptr;
            stageCreateInfos[0].pNext = nullptr;
            stageCreateInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfos[1].flags = 0;
            stageCreateInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            stageCreateInfos[1].module = Core::skeletonModelFragmentShader.createShaderModule();
            stageCreateInfos[1].pName = "main";
            stageCreateInfos[1].pSpecializationInfo = nullptr;
            stageCreateInfos[1].pNext = nullptr;

            VkVertexInputBindingDescription vertexBindingDescr{};
            vertexBindingDescr.binding = 0;
            vertexBindingDescr.stride = sizeof(Vertex::Skeleton);
            vertexBindingDescr.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            std::vector<VkVertexInputAttributeDescription> vertexAttributeDescr(6);
            vertexAttributeDescr[0].binding = 0;
            vertexAttributeDescr[0].location = 0;
            vertexAttributeDescr[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            vertexAttributeDescr[0].offset = offsetof(Vertex::Skeleton,position);
            vertexAttributeDescr[1].binding = 0;
            vertexAttributeDescr[1].location = 1;
            vertexAttributeDescr[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            vertexAttributeDescr[1].offset = offsetof(Vertex::Skeleton,normal);
            vertexAttributeDescr[2].binding = 0;
            vertexAttributeDescr[2].location = 2;
            vertexAttributeDescr[2].format = VK_FORMAT_R32G32_SFLOAT;
            vertexAttributeDescr[2].offset = offsetof(Vertex::Skeleton,texCoord);
            vertexAttributeDescr[3].binding = 0;
            vertexAttributeDescr[3].location = 3;
            vertexAttributeDescr[3].format = VK_FORMAT_R32G32B32A32_SINT;
            vertexAttributeDescr[3].offset = offsetof(Vertex::Skeleton,bones);
            vertexAttributeDescr[4].binding = 0;
            vertexAttributeDescr[4].location = 4;
            vertexAttributeDescr[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
            vertexAttributeDescr[4].offset = offsetof(Vertex::Skeleton,weights);
            vertexAttributeDescr[5].binding = 0;
            vertexAttributeDescr[5].location = 5;
            vertexAttributeDescr[5].format = VK_FORMAT_R32_SINT;
            vertexAttributeDescr[5].offset = offsetof(Vertex::Skeleton, nodeIndex);

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
            createInfo.layout = skeletonModelPipelineLayout;
            createInfo.renderPass = Core::vkRenderPass;
            createInfo.subpass = 0;
            createInfo.basePipelineHandle = VK_NULL_HANDLE;
            createInfo.basePipelineIndex = -1;

            RX_VK_MUTEX(
            RX_CHECK_VULKAN
            (vkCreateGraphicsPipelines
            (Core::vkDevice,
            VK_NULL_HANDLE,
            1,
            &createInfo,
            nullptr,
            &skeletonModelPipeline),
            "createSkeletonModelPipeline",
            "vkCreateGraphicsPipelines"))

            for(size_t i = 0; i < stageCreateInfos.size(); i++)
            {
                vkDestroyShaderModule
                (Core::vkDevice,
                stageCreateInfos[i].module,
                nullptr);
            }
        }

        void destroySkeletonModelPipeline(){
            RX_VK_MUTEX(
                vkDestroyPipeline(vkDevice, skeletonModelPipeline, nullptr);
            )
        }

        void createSkeletonModelCompPipeline(){
            VkPipelineShaderStageCreateInfo stageCreateInfo{};
            stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
            stageCreateInfo.module = skeletonModelComputeShader.createShaderModule();
            stageCreateInfo.pName = "main";

            VkComputePipelineCreateInfo pipelineCreateInfo{};
            pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
            pipelineCreateInfo.stage = stageCreateInfo;
            pipelineCreateInfo.layout = skeletonModelCompPipelineLayout;
            pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
            pipelineCreateInfo.basePipelineIndex = -1;

            RX_VK_MUTEX(
            RX_CHECK_VULKAN(
            vkCreateComputePipelines(
                Core::vkDevice,
                VK_NULL_HANDLE,
                1,
                &pipelineCreateInfo,
                nullptr,
                &skeletonModelCompPipeline),
                "createSkeletonModelCompPipeline",
                "vkCreateComputePipelines"
            ));

            RX_VK_MUTEX(
                vkDestroyShaderModule(Core::vkDevice, stageCreateInfo.module, nullptr);
            )
        }

        void destroySkeletonModelCompPipeline(){
            RX_VK_MUTEX(
                vkDestroyPipeline(Core::vkDevice, skeletonModelCompPipeline, nullptr);
            )
        }


        void createShadowColorMeshPipeline(){

            for(uint32_t i = 0; i < 16; i++){
                // Shader stage: vertex only (no fragment shader for depth-only pass)
                VkPipelineShaderStageCreateInfo vertexStageInfo{};
                vertexStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertexStageInfo.flags = 0;
                vertexStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vertexStageInfo.module = shadowColorMeshVertexShader.createShaderModule();
                vertexStageInfo.pName = "main";
                vertexStageInfo.pSpecializationInfo = nullptr;
                vertexStageInfo.pNext = nullptr;

                // Vertex input: only position from Vertex::Color
                VkVertexInputBindingDescription vertexBindingDescr{};
                vertexBindingDescr.binding = 0;
                vertexBindingDescr.stride = sizeof(Vertex::Color);
                vertexBindingDescr.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                VkVertexInputAttributeDescription vertexAttributeDescr{};
                vertexAttributeDescr.binding = 0;
                vertexAttributeDescr.location = 0;
                vertexAttributeDescr.format = VK_FORMAT_R32G32B32_SFLOAT;
                vertexAttributeDescr.offset = offsetof(Vertex::Color, position);

                VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
                vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
                vertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexBindingDescr;
                vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 1;
                vertexInputStateCreateInfo.pVertexAttributeDescriptions = &vertexAttributeDescr;

                VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
                inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

                // Viewport/scissor (will be set dynamically or use shadow map dimensions)
                VkViewport viewport{};
                viewport.x = 0.f;
                viewport.y = 0.f;
                viewport.width = static_cast<float>(Core::shadowMapArray.width);
                viewport.height = static_cast<float>(Core::shadowMapArray.height);
                viewport.minDepth = 0.f;
                viewport.maxDepth = 1.f;

                VkRect2D scissor{};
                scissor.offset = {0, 0};
                scissor.extent = {Core::shadowMapArray.width, Core::shadowMapArray.height};

                VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
                viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportStateCreateInfo.viewportCount = 1;
                viewportStateCreateInfo.pViewports = &viewport;
                viewportStateCreateInfo.scissorCount = 1;
                viewportStateCreateInfo.pScissors = &scissor;

                VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
                rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
                rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
                rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
                rasterizationStateCreateInfo.lineWidth = 1.f;
                rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
                rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
                rasterizationStateCreateInfo.depthBiasEnable = VK_TRUE; // Enable depth bias for shadow acne
                rasterizationStateCreateInfo.depthBiasConstantFactor = 1.25f;
                rasterizationStateCreateInfo.depthBiasClamp = 0.f;
                rasterizationStateCreateInfo.depthBiasSlopeFactor = 1.75f;

                VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
                multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
                multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // Shadow maps are typically single-sampled
                multisampleStateCreateInfo.minSampleShading = 1.f;
                multisampleStateCreateInfo.pSampleMask = nullptr;
                multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
                multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

                VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo{};
                depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
                depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
                depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
                depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
                depthStencilStateCreateInfo.minDepthBounds = 0.f;
                depthStencilStateCreateInfo.maxDepthBounds = 1.f;
                depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;

                // Dynamic rendering info (for VK_KHR_dynamic_rendering)
                VkPipelineRenderingCreateInfo renderingInfo{};
                renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
                renderingInfo.colorAttachmentCount = 0; // Depth-only pass
                renderingInfo.pColorAttachmentFormats = nullptr;
                renderingInfo.depthAttachmentFormat = Core::shadowMapArray.format; // Use shadow map format (e.g., VK_FORMAT_D32_SFLOAT)
                renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
                renderingInfo.viewMask = (1u << i+1) - 1u; // Multiview: enable all layers

                
                VkGraphicsPipelineCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                createInfo.pNext = &renderingInfo; // Chain dynamic rendering info
                createInfo.stageCount = 1; // Vertex stage only
                createInfo.pStages = &vertexStageInfo;
                createInfo.pVertexInputState = &vertexInputStateCreateInfo;
                createInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
                createInfo.pTessellationState = nullptr;
                createInfo.pViewportState = &viewportStateCreateInfo;
                createInfo.pRasterizationState = &rasterizationStateCreateInfo;
                createInfo.pMultisampleState = &multisampleStateCreateInfo;
                createInfo.pDepthStencilState = &depthStencilStateCreateInfo;
                createInfo.pColorBlendState = nullptr; // No color attachments
                createInfo.pDynamicState = nullptr;
                createInfo.layout = shadowColorMeshPipelineLayout;
                createInfo.renderPass = VK_NULL_HANDLE; // Dynamic rendering: no render pass
                createInfo.subpass = 0;
                createInfo.basePipelineHandle = VK_NULL_HANDLE;
                createInfo.basePipelineIndex = -1;

                RX_VK_MUTEX(
                RX_CHECK_VULKAN(
                    vkCreateGraphicsPipelines(
                        Core::vkDevice,
                        VK_NULL_HANDLE,
                        1,
                        &createInfo,
                        nullptr,
                        &shadowColorMeshPipeline[i]),
                    "createShadowColorMeshPipeline",
                    "vkCreateGraphicsPipelines"));

                RX_VK_MUTEX(
                    vkDestroyShaderModule(Core::vkDevice, vertexStageInfo.module, nullptr);
                )
            }
        
        }

        void destroyShadowColorMeshPipeline(){
            RX_VK_MUTEX(
                for (size_t i = 0; i < 16; ++i) {
                    vkDestroyPipeline(vkDevice, shadowColorMeshPipeline[i], nullptr);
                }
            )
        }


        void createShadowColorModelArrayPipeline(){
            for(int i = 0; i < 16; i++){
                // Shader stage: vertex only (no fragment shader for depth-only pass)
                VkPipelineShaderStageCreateInfo vertexStageInfo{};
                vertexStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertexStageInfo.flags = 0;
                vertexStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vertexStageInfo.module = shadowColorModelArrayVertexShader.createShaderModule();
                vertexStageInfo.pName = "main";
                vertexStageInfo.pSpecializationInfo = nullptr;
                vertexStageInfo.pNext = nullptr;

                // Vertex input: only position from Vertex::Color
                VkVertexInputBindingDescription vertexBindingDescr{};
                vertexBindingDescr.binding = 0;
                vertexBindingDescr.stride = sizeof(Vertex::Color);
                vertexBindingDescr.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                VkVertexInputAttributeDescription vertexAttributeDescr{};
                vertexAttributeDescr.binding = 0;
                vertexAttributeDescr.location = 0;
                vertexAttributeDescr.format = VK_FORMAT_R32G32B32_SFLOAT;
                vertexAttributeDescr.offset = offsetof(Vertex::Color, position);

                VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
                vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
                vertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexBindingDescr;
                vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 1;
                vertexInputStateCreateInfo.pVertexAttributeDescriptions = &vertexAttributeDescr;

                VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
                inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

                // Viewport/scissor (will be set dynamically or use shadow map dimensions)
                VkViewport viewport{};
                viewport.x = 0.f;
                viewport.y = 0.f;
                viewport.width = static_cast<float>(Core::shadowMapArray.width);
                viewport.height = static_cast<float>(Core::shadowMapArray.height);
                viewport.minDepth = 0.f;
                viewport.maxDepth = 1.f;

                VkRect2D scissor{};
                scissor.offset = {0, 0};
                scissor.extent = {Core::shadowMapArray.width, Core::shadowMapArray.height};

                VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
                viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportStateCreateInfo.viewportCount = 1;
                viewportStateCreateInfo.pViewports = &viewport;
                viewportStateCreateInfo.scissorCount = 1;
                viewportStateCreateInfo.pScissors = &scissor;

                VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
                rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
                rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
                rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
                rasterizationStateCreateInfo.lineWidth = 1.f;
                rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
                rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
                rasterizationStateCreateInfo.depthBiasEnable = VK_TRUE; // Enable depth bias for shadow acne
                rasterizationStateCreateInfo.depthBiasConstantFactor = 1.25f;
                rasterizationStateCreateInfo.depthBiasClamp = 0.f;
                rasterizationStateCreateInfo.depthBiasSlopeFactor = 1.75f;

                VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
                multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
                multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // Shadow maps are typically single-sampled
                multisampleStateCreateInfo.minSampleShading = 1.f;
                multisampleStateCreateInfo.pSampleMask = nullptr;
                multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
                multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

                VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo{};
                depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
                depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
                depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
                depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
                depthStencilStateCreateInfo.minDepthBounds = 0.f;
                depthStencilStateCreateInfo.maxDepthBounds = 1.f;
                depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;

                // Dynamic rendering info (for VK_KHR_dynamic_rendering)
                VkPipelineRenderingCreateInfo renderingInfo{};
                renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
                renderingInfo.colorAttachmentCount = 0; // Depth-only pass
                renderingInfo.pColorAttachmentFormats = nullptr;
                renderingInfo.depthAttachmentFormat = Core::shadowMapArray.format; // Use shadow map format (e.g., VK_FORMAT_D32_SFLOAT)
                renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
                renderingInfo.viewMask = (1u << i+1) - 1u; // Multiview: enable all layers

                
                VkGraphicsPipelineCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                createInfo.pNext = &renderingInfo; // Chain dynamic rendering info
                createInfo.stageCount = 1; // Vertex stage only
                createInfo.pStages = &vertexStageInfo;
                createInfo.pVertexInputState = &vertexInputStateCreateInfo;
                createInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
                createInfo.pTessellationState = nullptr;
                createInfo.pViewportState = &viewportStateCreateInfo;
                createInfo.pRasterizationState = &rasterizationStateCreateInfo;
                createInfo.pMultisampleState = &multisampleStateCreateInfo;
                createInfo.pDepthStencilState = &depthStencilStateCreateInfo;
                createInfo.pColorBlendState = nullptr; // No color attachments
                createInfo.pDynamicState = nullptr;
                createInfo.layout = shadowColorModelArrayPipelineLayout;
                createInfo.renderPass = VK_NULL_HANDLE; // Dynamic rendering: no render pass
                createInfo.subpass = 0;
                createInfo.basePipelineHandle = VK_NULL_HANDLE;
                createInfo.basePipelineIndex = -1;

                RX_VK_MUTEX(
                RX_CHECK_VULKAN(
                    vkCreateGraphicsPipelines(
                        Core::vkDevice,
                        VK_NULL_HANDLE,
                        1,
                        &createInfo,
                        nullptr,
                        &shadowColorModelArrayPipeline[i]),
                    "createShadowColorMeshPipeline",
                    "vkCreateGraphicsPipelines"));

                RX_VK_MUTEX(
                    vkDestroyShaderModule(Core::vkDevice, vertexStageInfo.module, nullptr);
                )
            }
        }

        void destroyShadowColorModelArrayPipeline(){
            RX_VK_MUTEX(
                for (size_t i = 0; i < 16; ++i) {
                    vkDestroyPipeline(vkDevice, shadowColorModelArrayPipeline[i], nullptr);
                }
            )
        }
        
        void createShadowTextureModelPipeline(){
            for(uint32_t i = 0; i < 16; i++){
                // Shader stage: vertex only (no fragment shader for depth-only pass)
                VkPipelineShaderStageCreateInfo vertexStageInfo{};
                vertexStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertexStageInfo.flags = 0;
                vertexStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vertexStageInfo.module = shadowTextureModelVertexShader.createShaderModule();
                vertexStageInfo.pName = "main";
                vertexStageInfo.pSpecializationInfo = nullptr;
                vertexStageInfo.pNext = nullptr;

                // Vertex input: only position from Vertex::Texture
                VkVertexInputBindingDescription vertexBindingDescr{};
                vertexBindingDescr.binding = 0;
                vertexBindingDescr.stride = sizeof(Vertex::Texture);
                vertexBindingDescr.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                VkVertexInputAttributeDescription vertexAttributeDescr{};
                vertexAttributeDescr.binding = 0;
                vertexAttributeDescr.location = 0;
                vertexAttributeDescr.format = VK_FORMAT_R32G32B32_SFLOAT;
                vertexAttributeDescr.offset = offsetof(Vertex::Texture, position);

                VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
                vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
                vertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexBindingDescr;
                vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 1;
                vertexInputStateCreateInfo.pVertexAttributeDescriptions = &vertexAttributeDescr;

                VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
                inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

                // Viewport/scissor (will be set dynamically or use shadow map dimensions)
                VkViewport viewport{};
                viewport.x = 0.f;
                viewport.y = 0.f;
                viewport.width = static_cast<float>(Core::shadowMapArray.width);
                viewport.height = static_cast<float>(Core::shadowMapArray.height);
                viewport.minDepth = 0.f;
                viewport.maxDepth = 1.f;

                VkRect2D scissor{};
                scissor.offset = {0, 0};
                scissor.extent = {Core::shadowMapArray.width, Core::shadowMapArray.height};

                VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
                viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportStateCreateInfo.viewportCount = 1;
                viewportStateCreateInfo.pViewports = &viewport;
                viewportStateCreateInfo.scissorCount = 1;
                viewportStateCreateInfo.pScissors = &scissor;

                VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
                rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
                rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
                rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
                rasterizationStateCreateInfo.lineWidth = 1.f;
                rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
                rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
                rasterizationStateCreateInfo.depthBiasEnable = VK_TRUE; // Enable depth bias for shadow acne
                rasterizationStateCreateInfo.depthBiasConstantFactor = 1.25f;
                rasterizationStateCreateInfo.depthBiasClamp = 0.f;
                rasterizationStateCreateInfo.depthBiasSlopeFactor = 1.75f;

                VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
                multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
                multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // Shadow maps are typically single-sampled
                multisampleStateCreateInfo.minSampleShading = 1.f;
                multisampleStateCreateInfo.pSampleMask = nullptr;
                multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
                multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

                VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo{};
                depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
                depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
                depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
                depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
                depthStencilStateCreateInfo.minDepthBounds = 0.f;
                depthStencilStateCreateInfo.maxDepthBounds = 1.f;
                depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;

                // Dynamic rendering info (for VK_KHR_dynamic_rendering)
                VkPipelineRenderingCreateInfo renderingInfo{};
                renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
                renderingInfo.colorAttachmentCount = 0; // Depth-only pass
                renderingInfo.pColorAttachmentFormats = nullptr;
                renderingInfo.depthAttachmentFormat = Core::shadowMapArray.format; // Use shadow map format (e.g., VK_FORMAT_D32_SFLOAT)
                renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
                renderingInfo.viewMask = (1u << i+1) - 1u; // Multiview: enable all layers

                
                VkGraphicsPipelineCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                createInfo.pNext = &renderingInfo; // Chain dynamic rendering info
                createInfo.stageCount = 1; // Vertex stage only
                createInfo.pStages = &vertexStageInfo;
                createInfo.pVertexInputState = &vertexInputStateCreateInfo;
                createInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
                createInfo.pTessellationState = nullptr;
                createInfo.pViewportState = &viewportStateCreateInfo;
                createInfo.pRasterizationState = &rasterizationStateCreateInfo;
                createInfo.pMultisampleState = &multisampleStateCreateInfo;
                createInfo.pDepthStencilState = &depthStencilStateCreateInfo;
                createInfo.pColorBlendState = nullptr; // No color attachments
                createInfo.pDynamicState = nullptr;
                createInfo.layout = shadowTextureModelPipelineLayout;
                createInfo.renderPass = VK_NULL_HANDLE; // Dynamic rendering: no render pass
                createInfo.subpass = 0;
                createInfo.basePipelineHandle = VK_NULL_HANDLE;
                createInfo.basePipelineIndex = -1;

                RX_VK_MUTEX(
                RX_CHECK_VULKAN(
                    vkCreateGraphicsPipelines(
                        Core::vkDevice,
                        VK_NULL_HANDLE,
                        1,
                        &createInfo,
                        nullptr,
                        &shadowTextureModelPipeline[i]),
                    "createShadowTextureModelPipeline",
                    "vkCreateGraphicsPipelines"));

                RX_VK_MUTEX(
                    vkDestroyShaderModule(Core::vkDevice, vertexStageInfo.module, nullptr);
                )
            }
        }

        void destroyShadowTextureModelPipeline(){
            RX_VK_MUTEX(
                for (size_t i = 0; i < 16; ++i) {
                    vkDestroyPipeline(vkDevice, shadowTextureModelPipeline[i], nullptr);
                }
            )
        }


        void createShadowSkeletonModelPipeline(){
            for(uint32_t i = 0; i < 16; i++){
                // Shader stage: vertex only (no fragment shader for depth-only pass)
                VkPipelineShaderStageCreateInfo vertexStageInfo{};
                vertexStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertexStageInfo.flags = 0;
                vertexStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vertexStageInfo.module = shadowSkeletonModelVertexShader.createShaderModule();
                vertexStageInfo.pName = "main";
                vertexStageInfo.pSpecializationInfo = nullptr;
                vertexStageInfo.pNext = nullptr;

                VkVertexInputBindingDescription vertexBindingDescr{};
                vertexBindingDescr.binding = 0;
                vertexBindingDescr.stride = sizeof(Vertex::Skeleton);
                vertexBindingDescr.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                std::vector<VkVertexInputAttributeDescription> vertexAttributeDescr(6);
                vertexAttributeDescr[0].binding = 0;
                vertexAttributeDescr[0].location = 0;
                vertexAttributeDescr[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                vertexAttributeDescr[0].offset = offsetof(Vertex::Skeleton,position);
                vertexAttributeDescr[1].binding = 0;
                vertexAttributeDescr[1].location = 1;
                vertexAttributeDescr[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                vertexAttributeDescr[1].offset = offsetof(Vertex::Skeleton,normal);
                vertexAttributeDescr[2].binding = 0;
                vertexAttributeDescr[2].location = 2;
                vertexAttributeDescr[2].format = VK_FORMAT_R32G32_SFLOAT;
                vertexAttributeDescr[2].offset = offsetof(Vertex::Skeleton,texCoord);
                vertexAttributeDescr[3].binding = 0;
                vertexAttributeDescr[3].location = 3;
                vertexAttributeDescr[3].format = VK_FORMAT_R32G32B32A32_SINT;
                vertexAttributeDescr[3].offset = offsetof(Vertex::Skeleton,bones);
                vertexAttributeDescr[4].binding = 0;
                vertexAttributeDescr[4].location = 4;
                vertexAttributeDescr[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
                vertexAttributeDescr[4].offset = offsetof(Vertex::Skeleton,weights);
                vertexAttributeDescr[5].binding = 0;
                vertexAttributeDescr[5].location = 5;
                vertexAttributeDescr[5].format = VK_FORMAT_R32_SINT;
                vertexAttributeDescr[5].offset = offsetof(Vertex::Skeleton, nodeIndex);

                VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
                vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
                vertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexBindingDescr;
                vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 6;
                vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexAttributeDescr.data();

                VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
                inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

                // Viewport/scissor (will be set dynamically or use shadow map dimensions)
                VkViewport viewport{};
                viewport.x = 0.f;
                viewport.y = 0.f;
                viewport.width = static_cast<float>(Core::shadowMapArray.width);
                viewport.height = static_cast<float>(Core::shadowMapArray.height);
                viewport.minDepth = 0.f;
                viewport.maxDepth = 1.f;

                VkRect2D scissor{};
                scissor.offset = {0, 0};
                scissor.extent = {Core::shadowMapArray.width, Core::shadowMapArray.height};

                VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
                viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportStateCreateInfo.viewportCount = 1;
                viewportStateCreateInfo.pViewports = &viewport;
                viewportStateCreateInfo.scissorCount = 1;
                viewportStateCreateInfo.pScissors = &scissor;

                VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
                rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
                rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
                rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
                rasterizationStateCreateInfo.lineWidth = 1.f;
                rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
                rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
                rasterizationStateCreateInfo.depthBiasEnable = VK_TRUE; // Enable depth bias for shadow acne
                rasterizationStateCreateInfo.depthBiasConstantFactor = 1.25f;
                rasterizationStateCreateInfo.depthBiasClamp = 0.f;
                rasterizationStateCreateInfo.depthBiasSlopeFactor = 1.75f;

                VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{};
                multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
                multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // Shadow maps are typically single-sampled
                multisampleStateCreateInfo.minSampleShading = 1.f;
                multisampleStateCreateInfo.pSampleMask = nullptr;
                multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
                multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

                VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo{};
                depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
                depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
                depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
                depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
                depthStencilStateCreateInfo.minDepthBounds = 0.f;
                depthStencilStateCreateInfo.maxDepthBounds = 1.f;
                depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;

                // Dynamic rendering info (for VK_KHR_dynamic_rendering)
                VkPipelineRenderingCreateInfo renderingInfo{};
                renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
                renderingInfo.colorAttachmentCount = 0; // Depth-only pass
                renderingInfo.pColorAttachmentFormats = nullptr;
                renderingInfo.depthAttachmentFormat = Core::shadowMapArray.format; // Use shadow map format (e.g., VK_FORMAT_D32_SFLOAT)
                renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
                renderingInfo.viewMask = (1u << i+1) - 1u; // Multiview: enable all layers

                
                VkGraphicsPipelineCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                createInfo.pNext = &renderingInfo; // Chain dynamic rendering info
                createInfo.stageCount = 1; // Vertex stage only
                createInfo.pStages = &vertexStageInfo;
                createInfo.pVertexInputState = &vertexInputStateCreateInfo;
                createInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
                createInfo.pTessellationState = nullptr;
                createInfo.pViewportState = &viewportStateCreateInfo;
                createInfo.pRasterizationState = &rasterizationStateCreateInfo;
                createInfo.pMultisampleState = &multisampleStateCreateInfo;
                createInfo.pDepthStencilState = &depthStencilStateCreateInfo;
                createInfo.pColorBlendState = nullptr; // No color attachments
                createInfo.pDynamicState = nullptr;
                createInfo.layout = shadowSkeletonModelPipelineLayout;
                createInfo.renderPass = VK_NULL_HANDLE; // Dynamic rendering: no render pass
                createInfo.subpass = 0;
                createInfo.basePipelineHandle = VK_NULL_HANDLE;
                createInfo.basePipelineIndex = -1;

                RX_VK_MUTEX(
                RX_CHECK_VULKAN(
                    vkCreateGraphicsPipelines(
                        Core::vkDevice,
                        VK_NULL_HANDLE,
                        1,
                        &createInfo,
                        nullptr,
                        &shadowSkeletonModelPipeline[i]),
                    "createShadowSkeletonModelPipeline",
                    "vkCreateGraphicsPipelines"));

                RX_VK_MUTEX(
                    vkDestroyShaderModule(Core::vkDevice, vertexStageInfo.module, nullptr);
                )
            }
        }

        void destroyShadowSkeletonModelPipeline(){
            RX_VK_MUTEX(
                for (size_t i = 0; i < 16; ++i) {
                    vkDestroyPipeline(vkDevice, shadowSkeletonModelPipeline[i], nullptr);
                }
            )
        }
    }
} // namespace Rx::Core