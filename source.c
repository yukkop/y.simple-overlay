#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char* text;
  int len;
} String;

String empty_string() {
  String string;
  string.text = "";
  string.len = 0;
  return string;
}

String all_args(int argc, char *argv[]) {
    // Calculate the total length needed for the concatenated string
    int totalLength = 0;
    for (int i = 1; i < argc; i++) {
        totalLength += strlen(argv[i]) + 1; // +1 for space or null terminator
    }

    if (totalLength == 0) {
        printf("Warn! No arguments to concatenate.\n");
        return empty_string();
    }

    // Allocate memory for the concatenated string
    char *allArgs = malloc(totalLength * sizeof(char));
    if (allArgs == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    allArgs[0] = '\0'; // Initialize the string with a null terminator

    // Concatenate all arguments
    for (int i = 1; i < argc; i++) {
        strcat(allArgs, argv[i]);
        if (i < argc - 1) {
            strcat(allArgs, " "); // Add a space between arguments, not after the last one
        }
    }

    String string;
    string.text = allArgs;
    string.len = totalLength;
    return string;
}

int main(int argc, char *argv[]) {
    String args = all_args(argc, argv);

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
    font = XLoadQueryFont(display, "-adobe-helvetica-medium-r-normal-*-12-*-*-*-*-*-*-*");
    if (!font) {
      fprintf(stderr, "Failed to open font\n");
      return 1;
    }

    XSetFont(display, gc, font->fid);

    while (1) {
        XNextEvent(display, &e);
        if (e.type == Expose) {
            XSetForeground(display, gc, 0xffffff); // Set text color to white
            XDrawString(display, win, gc, 10, 50, args.text, args.len - 1);
        }
        if (e.type == KeyPress)
            break;
    }

    XCloseDisplay(display);

    return 0;
}
