#include <X11/Xlib.h>
#include <unistd.h> // sleep()

int
main(void)
{
	Display* display = XOpenDisplay(NULL);          // open connection

	Window window = XCreateSimpleWindow(display,
		DefaultRootWindow(display), 							    // parent window
		0, 0, 720, 480, 0, 													  // x, y, width, height, border width
		BlackPixel(display, DefaultScreen(display)),  // border color
		BlackPixel(display, DefaultScreen(display)));	// background color

	XMapWindow(display, window);                    // make new window visible
	XFlush(display);															  // send all data to X11 server

	sleep(2);																			  // wait for 2 sec

	XUnmapWindow(display, window);                  // close window, connection and free resources
	XDestroyWindow(display, window);
	XCloseDisplay(display);
}
