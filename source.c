#include <X11/Xft/Xft.h>
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
  int screen = DefaultScreen(display);

  XMatchVisualInfo(display, screen, 32, TrueColor, &vinfo);

  unsigned long border = BlackPixel(display, screen);
  unsigned long background = WhitePixel(display, screen);
  Window root = RootWindow(display, screen);

  XSetWindowAttributes attributes;
  attributes.colormap = XCreateColormap(display, root,
                                        vinfo.visual, AllocNone);
  attributes.border_pixel = 0;
  attributes.background_pixel = 0;
  attributes.override_redirect = True;


   Window window = XCreateWindow(display, root, 10, 10, 100, 100, 0, vinfo.depth,
                              InputOutput, vinfo.visual,
                              CWColormap | CWBorderPixel | CWBackPixel |
                                  CWOverrideRedirect,
                              &attributes);

  XMapWindow(display, window);
  XStoreName(display, window, "Overlay");

  XSelectInput(display, window, ExposureMask | KeyPressMask);

  XftFont *font;
  XRenderColor color;
  XftDraw *draw;
  XftColor xftColor;

  XSelectInput(display, window, ExposureMask);
  XMapWindow(display, window);

  draw = XftDrawCreate(display, window, DefaultVisual(display, screen),
                       DefaultColormap(display, screen));
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

  XftColorAllocValue(display, DefaultVisual(display, screen),
                     DefaultColormap(display, screen), &color, &xftColor);
  XftDrawString8(draw, &xftColor, font, 10, 50, (XftChar8 *)"Hello, Overlay!",
               15);

  XEvent event;
  while (1) {
    XNextEvent(display, &event);
    if (event.type == Expose)
      XftDrawString8(draw, &xftColor, font, 10, 50,
                     (XftChar8 *)"Hello, Overlay!", 15);

    if (event.type == KeyPress)
      break;
  }

  XftColorFree(display, DefaultVisual(display, screen),
               DefaultColormap(display, screen), &xftColor);
  XftDrawDestroy(draw);
  XftFontClose(display, font);
  XCloseDisplay(display);
  return 0;
}
