#ifndef _VK_DEVICE
#define _VK_DEVICE

#include <stdexcept>
#include <set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "pdevice.h"
#include "queues.h"
#include "../general/macros.h"

VkDevice createVkDev(VkPhysicalDevice physicalDevice, QueueFamilyIndices indices) {
    vec<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueIndices = { indices.graphics.value(), indices.surface.value() };

    float queuePriority = 1.0f;
    for (uint32_t index : uniqueIndices) {
        deviceQueueInfo(queueCreateInfo);
        queueCreateInfo.queueFamilyIndex = indices.graphics.value();
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    deviceInfo(createInfo);
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if (true) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkDevice device;
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    return device;
}


#endif

