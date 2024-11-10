#ifndef _GLFW_WINDOW
#define _GLFW_WINDOW

#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../../general/math.h"

namespace _glfw {
    GLFWwindow* getWindow(math::vec2u dimensions) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // tell it NO OPENGL
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DEPTH_BITS, GLFW_TRUE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        return glfwCreateWindow(dimensions.x, dimensions.y, "vulkan window", nullptr, nullptr);
    }

    void disposeWindow(GLFWwindow *window) {
        glfwDestroyWindow(window);

        glfwTerminate();
    }

    bool shouldClose(GLFWwindow *window) {
        return glfwWindowShouldClose(window);
    }

    void pollEvents(GLFWwindow *window) {
        glfwPollEvents();
    }

    math::vec2u getDimensions(GLFWwindow *window) {
        math::vec2i dimensions;
        glfwGetFramebufferSize(window, &dimensions.x, &dimensions.y);
        return static_cast<math::vec2u>(dimensions);
    }

    VkSurfaceKHR createSurface(GLFWwindow *window, VkInstance instance) {
        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
        return surface;
    }
}

#endif

