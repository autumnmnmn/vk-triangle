#ifndef _VK_COMMAND
#define _VK_COMMAND

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../general/macros.h"

VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamilyIndex) {
    commandPoolInfo(info,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queueFamilyIndex
    );

    VkCommandPool pool;
    if (vkCreateCommandPool(device, &info, nullptr, &pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool");
    }
    return pool;
}

VkCommandBuffer createCommandBuffer(VkDevice device, VkCommandPool commandPool) {
    commandBufferInfo(info,
        .commandPool = commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    );

    VkCommandBuffer buffer;
    if (vkAllocateCommandBuffers(device, &info, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }
    return buffer;
}

void recordRenderCommands(VkCommandBuffer buffer, VkFramebuffer framebuffer, VkRenderPass renderPass, SurfaceDetails surfaceDetails, VkPipeline pipeline) {
    commandBufferBeginInfo(bufferBegin,
        .flags = 0,
        .pInheritanceInfo = nullptr
    );

    if (vkBeginCommandBuffer(buffer, &bufferBegin) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer");
    }

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

    renderPassBeginInfo(renderBegin,
        .renderPass = renderPass,
        .framebuffer = framebuffer,
        .renderArea = {
            .offset = {0, 0},
            .extent = surfaceDetails.selectedExtent
        },
        .clearValueCount = 1,
        .pClearValues = &clearColor
    );

    vkCmdBeginRenderPass(buffer, &renderBegin, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(surfaceDetails.selectedExtent.width),
        .height = static_cast<float>(surfaceDetails.selectedExtent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };
    vkCmdSetViewport(buffer, 0, 1, &viewport);

    VkRect2D scissor = {
        .offset = {0, 0},
        .extent = surfaceDetails.selectedExtent
    };
    vkCmdSetScissor(buffer, 0, 1, &scissor);

    vkCmdDraw(buffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(buffer);

    if (vkEndCommandBuffer(buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }
}

#endif

