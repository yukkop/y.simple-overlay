#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Display *display;
    Window root;
    XVisualInfo vinfo;
    XSetWindowAttributes attr;
    Visual *visual;
    Colormap cmap;
    XFontStruct *font;
    GC gc;
    XEvent e;
    int screen;

    char hello[] = "vas vzlomali";

    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    root = RootWindow(display, screen);

    if (!XMatchVisualInfo(display, screen, 32, TrueColor, &vinfo)) {
        fprintf(stderr, "No matching visual\n");
        exit(1);
    }

    visual = vinfo.visual;
    cmap = XCreateColormap(display, root, visual, AllocNone);

    attr.colormap = cmap;
    attr.border_pixel = 0;
    attr.background_pixel = 0;
    attr.override_redirect = True;

    Window win = XCreateWindow(display, root, 0, 0, 400, 200, 0,
                                vinfo.depth, InputOutput, visual,
                                CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect, &attr);

    XSelectInput(display, win, ExposureMask | KeyPressMask);
    XMapWindow(display, win);

    gc = XCreateGC(display, win, 0, NULL);
    font = XLoadQueryFont(display, "-");
    if (!font) {
      fprintf(stderr, "Failed to open font\n");
      return 1;
    }

    XSetFont(display, gc, font->fid);

    while (1) {
        XNextEvent(display, &e);
        if (e.type == Expose) {
            XSetForeground(display, gc, 0xffffff); // Set text color to white
            XDrawString(display, win, gc, 10, 50, hello, strlen(hello));
        }
        if (e.type == KeyPress)
            break;
    }

    XCloseDisplay(display);

    return 0;
}

