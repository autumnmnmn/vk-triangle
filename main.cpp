#include <cstdlib>
#include <chrono>
#include <thread>

#include "_vk/vkstate.h"
#include "general/macros.h"
#include "general/math.h"

const math::vec2u windowDimensions = { 500, 500 };

void mainLoop(window::Window window, VkState &vk) {
    while (!window::shouldClose(window)) {
        window::pollEvents(window);
        //STDOUT("transp: " << glfwGetWindowAttrib(window, GLFW_TRANSPARENT_FRAMEBUFFER));
        drawFrame(vk);
    }
}

int main() {
    try {
        enum window::Implementation implementation = window::X11;

        window::Window window = window::get(implementation, windowDimensions);
        auto vk = initVulkan(window);

        mainLoop(window, vk);
        //while (!window::shouldClose(window)) window::pollEvents(window);

        cleanup(vk);
        window::dispose(window);
    } catch (const std::exception& e) {
        STDERR(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


