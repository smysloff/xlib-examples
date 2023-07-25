#include "gl.h"
#include <math.h>
#include <time.h>
#include <string.h>

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 400
#define WINDOW_TITLE "X11 Window Example"

#define NANOSECONDS 1000000000
#define FRAME_RATE 144
#define FRAME_TIME NANOSECONDS / FRAME_RATE

#define RECT_X WINDOW_WIDTH / 2
#define RECT_Y WINDOW_HEIGHT / 2
#define RECT_SIZE 200
#define RECT_SPEED .5

typedef struct Controls_t
{
	int left;
	int right;
	int space;
	int any;
} Controls_t;

Controls_t Controls = {
	.left = 0,
	.right = 0,
	.space = 0
};

typedef struct Rect_t
{
	Point_t center;
	int size;
	double angle;
	double speed;
	int stopped;
	struct timespec last_time;
} Rect_t;

static Rect_t rect = {
	.center = { RECT_X, RECT_Y },
	.size = RECT_SIZE,
	.angle = 0,
	.speed = RECT_SPEED
};

double
NormalizeAngle(const double angle)
{
	if (angle < 0) return (angle + 360);
	if (angle >= 360) return (angle - 360);
	return angle;
}

void
Rect_GetVertexes(Rect_t* rect, Point_t points[5])
{
	double radians =
		NormalizeAngle(rect->angle - 45) * (M_PI / 180);

	for (register int i = 0; i < 4; ++i)
	{
		Point_t point;
		int hSize = rect->size / 2;

		radians += M_PI_2;
		point.x = (int) (rect->center.x + hSize * cos(radians));
		point.y = (int) (rect->center.y + hSize * sin(radians));

		points[i] = point;
	}
	points[4] = points[0];
}

void
Rect_Draw(Rect_t* rect)
{
	Point_t points[5];
	Rect_GetVertexes(rect, points);
	GL_DrawLines(points, GL_GetSizeOfArray(points));
}

unsigned long
GetDeltaTime(
	struct timespec* prev,
	struct timespec* curr
) {
	return (curr->tv_sec - prev->tv_sec) * 1000000000 
		+ (curr->tv_nsec - prev->tv_nsec);
}

void
KeyPressHandler(unsigned int keycode)
{
	if (keycode == KEY_ESCAPE)
		GL.loop = 0;

	if (keycode == KEY_A || keycode == KEY_LEFT)
		Controls.left = 1;

	if (keycode == KEY_D || keycode == KEY_RIGHT)
		Controls.right = 1;

	if (keycode == KEY_SPACE)
		Controls.space = 1;

	if (Controls.left || Controls.right || Controls.space)
		Controls.any = 1;
}

void
KeyReleaseHandler(unsigned int keycode)
{
	if (keycode == KEY_A || keycode == KEY_LEFT)
		Controls.left = 0;

	if (keycode == KEY_D || keycode == KEY_RIGHT)
		Controls.right = 0;

	if (keycode == KEY_SPACE)
		Controls.space = 0;

	if (!Controls.left && !Controls.right && !Controls.space)
		Controls.any = 0;
}

void
RenderFrame(void)
{
	GL_ClearWindow();
	Rect_Draw(&rect);
}

int
main(void)
{
	GL_Init();
	GL_CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GL.color.black);
	GL_SetWindowTitle(WINDOW_TITLE);

	long fps = -1;
	char text[20] = { '0', 0 };
	XFontStruct* font;
	unsigned long delta_time = 0;
	struct timespec prev_time, curr_time, last_time;

	font = XLoadQueryFont(GL.display, "10x20");
	XSetFont(GL.display, GL.gc, font->fid);

	clock_gettime(CLOCK_MONOTONIC, &prev_time);
	last_time = prev_time;

	while (GL.loop)
	{
		while (XPending(GL.display))
		{
			XNextEvent(GL.display, &GL.event);

			switch (GL.event.type)
			{
				case Expose: GL.hidden = 0;
					break;

				case KeyPress: KeyPressHandler(GL.event.xkey.keycode);
					break;

				case KeyRelease: KeyReleaseHandler(GL.event.xkey.keycode);
					break;
			}
		}

		delta_time = GetDeltaTime(&prev_time, &curr_time);

		// new frame
		clock_gettime(CLOCK_MONOTONIC, &curr_time);

		if (delta_time < FRAME_TIME) continue;

		prev_time = curr_time;
		++fps;

		// new second
		delta_time = GetDeltaTime(&last_time, &curr_time);
		if (delta_time >= NANOSECONDS)
		{
			snprintf(text, sizeof(text), "%ld", fps);
			fps = 0;
			last_time = curr_time;
		}

		// check if window showed
		if (GL.hidden) continue;


		// handle keys
		if (Controls.left)
			rect.angle = NormalizeAngle(rect.angle - rect.speed);
		
		if (Controls.right)
			rect.angle = NormalizeAngle(rect.angle + rect.speed);

		// update rect state
		if (Controls.any)
		{
			rect.stopped = 1;
			rect.last_time = curr_time;
		}
		else
		{
			rect.stopped = 0;
			delta_time = GetDeltaTime(&rect.last_time, &curr_time);
			if (delta_time >= (NANOSECONDS / 2))
				rect.angle = NormalizeAngle(rect.angle - rect.speed / 2);
		}
		
		// draw frame
		RenderFrame();
		XDrawString(GL.display, GL.window, GL.gc,
			5, 20, text, strlen(text));
	}

	XFreeFont(GL.display, font);
	GL_Quit();
}
