#include <X11/Xlib.h>
#include <stdio.h>

int main() {
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    unsigned long border = BlackPixel(display, screen);
    unsigned long background = WhitePixel(display, screen);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 100, 100, 1, border, background);

    XSetWindowAttributes attributes;
    attributes.override_redirect = True;
    XChangeWindowAttributes(display, window, CWOverrideRedirect, &attributes);

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
