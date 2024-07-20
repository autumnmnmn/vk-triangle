#ifndef _VK_SYNC
#define _VK_SYNC

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../general/macros.h"

VkSemaphore createSemaphore(VkDevice device) {
    semaphoreInfo(info);
    VkSemaphore semaphore;
    if (vkCreateSemaphore(device, &info, nullptr, &semaphore) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphore");
    }
    return semaphore;
}

VkFence createFence(VkDevice device) {
    fenceInfo(info,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    );
    VkFence fence;
    if (vkCreateFence(device, &info, nullptr, &fence) != VK_SUCCESS) {
        throw std::runtime_error("failed to create fence");
    }
    return fence;
}

#endif

