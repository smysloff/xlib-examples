// ==================================================================== //
//  _   _ _    ___ ___       ____ _   _      _   _ ___  _    ____  __   //
//   \ /  |     |  |__)      |__   \ /   /\  |\ /| |  \ |    |__  (__`  //
//    X   |     |  |  \      |      X   /__\ | V | |__/ |    |       \  //
//  _/ \_ |__/ _|_ |__/      |___ _/ \_ |  | |   | |    |__/ |___ \__/  //
//                                                                      //
// ==================================================================== //
// file:    event_handling.c                                            //
// author:  Alexander Smyslov                                           //
// e-mail:  kokoc.smyslov@yandex.ru                                     //
// website: https://github.com/smysloff/xlib-examples                   //
// license: GPL-3.0                                                     //
// ==================================================================== //

#include <X11/Xlib.h>
#include <stdio.h> // fprintf() printf() puts()

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

  Window window = XCreateSimpleWindow(display,
    DefaultRootWindow(display),
    0, 0, 720, 480, 0,
    BlackPixel(display, DefaultScreen(display)),
    BlackPixel(display, DefaultScreen(display)));

  long event_mask = ExposureMask | KeyPressMask; // select events
  XSelectInput(display, window, event_mask);     // for handling

  XMapWindow(display, window);
	
  XEvent event;

  for (int loop = 1; loop; )
  {
    XNextEvent(display, &event);

    switch (event.type)
    {
      case Expose:
      {
        puts("window expose");
        break;
      }

      case KeyPress:
      {
        printf("key: %d\n", event.xkey.keycode);
        if (event.xkey.keycode == KEY_ESCAPE)
          loop = 0;
        break;
      }

    }
  }

  puts("window close");

  XUnmapWindow(display, window);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}
