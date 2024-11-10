#ifndef _X11_WINDOW
#define _X11_WINDOW

#include <stdexcept>

#include <X11/Xlib.h>
#include "/usr/include/vulkan/vulkan.h"
#include "/usr/include/vulkan/vulkan_xlib.h"

#include "../../general/basics.h"
#include "../../general/macros.h"
#include "../../general/math.h"

namespace _x11 {
    struct XWindow {
        Window window;
        Display *display;
        vec<XEvent> events; // TODO should be a queue really
        Atom wmDeleteMessage;
        bool shouldClose;
    };

    const long EVENT_MASK = StructureNotifyMask | KeyPressMask | KeyReleaseMask;

    XWindow* getWindow(math::vec2u dimensions) {
        Display *display = XOpenDisplay(nullptr);

        if (display == nullptr) {
            throw std::runtime_error("failed to open X display");
        }

        int screen = DefaultScreen(display);

        Window window = XCreateSimpleWindow(display, RootWindow(display, screen),
            0, 0, // position
            dimensions.x, dimensions.y,
            1, BlackPixel(display, screen), // border
            BlackPixel(display, screen) // background
        );

        XMapWindow(display, window);

        XSelectInput(display, window, EVENT_MASK);

        XFlush(display);

        XWindow *ptr = (XWindow *) malloc(sizeof(XWindow));

        ptr->window = window;
        ptr->display = display;
        ptr->wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False); // TODO research
        ptr->shouldClose = false;
        ptr->events = vec<XEvent>(16);

        XSetWMProtocols(display, window, &ptr->wmDeleteMessage, 1); // TODO research


        return ptr;
    }

    math::vec2u getDimensions(XWindow *window) {
        return math::vec2u { .x = 0, .y = 0 };
    }


    void pollEvents(XWindow *window) {
        XEvent event;

        while (XCheckTypedWindowEvent(window->display, window->window, ClientMessage, &event)) {
            if (event.xclient.data.l[0] == window->wmDeleteMessage) {
                window->shouldClose = true;
            } else {
                window->events.push_back(event);
            }
        }

        while (XCheckWindowEvent(window->display, window->window, EVENT_MASK, &event)) {
            STDOUT("poll");
            window->events.push_back(event);
        }
    }

    void disposeWindow(XWindow *window) {
        XCloseDisplay(window->display);
        //delete window->events;
        free(window);
        //delete window;
    }

    VkSurfaceKHR createSurface(XWindow *window, VkInstance instance) {
        VkSurfaceKHR surface;

        xlibSurfaceInfo(createInfo,
            .flags = 0,
            .dpy = window->display,
            .window = window->window
        );

        if (vkCreateXlibSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
            THROW("failed to create window surface");
        }

        return surface;
    }
}

#endif

