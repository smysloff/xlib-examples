#include "gl.h"

GL_t GL;

void
GL_Init(void)
{
	GL.display = XOpenDisplay(NULL);

	if (!GL.display)
	{
		fprintf(stderr, "error:"
			" Can't open connection to display server."
			" Probably X server is not started.");
		exit(1);
	}

	GL.screen = DefaultScreen(GL.display);
	GL.root = RootWindow(GL.display, GL.screen);

	GL.color.black = BlackPixel(GL.display, GL.screen);
	GL.color.white = WhitePixel(GL.display, GL.screen);

	GL.hidden = 1;
	GL.loop = 1;
}

void
GL_CreateWindow(
	const unsigned short width,
	const unsigned short height,
	const Color_t background_color
) {

	XSetWindowAttributes swa = {
		.background_pixel = background_color,
		.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask
	};

	GL.window = XCreateWindow(GL.display, GL.root,
		0, 0, width, height, 0,
		CopyFromParent,
		InputOutput,
		CopyFromParent,
		CWEventMask | CWBackPixel,
		&swa);

	GL.gc = XCreateGC(GL.display, GL.window, 0, NULL);

	(background_color == GL.color.black)
		? GL_SetForeground(GL.color.white)
		: GL_SetForeground(GL.color.black);

	XMapWindow(GL.display, GL.window);
}

void
GL_SetForeground(Color_t color)
{
	XSetForeground(GL.display, GL.gc, color);
}

void
GL_SetWindowTitle(const char* restrict title)
{
	XStoreName(GL.display, GL.window, title);
}

void
GL_Quit(void)
{
	XFreeGC(GL.display, GL.gc);
	XUnmapWindow(GL.display, GL.window);
	XDestroyWindow(GL.display, GL.window);
	XCloseDisplay(GL.display);
}

void
GL_ClearWindow(void)
{
	XClearWindow(GL.display, GL.window);
}

void
GL_DrawPoint(const int x, const int y)
{
	XDrawPoint(GL.display, GL.window, GL.gc, x, y);
}

void
GL_DrawPoints(Point_t* points, const int count)
{
	XDrawPoints(GL.display, GL.window, GL.gc,
		points, count, CoordModeOrigin);
}

void
GL_DrawLine(
	const int x1, const int y1,
	const int x2, const int y2
) {
	XDrawLine(GL.display, GL.window, GL.gc, x1, y1, x2, y2);
}

void
GL_DrawLines(Point_t* points, const int count)
{
	XDrawLines(GL.display, GL.window, GL.gc,
		points, count, CoordModeOrigin);
}

void
GL_DrawRectangle(
	const int x, const int y,
	const unsigned width, const unsigned height
) {
	XDrawRectangle(GL.display, GL.window, GL.gc,
		x, y, width, height);
}
