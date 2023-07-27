// ==================================================================== //
//  _   _ _    ___ ___       ____ _   _      _   _ ___  _    ____  __   //
//   \ /  |     |  |__)      |__   \ /   /\  |\ /| |  \ |    |__  (__`  //
//    X   |     |  |  \      |      X   /__\ | V | |__/ |    |       \  //
//  _/ \_ |__/ _|_ |__/      |___ _/ \_ |  | |   | |    |__/ |___ \__/  //
//                                                                      //
// ==================================================================== //
// file:    create_window.c                                             //
// author:  Alexander Smyslov                                           //
// e-mail:  kokoc.smyslov@yandex.ru                                     //
// website: https://github.com/smysloff/xlib-examples                   //
// license: GPL-3.0                                                     //
// ==================================================================== //

#include <X11/Xlib.h>
#include <stdio.h>

#ifdef __APPLE__
  #define KEY_ESCAPE 61
#else
  #define KEY_ESCAPE 9
#endif

int main(void)
{
  Display* display = XOpenDisplay(NULL);

  if (!display)
  {
    fprintf(stderr, "error:"
      "	Can't open connection to display server."
      " Probably X server is not started.\n");
    return 1;
  }

  int screen = DefaultScreen(display);

  XSetWindowAttributes attributes = {
    .background_pixel = BlackPixel(display, screen),
    .event_mask = ExposureMask | KeyPressMask
  };

  Window window = XCreateWindow(display,
    RootWindow(display, screen),
    0, 0, 720, 480, 0,
    DefaultDepth(display, screen),
    InputOutput,
    DefaultVisual(display, screen),
    CWBackPixel | CWEventMask, // same as attributes
    &attributes);

  XStoreName(display, window, "X Window Example");          // set window title

  GC gc = XCreateGC(display, window, 0, NULL);              // create graphic context
  XSetForeground(display, gc, WhitePixel(display, screen)); // set foreground color for gc

  Atom wm_delete_window = XInternAtom(display,              // create event handler
    "WM_DELETE_WINDOW", False);                             // for closing window
  XSetWMProtocols(display, window, &wm_delete_window, 1);   // in window manager

  XMapWindow(display, window);

  XEvent event;

  for (int loop = 1; loop; )
  {
    XNextEvent(display, &event);

    switch (event.type)
    {
      case Expose:
      {
        XDrawRectangle(display, window, gc, // draw rect
          210, 140, 300, 200);              // x, y, width, height
        break;
      }

      case KeyPress:
      {
        if (event.xkey.keycode == KEY_ESCAPE)
          loop = 0;
        break;
      }

      case ClientMessage:                                       // handle event
      {                                                         // of closing window
        if ((Atom) event.xclient.data.l[0] == wm_delete_window) // in window manager
          loop = 0;
        break;
      }
    }

  }

  XFreeGC(display, gc);
  XUnmapWindow(display, window);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}
