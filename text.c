#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
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
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 400, 200, 1, border, background);

    XftFont *font;
    XRenderColor color;
    XftDraw *draw;
    XftColor xftColor;

    XSelectInput(display, window, ExposureMask);
    XMapWindow(display, window);
    
    draw = XftDrawCreate(display, window, DefaultVisual(display, screen), DefaultColormap(display, screen));
    if (!draw) {
        fprintf(stderr, "Failed to create Xft draw object\n");
        return 1;
    }

    font = XftFontOpenName(display, screen, "monospace");
    if (!font) {
        fprintf(stderr, "Failed to open font\n");
        return 1;
    }

    color.red = 0;
    color.green = 65535;
    color.blue = 0;
    color.alpha = 65535;

    XftColorAllocValue(display, DefaultVisual(display, screen), DefaultColormap(display, screen), &color, &xftColor);
    XftDrawString8(draw, &xftColor, font, 10, 50, (XftChar8 *)"Hello, Overlay!", 15);

    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            XftDrawString8(draw, &xftColor, font, 10, 50, (XftChar8 *)"Hello, Overlay!", 15);
        }
    }

    XftColorFree(display, DefaultVisual(display, screen), DefaultColormap(display, screen), &xftColor);
    XftDrawDestroy(draw);
    XftFontClose(display, font);
    XCloseDisplay(display);
    return 0;
}

