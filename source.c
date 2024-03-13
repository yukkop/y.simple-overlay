#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Display *dpy;
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

    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);

    if (!XMatchVisualInfo(dpy, screen, 32, TrueColor, &vinfo)) {
        fprintf(stderr, "No matching visual\n");
        exit(1);
    }

    visual = vinfo.visual;
    cmap = XCreateColormap(dpy, root, visual, AllocNone);

    attr.colormap = cmap;
    attr.border_pixel = 0;
    attr.background_pixel = 0;
    attr.override_redirect = True;

    Window win = XCreateWindow(dpy, root, 0, 0, 400, 200, 0,
                                vinfo.depth, InputOutput, visual,
                                CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect, &attr);

    XSelectInput(dpy, win, ExposureMask | KeyPressMask);
    XMapWindow(dpy, win);

    gc = XCreateGC(dpy, win, 0, NULL);
    font = XLoadQueryFont(dpy, "10x20");
    XSetFont(dpy, gc, font->fid);

    while (1) {
        XNextEvent(dpy, &e);
        if (e.type == Expose) {
            XSetForeground(dpy, gc, 0xffffff); // Set text color to white
            XDrawString(dpy, win, gc, 10, 50, hello, strlen(hello));
        }
        if (e.type == KeyPress)
            break;
    }

    XCloseDisplay(dpy);

    return 0;
}

