#ifndef _VK_PDEVICE
#define _VK_PDEVICE

#include <stdexcept>
#include <cstring>
#include <format>
#include <limits>
#include <algorithm>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "queues.h"
#include "surface.h"
#include "../general/macros.h"

const vec<const char*> requiredExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

vec<VkExtensionProperties> getAvailableExtensions(VkPhysicalDevice device) {
    uint32_t count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> extensions(count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data());
    return extensions;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
    auto availableExtensions = getAvailableExtensions(device);
    for (const auto &requirement : requiredExtensions) {
        bool requirementMet = false;
        for (const auto &extension : availableExtensions) {
            requirementMet = !strcmp(requirement, extension.extensionName);
            if (requirementMet) break;
        }
        if (!requirementMet) {
            return false;
        }
    }

    auto surfaceDetails = getDetails(device, surface, nullptr);
    if (surfaceDetails.formats.empty() || surfaceDetails.presentModes.empty()) {
        return false;
    }

    QueueFamilyIndices indices = findQueueFamilies(device, surface);
    return indices.isComplete();
}

VkPhysicalDevice selectPhysDev(VkInstance instance, VkSurfaceKHR surface) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("no physical devices available");
    }
    vec<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;

    for (const auto &device : devices) {
        if (isDeviceSuitable(device, surface)) {
            selectedDevice = device;
            break;
        }
    }

    if (selectedDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("no suitable physical device");
    }

    return selectedDevice;
}

#endif

