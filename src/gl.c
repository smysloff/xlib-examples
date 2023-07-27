#include "gl.h"
#include <X11/X.h>

GL_t GL;

static void GL_InitFPS(void)
{
  GL.fps.previous = GL.time.previous;
  GL.fps.value = 0;
  strcpy(GL.fps.text, "0");
}

static unsigned long GL_UpdateFPS(void)
{
  unsigned long delta_time;

  delta_time = GetDeltaTime(&GL.fps.previous, &GL.time.current);
  if (delta_time >= BILLION)
  {
    snprintf(GL.fps.text, sizeof(GL.fps.text), "%ld", GL.fps.value);
    GL.fps.value = 0;
    GL.fps.previous = GL.time.current;
  }

  return (GL.hidden ? 0 : delta_time);
}

static void GL_StartClock(void)
{
	clock_gettime(CLOCK_MONOTONIC, &GL.time.previous);
}

static unsigned long GL_UpdateClock(void)
{
  unsigned long delta_time;

	clock_gettime(CLOCK_MONOTONIC, &GL.time.current);
  delta_time = GetDeltaTime(&GL.time.previous, &GL.time.current);

  if (delta_time < GL.time.frame_time)
    return 0;

	GL.time.previous = GL.time.current;
	++GL.fps.value;

  return delta_time;
}

void GL_Init(void)
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

  GL_InitFPS();
  GL_SetWindowHidden();
	GL_StartLoop();
}

void GL_Quit(void)
{
	XFreeFont(GL.display, GL.font);
	XFreeGC(GL.display, GL.gc);
	XUnmapWindow(GL.display, GL.window);
	XDestroyWindow(GL.display, GL.window);
	XCloseDisplay(GL.display);
}

void GL_Loop(void (*UpdateState)(void), void (*RenderFrame)(void))
{
  GL_StartClock();

	while (GL.loop)
	{
		while (XPending(GL.display))
		{
			XNextEvent(GL.display, &GL.event);
			switch (GL.event.type)
			{
				case Expose: GL.ExposeHandler(); break;
				case KeyPress: GL.KeyPressHandler(GL.event.xkey.keycode); break;
				case KeyRelease: GL.KeyReleaseHandler(GL.event.xkey.keycode); break;
			}
		}

    if (!GL_UpdateClock()) continue;
    if (!GL_UpdateFPS()) continue;

    UpdateState();
		RenderFrame();
    GL_DrawFPS();
	}
}

void GL_StartLoop(void)
{
  GL.loop = 1;
}

void GL_StopLoop(void) {
  GL.loop = 0;
}

void GL_CreateWindow(
  unsigned short width, unsigned short height, GL_Color background_color)
{
	XSetWindowAttributes swa = {
		.background_pixel = background_color,
		.event_mask = ExposureMask
      | KeyPressMask | KeyReleaseMask
      | ButtonPressMask | ButtonReleaseMask
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

void GL_SetExposeHandler(void (*handler)(void))
{
  GL.ExposeHandler = handler;
}

void GL_SetKeyPressHandler(void (*handler)(unsigned keycode))
{
  GL.KeyPressHandler = handler;
}

void GL_SetKeyReleaseHandler(void (*handler)(unsigned keycode))
{
  GL.KeyReleaseHandler = handler;
}

void GL_SetFrameRate(unsigned long frame_rate)
{
  GL.time.frame_rate = frame_rate;
  GL.time.frame_time = BILLION / (frame_rate);
}

void GL_SetFont(const char* fontname)
{
	GL.font = XLoadQueryFont(GL.display, fontname);
	XSetFont(GL.display, GL.gc, GL.font->fid);
}

void GL_SetForeground(GL_Color color)
{
	XSetForeground(GL.display, GL.gc, color);
}

void GL_SetWindowTitle(const char* title)
{
  XStoreName(GL.display, GL.window, title);
}

void GL_ClearWindow(void)
{
  XClearWindow(GL.display, GL.window);
}

void GL_SetWindowVisible(void)
{
  GL.hidden = 0;
}

void GL_SetWindowHidden(void)
{
  GL.hidden = 1;
}

void GL_DrawText(const char* text, int x, int y)
{
	XDrawString(GL.display, GL.window, GL.gc, x, y, (text), strlen(text));
}

void GL_DrawFPS(void)
{
  GL_DrawText(GL.fps.text, 5, 20);
}

void GL_DrawLine(int x1, int y1, int x2, int y2)
{
  XDrawLine(GL.display, GL.window, GL.gc, x1, y1, x2, y2);
}

void GL_DrawLines(GL_Point* points, int count)
{
  XDrawLines(GL.display, GL.window, GL.gc, points, count, CoordModeOrigin);
}

void GL_DrawRectangle(int x, int y, unsigned width, unsigned height)
{
  XDrawRectangle(GL.display, GL.window, GL.gc, x, y, width, height);
}
