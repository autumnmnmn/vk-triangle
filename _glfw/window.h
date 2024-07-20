#ifndef _GLFW_WINDOW
#define _GLFW_WINDOW

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

GLFWwindow* initWindow(uint32_t width, uint32_t height) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // tell it NO OPNEGL
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // NOT RIGHT NOW FFS
    return glfwCreateWindow(width, height, "vulkan window", nullptr, nullptr);
}

void cleanup(GLFWwindow *window) {
    glfwDestroyWindow(window);

    glfwTerminate();
}

VkSurfaceKHR createSurface(GLFWwindow *window, VkInstance instance) {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
    return surface;
}

#endif

