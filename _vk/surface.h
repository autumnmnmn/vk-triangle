#ifndef _VK_SURFACE
#define _VK_SURFACE

#include <cstring>
#include <format>
#include <limits>
#include <algorithm>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "queues.h"
#include "../general/macros.h"

struct SurfaceDetails {
    VkSurfaceKHR surface;
    VkSurfaceCapabilitiesKHR capabilities;
    vec<VkSurfaceFormatKHR> formats;
    vec<VkPresentModeKHR> presentModes;
    VkSurfaceFormatKHR selectedFormat;
    VkPresentModeKHR selectedMode;
    VkExtent2D selectedExtent;
};

VkSurfaceFormatKHR selectSurfaceFormat(vec<VkSurfaceFormatKHR> &formats) {
    for (const auto &format : formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }
    return formats[0];
}

VkPresentModeKHR selectPresentMode(vec<VkPresentModeKHR> &modes) {
    for (const auto &mode : modes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D selectSwapExtent(VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

        auto minExtent = capabilities.minImageExtent;
        auto maxExtent = capabilities.maxImageExtent;

        extent.width = std::clamp(extent.width, minExtent.width, maxExtent.width);
        extent.height = std::clamp(extent.height, minExtent.height, maxExtent.height);

        return extent;
    }
}

SurfaceDetails getDetails(VkPhysicalDevice device, VkSurfaceKHR surface, GLFWwindow *window) {
    SurfaceDetails details;

    details.surface = surface;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);
    if (count) {
        details.formats.resize(count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, details.formats.data());
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);
    if (count) {
        details.presentModes.resize(count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, details.presentModes.data());
    }

    details.selectedFormat = selectSurfaceFormat(details.formats);
    details.selectedMode = selectPresentMode(details.presentModes);

    if (window != nullptr) {
        details.selectedExtent = selectSwapExtent(details.capabilities, window);
    }

    return details;
}

#endif

