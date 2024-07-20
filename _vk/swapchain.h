#ifndef _VK_SWAPCHAIN
#define _VK_SWAPCHAIN

#include <cstring>
#include <format>
#include <limits>
#include <algorithm>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "queues.h"
#include "surface.h"
#include "../general/macros.h"

VkSwapchainCreateInfoKHR createSwapchainCreateInfo(SurfaceDetails surface, QueueFamilyIndices indices) {
    uint32_t imageCount = surface.capabilities.minImageCount + 1;
    if (surface.capabilities.maxImageCount > 0 && imageCount > surface.capabilities.maxImageCount) {
        imageCount = surface.capabilities.maxImageCount;
    }

    swapchainInfo(createInfo,
        .surface = surface.surface,
        .minImageCount = imageCount,
        .imageFormat = surface.selectedFormat.format,
        .imageColorSpace = surface.selectedFormat.colorSpace,
        .imageExtent = surface.selectedExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = surface.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = surface.selectedMode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    );

    uint32_t queueFamilyIndices[] = {indices.graphics.value(), indices.surface.value()};

    if (indices.graphics != indices.surface) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    return createInfo;
}

VkSwapchainKHR createSwapchain(VkDevice device, VkSwapchainCreateInfoKHR createInfo) {
    VkSwapchainKHR swapchain;
    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swapchain");
    }
    return swapchain;
}

vec<VkImage> getImages(VkDevice device, VkSwapchainKHR swapchain) {
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
    vec<VkImage> images(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data());
    return images;
}

vec<VkImageView> getViews(VkDevice device, vec<VkImage> images, SurfaceDetails surface) {
    vec<VkImageView> views(images.size());

    for (size_t index = 0; index < images.size(); ++index) {
        imageViewInfo(createInfo,
            .image = images[index],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = surface.selectedFormat.format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        );

        if (vkCreateImageView(device, &createInfo, nullptr, &views[index]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views");
        }
    }

    return views;
}

vec<VkFramebuffer> getFramebuffers(VkDevice device, vec<VkImageView> imageViews, VkRenderPass renderPass, SurfaceDetails surfaceDetails) {
    vec<VkFramebuffer> framebuffers(imageViews.size());

    for (size_t index = 0; index < imageViews.size(); ++index) {
        VkImageView attachments[] = {
            imageViews[index]
        };

        framebufferInfo(info,
            .renderPass = renderPass,
            .attachmentCount = 1,
            .pAttachments = attachments, // &imageViews[index] directly?
            .width = surfaceDetails.selectedExtent.width,
            .height = surfaceDetails.selectedExtent.height,
            .layers = 1
        );

        if (vkCreateFramebuffer(device, &info, nullptr, &framebuffers[index]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        }
    }

    return framebuffers;
}

#endif

