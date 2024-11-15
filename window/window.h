#ifndef WINDOW_WINDOW
#define WINDOW_WINDOW

#include "/usr/include/vulkan/vulkan.h"

#include "../_vk/macros.h"
#include "../general/macros.h"
#include "../general/math.h"
#include "_glfw/window.h"
#include "_x11/window.h"

namespace window {
    enum Implementation {
        X11,
        GLFW
    };

    struct Window {
        enum Implementation implementation;
        void *window;
    };

    Window get(enum Implementation implementation, math::vec2u dimensions) {
        void *window;

        switch (implementation) {
            case X11:
                window = (void*) _x11::getWindow(dimensions);
                break;
            case GLFW:
                window = (void*) _glfw::getWindow(dimensions);
                break;
            default:
                THROW("[get] bad window implementation");
        }

        return Window {
            .implementation = implementation,
            .window = window
        };
    }

    math::vec2u getDimensions(Window window) {
        switch (window.implementation) {
            case X11:
                return _x11::getDimensions((_x11::XWindow *) window.window);
            case GLFW:
                return _glfw::getDimensions((GLFWwindow *) window.window);
            default:
                THROW("[getDimensions] bad window implementation");
        }
    }

    void dispose(Window window) {
        switch (window.implementation) {
            case X11:
                _x11::disposeWindow((_x11::XWindow *) window.window);
                break;
            case GLFW:
                _glfw::disposeWindow((GLFWwindow *) window.window);
                break;
            default:
                THROW("[dispose] bad window implementation");
        }
    }

    bool shouldClose(Window window) {
        switch (window.implementation) {
            case X11:
                return ((_x11::XWindow *) window.window)->shouldClose;
            case GLFW:
                return _glfw::shouldClose((GLFWwindow *) window.window);
            default:
                THROW("[shouldClose] bad window implementation");
        }
    }

    void pollEvents(Window window) {
        switch (window.implementation) {
            case X11:
                _x11::pollEvents((_x11::XWindow *) window.window);
                break;
            case GLFW:
                _glfw::pollEvents((GLFWwindow *) window.window);
                break;
            default:
                THROW("[pollEvents] bad window implementation");
        }
    }

    VkSurfaceKHR createSurface(Window window, VkInstance instance) {
        switch (window.implementation) {
            case X11:
                return _x11::createSurface((_x11::XWindow *) window.window, instance);
            case GLFW:
                return _glfw::createSurface((GLFWwindow *) window.window, instance);
            default:
                THROW("[createSurface] bad window implementation");
        }
    }
}

#endif

