#ifndef _VK_VKSTATE
#define _VK_VKSTATE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "debug.h"
#include "queues.h"
#include "device.h"
#include "pdevice.h"
#include "instance.h"
#include "surface.h"
#include "swapchain.h"
#include "pipeline.h"
#include "renderpass.h"
#include "command.h"
#include "sync.h"
#include "../_glfw/window.h"
#include "../general/macros.h"

struct VkState {
    GLFWwindow *glfwWindow;
    VkAllocationCallbacks *pAllocator = nullptr;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    QueueFamilyIndices queueFamilies;
    VkQueue graphicsQueue;
    VkQueue surfaceQueue;
    SurfaceDetails surfaceDetails;
    VkSwapchainKHR swapchain;
    vec<VkImage> images;
    vec<VkImageView> imageViews;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkPipeline pipeline;
    vec<VkFramebuffer> framebuffers;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    VkSemaphore imageAvailable;
    VkSemaphore renderFinished;
    VkFence inFlight;
    bool framebufferResized = false;
};

void createSwapchain(VkState &vk) {
    vk.surfaceDetails = getDetails(vk.physicalDevice, vk.surfaceDetails.surface, vk.glfwWindow);
    auto swapchainCreateInfo = createSwapchainCreateInfo(vk.surfaceDetails, vk.queueFamilies);
    vk.swapchain = createSwapchain(vk.device, swapchainCreateInfo);
    vk.images = getImages(vk.device, vk.swapchain);
    vk.imageViews = getViews(vk.device, vk.images, vk.surfaceDetails);
    vk.framebuffers = getFramebuffers(vk.device, vk.imageViews, vk.renderPass, vk.surfaceDetails);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    auto vk = reinterpret_cast<VkState*>(glfwGetWindowUserPointer(window));
    vk->framebufferResized = true;
}

VkState initVulkan(GLFWwindow *window) {
    VkState vk;

    glfwSetWindowUserPointer(window, &vk);
    vk.glfwWindow = window;

    vk.instance = createInstance();
#ifndef NO_DEBUG
    vk.debugMessenger = setupDebugMessenger(vk.instance);
#endif
    auto surface = createSurface(window, vk.instance);
    vk.physicalDevice = selectPhysDev(vk.instance, surface);
    vk.surfaceDetails = getDetails(vk.physicalDevice, surface, window);

    vk.queueFamilies = findQueueFamilies(vk.physicalDevice, surface);
    vk.device = createVkDev(vk.physicalDevice, vk.queueFamilies);

    vk.graphicsQueue = getDeviceQueue(vk.device, vk.queueFamilies.graphics.value());
    vk.surfaceQueue = getDeviceQueue(vk.device, vk.queueFamilies.surface.value());

    vk.pipelineLayout = createPipelineLayout(vk.device);
    vk.renderPass = createRenderPass(vk.device, vk.surfaceDetails);
    vk.pipeline = createGraphicsPipeline(vk.device, vk.surfaceDetails, vk.pipelineLayout, vk.renderPass);

    vk.commandPool = createCommandPool(vk.device, vk.queueFamilies.graphics.value());
    vk.commandBuffer = createCommandBuffer(vk.device, vk.commandPool);

    vk.imageAvailable = createSemaphore(vk.device);
    vk.renderFinished = createSemaphore(vk.device);
    vk.inFlight = createFence(vk.device);

    createSwapchain(vk);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    return vk;
}

void destroySwapchain(VkState &vk) {
    for (auto framebuffer : vk.framebuffers) {
        vkDestroyFramebuffer(vk.device, framebuffer, vk.pAllocator);
    }
    for (auto view : vk.imageViews) {
        vkDestroyImageView(vk.device, view, vk.pAllocator);
    }
    vkDestroySwapchainKHR(vk.device, vk.swapchain, vk.pAllocator);
}

void refreshSwapchain(VkState &vk) {
    vkDeviceWaitIdle(vk.device);

    destroySwapchain(vk);

    createSwapchain(vk);
}


void drawFrame(VkState &vk) {
    vkWaitForFences(vk.device, 1, &vk.inFlight, VK_TRUE, UINT64_MAX);


    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(vk.device, vk.swapchain, UINT64_MAX, vk.imageAvailable, VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || vk.framebufferResized || result == VK_SUBOPTIMAL_KHR) {
        refreshSwapchain(vk);
        vk.framebufferResized = false;
        vkDeviceWaitIdle(vk.device);
        vkDestroySemaphore(vk.device, vk.imageAvailable, vk.pAllocator);
        vk.imageAvailable = createSemaphore(vk.device);
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swapchain image");
    }

    vkResetFences(vk.device, 1, &vk.inFlight);
    vkResetCommandBuffer(vk.commandBuffer, 0);

    recordRenderCommands(vk.commandBuffer, vk.framebuffers[imageIndex], vk.renderPass, vk.surfaceDetails, vk.pipeline);

    VkSemaphore waitSemaphores[] = {vk.imageAvailable};
    VkSemaphore signalSemaphores[] = {vk.renderFinished};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    submitInfo(submit,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &vk.commandBuffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signalSemaphores
    );

    if (vkQueueSubmit(vk.graphicsQueue, 1, &submit, vk.inFlight) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    presentInfo(present,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signalSemaphores,
        .swapchainCount = 1,
        .pSwapchains = &vk.swapchain,
        .pImageIndices = &imageIndex
    );

    vkQueuePresentKHR(vk.surfaceQueue, &present);

}


void cleanup(VkState &vk) {
    vkDeviceWaitIdle(vk.device);

    vkDestroySemaphore(vk.device, vk.imageAvailable, vk.pAllocator);
    vkDestroySemaphore(vk.device, vk.renderFinished, vk.pAllocator);
    vkDestroyFence(vk.device, vk.inFlight, vk.pAllocator);

    vkDestroyCommandPool(vk.device, vk.commandPool, vk.pAllocator);

    destroySwapchain(vk);

    vkDestroyPipeline(vk.device, vk.pipeline, vk.pAllocator);
    vkDestroyRenderPass(vk.device, vk.renderPass, vk.pAllocator);
    vkDestroyPipelineLayout(vk.device, vk.pipelineLayout, vk.pAllocator);
    vkDestroyDevice(vk.device, vk.pAllocator);
    vkDestroySurfaceKHR(vk.instance, vk.surfaceDetails.surface, vk.pAllocator);

#ifndef NO_DEBUG
    DestroyDebugUtilsMessengerEXT(vk.instance, vk.debugMessenger, vk.pAllocator);
#endif

    vkDestroyInstance(vk.instance, vk.pAllocator);
}

#endif

