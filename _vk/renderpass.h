#ifndef _VK_RENDERPASS
#define _VK_RENDERPASS

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../general/macros.h"

#define l_op(x) VK_ATTACHMENT_LOAD_OP_ ## x
#define s_op(x) VK_ATTACHMENT_STORE_OP_ ## x

VkRenderPass createRenderPass(VkDevice device, SurfaceDetails surfaceDetails) {
    VkAttachmentDescription colorAttachment = {
        .format = surfaceDetails.selectedFormat.format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = l_op(CLEAR),
        .storeOp = s_op(STORE),
        .stencilLoadOp = l_op(DONT_CARE),
        .stencilStoreOp = s_op(DONT_CARE),
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VkAttachmentReference colorAttachmentRef = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef
    };

    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    };

    renderPassInfo(renderPassInfo,
        .attachmentCount = 1,
        .pAttachments = &colorAttachment,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    );

    VkRenderPass renderPass;
    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass");
    }
    return renderPass;
}

#undef l_op
#undef s_op

#endif

