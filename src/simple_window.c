// ==================================================================== //
//  _   _ _    ___ ___       ____ _   _      _   _ ___  _    ____  __   //
//   \ /  |     |  |__)      |__   \ /   /\  |\ /| |  \ |    |__  (__`  //
//    X   |     |  |  \      |      X   /__\ | V | |__/ |    |       \  //
//  _/ \_ |__/ _|_ |__/      |___ _/ \_ |  | |   | |    |__/ |___ \__/  //
//                                                                      //
// ==================================================================== //
// file:    simple_window.c                                             //
// author:  Alexander Smyslov                                           //
// e-mail:  kokoc.smyslov@yandex.ru                                     //
// website: https://github.com/smysloff/xlib-examples                   //
// license: GPL-3.0                                                     //
// ==================================================================== //

#include <X11/Xlib.h>
#include <unistd.h> // sleep()
#include <stdio.h>  // fprintf()

int main(void)
{
  Display* display = XOpenDisplay(NULL);          // open connection

  if (!display)
  {
    fprintf(stderr, "error:"
      "	Can't open connection to display server."
      " Probably X server is not started.\n");
    return 1;
  }

  Window window = XCreateSimpleWindow(display,
    DefaultRootWindow(display),                   // parent window
    0, 0, 720, 480, 0,                            // x, y, width, height, border width
    BlackPixel(display, DefaultScreen(display)),  // border color
    BlackPixel(display, DefaultScreen(display))); // background color

  XMapWindow(display, window);                    // make new window visible
  XFlush(display);                                // send all data to X11 server

  sleep(2);                                       // wait for 2 sec

  XUnmapWindow(display, window);                  // close window, connection and free resources
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}
