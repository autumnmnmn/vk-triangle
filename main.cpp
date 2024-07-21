#include <cstdlib>

#include "_glfw/window.h"
#include "_vk/vkstate.h"
#include "general/macros.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

void mainLoop(GLFWwindow *window, VkState &vk) {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame(vk);
    }
}

int main() {
    try {
        auto window = initWindow(WIDTH, HEIGHT);
        auto vk = initVulkan(window);

        mainLoop(window, vk);

        cleanup(vk);
        cleanup(window);
    } catch (const std::exception& e) {
        STDERR(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

