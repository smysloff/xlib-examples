#ifndef __GL_H__
#define __GL_H__

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
	#define KEY_ESCAPE 61
	#define KEY_Q      20
	#define KEY_W      21
	#define KEY_E      22
	#define KEY_A      8
	#define KEY_S      9
	#define KEY_D      10
	#define KEY_UP     134
	#define KEY_LEFT   131
	#define KEY_DOWN   133
	#define KEY_RIGHT  132
	#define KEY_SPACE  57
	#define KEY_RETURN 44
#else
	#define KEY_ESCAPE 9
	#define KEY_Q      24
	#define KEY_W      25
	#define KEY_E      26
	#define KEY_A      38
	#define KEY_S      39
	#define KEY_D      40
	#define KEY_UP     111
	#define KEY_LEFT   113
	#define KEY_DOWN   116
	#define KEY_RIGHT  114
	#define KEY_SPACE  65
	#define KEY_RETURN 36
#endif

#define GL_GetSizeOfArray(array) \
	(sizeof(array) / sizeof(*array))

typedef unsigned long Color_t;
typedef XPoint Point_t;

typedef struct ColorMap_t
{
	Color_t black;
	Color_t white;
} ColorMap_t;

typedef struct GL_t
{
	Display* display;
	int screen;
	Window root;
	Window window;
	GC gc;
	XEvent event;
	ColorMap_t color;
	int loop;
	int hidden;
} GL_t;

extern GL_t GL;

void
GL_Init(void);

void
GL_Quit(void);

void
GL_CreateWindow(
	const unsigned short width,
	const unsigned short height,
	const Color_t background_color
);

void
GL_SetForeground(const Color_t color);

void
GL_SetWindowTitle(const char* restrict title);

void
GL_ClearWindow(void);

void
GL_DrawPoint(const int x, const int y);

void
GL_DrawPoints(Point_t* points, const int count);

void
GL_DrawLine(
	const int x1, const int y1,
	const int x2, const int y2
);

void
GL_DrawLines(Point_t* points, const int count);

#endif
