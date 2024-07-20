#ifndef _VK_QUEUES
#define _VK_QUEUES

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../general/macros.h"

struct QueueFamilyIndices {
    opt<uint32_t> graphics;
    opt<uint32_t> surface;

    bool isComplete() {
        return graphics.has_value() && surface.has_value();
    }
};

vec<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    vec<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    return queueFamilies;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;

    auto queueFamilies = getQueueFamilyProperties(device);

    for (size_t index = 0; index < queueFamilies.size(); ++index) {
        if (queueFamilies[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics = index;
        }

        VkBool32 surfaceSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &surfaceSupport);
        if (surfaceSupport) {
            indices.surface = index;
        }

        if (indices.isComplete()) break;
    }

    return indices;
}

VkQueue getDeviceQueue(VkDevice device, uint32_t familyIndex) {
    VkQueue queue;
    vkGetDeviceQueue(device, familyIndex, 0, &queue);
    return queue;
}

#endif
