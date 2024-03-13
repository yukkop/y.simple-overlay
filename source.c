#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

int main() {
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    XVisualInfo vinfo;
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);

    int screen = DefaultScreen(display);

    XSetWindowAttributes attributes;
    attributes.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
    attributes.border_pixel = 0;
    attributes.background_pixel = 0;
    attributes.override_redirect = True;

    Window window = XCreateWindow(display, RootWindow(display, screen), 10, 10, 100, 100, 0, vinfo.depth, InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect, &attributes);



    XMapWindow(display, window);
    XStoreName(display, window, "Overlay");

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == KeyPress) break;
    }

    XCloseDisplay(display);
    return 0;
}
