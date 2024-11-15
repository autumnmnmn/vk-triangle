#ifndef _VK_SURFACE
#define _VK_SURFACE

#include <cstring>
#include <format>
#include <limits>
#include <algorithm>

#include "../general/math.h"
#include "../window/window.h"

#include "macros.h"
#include "queues.h"

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

void selectSwapExtent(SurfaceDetails &details, window::Window window) {
    if (details.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        details.selectedExtent = details.capabilities.currentExtent;
    } else {
        math::vec2u dimensions = window::getDimensions(window);

        VkExtent2D extent = { dimensions.x, dimensions.y };

        auto minExtent = details.capabilities.minImageExtent;
        auto maxExtent = details.capabilities.maxImageExtent;

        extent.width = std::clamp(extent.width, minExtent.width, maxExtent.width);
        extent.height = std::clamp(extent.height, minExtent.height, maxExtent.height);

        details.selectedExtent = extent;
    }
}

SurfaceDetails getDetails(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SurfaceDetails details;

    details.surface = surface;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    STDOUT("CAP ALPHA: " << details.capabilities.supportedCompositeAlpha);

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

    return details;
}

#endif

