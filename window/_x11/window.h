#ifndef _X11_WINDOW
#define _X11_WINDOW

#include <cstring>
#include <stdexcept>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
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

        XVisualInfo visualInfo;

        int status = XMatchVisualInfo(display, screen, 32, TrueColor, &visualInfo);

        std::cout << status << std::endl;

        std::cout << visualInfo.bits_per_rgb << std::endl;



        XSetWindowAttributes attributes;
        attributes.colormap = XCreateColormap(display, RootWindow(display, screen), visualInfo.visual, AllocNone);
        attributes.border_pixel = 0;
        attributes.background_pixel = 0x0;
        attributes.background_pixmap = None;

        Window window = XCreateWindow(display, RootWindow(display, screen),
            0, 0, // position
            dimensions.x, dimensions.y,
            1, visualInfo.depth, InputOutput, visualInfo.visual, CWColormap | CWBorderPixel | CWBackPixel | CWBackPixmap, &attributes
            //BlackPixel(display, screen), // border
            //BlackPixel(display, screen) // background
        );

        XStoreName(display, window, "Triangle :)");

        XMapWindow(display, window);

        XSelectInput(display, window, EVENT_MASK);

        XFlush(display);

        XWindow *ptr = (XWindow *) malloc(sizeof(XWindow));

        ptr->window = window;
        ptr->display = display;
        ptr->wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False); // TODO research
        ptr->shouldClose = false;
        new (&ptr->events) std::vector<XEvent>();

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
            window->events.push_back(event);
            STDOUT(window->events.size());
        }
    }

    void disposeWindow(XWindow *window) {
        XDestroyWindow(window->display, window->window);
        XCloseDisplay(window->display);
        //delete window->events;
        window->events.~vector<XEvent>();
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

