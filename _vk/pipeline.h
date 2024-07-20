#ifndef _VK_PIPELINE
#define _VK_PIPELINE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "shader.h"
#include "../general/macros.h"


const vec<VkDynamicState> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
};

VkPipelineLayout createPipelineLayout(VkDevice device) {
    pipelineLayoutInfo(info,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr
    );

    VkPipelineLayout layout;
    if (vkCreatePipelineLayout(device, &info, nullptr, &layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout");
    }
    return layout;
}

VkPipeline createGraphicsPipeline(VkDevice device, SurfaceDetails surfaceDetails, VkPipelineLayout pipelineLayout, VkRenderPass renderPass) {
    auto vertexShaderModule = createShaderModule(device, "shaders/spv/vertex.spv");
    auto fragmentShaderModule = createShaderModule(device, "shaders/spv/fragment.spv");

    pipelineShaderStageInfo(vertexStageInfo,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertexShaderModule,
        .pName = "main"
    );

    pipelineShaderStageInfo(fragmentStageInfo,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragmentShaderModule,
        .pName = "main"
    );

    VkPipelineShaderStageCreateInfo stages[] = {vertexStageInfo, fragmentStageInfo};

    pipelineDynamicStateInfo(dynamicStateInfo,
        .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
        .pDynamicStates = dynamicStates.data()
    );

    pipelineVertexInputStateInfo(vertexInputInfo,
        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions = nullptr,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions = nullptr
    );

    pipelineInputAssemblyStateInfo(inputAssemblyInfo,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    );

    VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) surfaceDetails.selectedExtent.width,
        .height = (float) surfaceDetails.selectedExtent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    VkRect2D scissor = {
        .offset = {0, 0},
        .extent = surfaceDetails.selectedExtent
    };

    pipelineViewportStateInfo(viewportStateInfo,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor
    );

    pipelineRasterizationStateInfo(rasterizationInfo,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f
    );

    pipelineMultisampleStateInfo(multisamplingInfo,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE
    );

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
        .blendEnable = VK_FALSE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    pipelineColorBlendStateInfo(colorBlendInfo,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachment,
        .blendConstants = {
            [0] = 0.0f,
            [1] = 0.0f,
            [2] = 0.0f,
            [3] = 0.0f
        }
    );

    graphicsPipelineInfo(pipelineInfo,
        .stageCount = 2,
        .pStages = stages,
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &inputAssemblyInfo,
        .pViewportState = &viewportStateInfo,
        .pRasterizationState = &rasterizationInfo,
        .pMultisampleState = &multisamplingInfo,
        .pDepthStencilState = nullptr,
        .pColorBlendState = &colorBlendInfo,
        .pDynamicState = &dynamicStateInfo,
        .layout = pipelineLayout,
        .renderPass = renderPass,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1
    );

    VkPipeline pipeline;

    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }

    vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
    vkDestroyShaderModule(device, vertexShaderModule, nullptr);

    return pipeline;
}

#endif

