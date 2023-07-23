#include <X11/Xlib.h>

#ifdef __APPLE__
	#define KEY_ESCAPE 61
#else
	#define KEY_ESCAPE 9
#endif

int
main(void)
{
	Display* display = XOpenDisplay(NULL);
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
	
	XStoreName(display, window, "X Window Example"); 					// set window title

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
					210, 140, 300, 200);							// x, y, width, height
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
