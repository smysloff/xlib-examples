// ==================================================================== //
//  _   _ _    ___ ___       ____ _   _      _   _ ___  _    ____  __   //
//   \ /  |     |  |__)      |__   \ /   /\  |\ /| |  \ |    |__  (__`  //
//    X   |     |  |  \      |      X   /__\ | V | |__/ |    |       \  //
//  _/ \_ |__/ _|_ |__/      |___ _/ \_ |  | |   | |    |__/ |___ \__/  //
//                                                                      //
// ==================================================================== //
// file:    gl.h                                                        //
// author:  Alexander Smyslov                                           //
// e-mail:  kokoc.smyslov@yandex.ru                                     //
// website: https://github.com/smysloff/xlib-examples                   //
// license: GPL-3.0                                                     //
// ==================================================================== //

#ifndef __GL_H__
#define __GL_H__

#include "gl_keys.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BILLION 1000000000

typedef struct timespec GL_Time;
typedef XPoint GL_Point;
typedef unsigned long GL_Color;

typedef struct GL_ColorMap
{
  GL_Color black;
  GL_Color white;
} GL_ColorMap;

typedef struct GL_Clock
{
  unsigned long frame_rate;
  unsigned long frame_time;
  GL_Time previous;
  GL_Time current;
} GL_Clock;

typedef struct GL_FPS
{
  GL_Time previous;
  unsigned long value;
  char text[32];
} GL_FPS;

typedef struct Atoms
{
  Atom delete_window;
} Atoms;

typedef struct GL_t
{
  Display* display;
  int screen;
  Window root;
  Window window;
  GC gc;
  XEvent event;
  XFontStruct* font;
  GL_Clock time;
  GL_FPS fps;
  GL_ColorMap color;
  unsigned loop;
  unsigned hidden;
  void (*ExposeHandler)(void);
  void (*KeyPressHandler)(XKeyEvent* event);
  void (*KeyReleaseHandler)(XKeyEvent* event);
  void (*ButtonPressHandler)(XButtonEvent* event);
  void (*ButtonReleaseHandler)(XButtonEvent* event);
  void (*UpdateState)(void);
  void (*RenderFrame)(void);
  Atoms atoms;
} GL_t;

extern GL_t GL;

#define GetSizeOfArray(array) \
  (sizeof(array) / sizeof(*(array)))

#define GetDeltaTime(prev_timespec_ptr, curr_timespec_ptr) \
  (((curr_timespec_ptr)->tv_sec - (prev_timespec_ptr)->tv_sec) * BILLION \
    + (curr_timespec_ptr)->tv_nsec - (prev_timespec_ptr)->tv_nsec)

#define NormalizeDegrees(angle) \
  ((angle < 0) ? ((angle) + 360) \
    : ((angle) >= 360) ? ((angle) - 360) \
      : (angle))

#define NormalizeRadians(angle) \
  ((angle < 0) ? ((angle) + M_2_PI) \
    : ((angle) >= M_2_PI) ? ((angle) - M_2_PI) \
      : (angle))

void GL_Init(void);
void GL_Quit(void);
void GL_Loop(void (*UpdateState)(void), void (*RenderFrame)(void));

void GL_StartLoop(void);
void GL_StopLoop(void);

void GL_CreateWindow(
  unsigned short width, unsigned short height, GL_Color background_color);

void GL_SetExposeHandler(void (*handler)(void));
void GL_SetKeyPressHandler(void (*handler)(XKeyEvent* event));
void GL_SetKeyReleaseHandler(void (*handler)(XKeyEvent* event));
void GL_SetButtonPressHandler(void (*handler)(XButtonEvent* event));
void GL_SetButtonReleaseHandler(void (*handler)(XButtonEvent* event));

void GL_SetFrameRate(unsigned long frame_rate);
void GL_SetFont(const char* fontname);
void GL_SetForeground(GL_Color color);
void GL_SetWindowTitle(const char* title);
void GL_ClearWindow(void);
void GL_SetWindowVisible(void);
void GL_SetWindowHidden(void);
void GL_SetWindowFixed(void);

void GL_DrawText(const char* text, int x, int y);
void GL_DrawFPS(void);

void GL_DrawLine(int x1, int y1, int x2, int y2);
void GL_DrawLines(GL_Point* points, int count);
void GL_DrawRectangle(int x, int y, unsigned width, unsigned height);
//void GL_DrawRectangles()
void GL_FillRectangle(int x, int y, unsigned width, unsigned height);

#endif
