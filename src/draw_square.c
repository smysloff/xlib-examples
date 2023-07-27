#include "gl.h"

#include <math.h>

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 400
#define WINDOW_TITLE "X11 Window Example"

#define FRAME_RATE 60

#define RECT_POS_X WINDOW_WIDTH / 2
#define RECT_POS_Y WINDOW_HEIGHT / 2
#define RECT_WIDTH 300
#define RECT_HEIGHT 200
#define RECT_ROTATION_SPEED 1
#define RECT_ANGLE 0

typedef struct Controls_t
{
  unsigned left;
  unsigned right;
  unsigned space;
  unsigned any;
} Controls_t;

typedef struct Rect_t
{
  GL_Point center;
  unsigned width;
  unsigned height;
  double angle;
  double rotation_speed;
  unsigned stopped;
  GL_Time last_time;
  Controls_t controls;
} Rect_t;

static Rect_t rect = {
  .center = { RECT_POS_X, RECT_POS_Y },
  .width = RECT_WIDTH,
  .height = RECT_HEIGHT,
  .angle = RECT_ANGLE,
  .rotation_speed = RECT_ROTATION_SPEED,
};

static void Rect_GetVertexes(Rect_t* rect, GL_Point points[5]);

static void Rect_Draw(Rect_t* rect);

static void Rect_GetVertexes(Rect_t* rect, GL_Point points[5])
{
  GL_Point point;
	
  double radians =
    NormalizeDegrees(rect->angle - 45) * (M_PI / 180);

  for (register int i = 0; i < 4; ++i)
  {
    int hWidth = rect->width / 2;
    int hHeight = rect->width / 2;

    radians += M_PI_2;
    point.x = (int) (rect->center.x + hWidth * cos(radians));
    point.y = (int) (rect->center.y + hHeight * sin(radians));

    points[i] = point;
  }
  points[4] = points[0];
}

static void Rect_Draw(Rect_t* rect)
{
  GL_Point points[5];
  Rect_GetVertexes(rect, points);
  GL_DrawLines(points, GetSizeOfArray(points));
}

static void ExposeHandler(void)
{
  GL_SetWindowVisible();
}

static void KeyPressHandler(unsigned keycode)
{
  if (keycode == KEY_ESCAPE)
    GL.loop = 0;

  if (keycode == KEY_A || keycode == KEY_LEFT)
    rect.controls.left = 1;
  
  if (keycode == KEY_D || keycode == KEY_RIGHT)
    rect.controls.right = 1;

  if (keycode == KEY_SPACE)
    rect.controls.space = 1;

  if (rect.controls.left || rect.controls.right || rect.controls.space)
    rect.controls.any = 1;
}

static void KeyReleaseHandler(unsigned keycode)
{
  if (keycode == KEY_A || keycode == KEY_LEFT)
    rect.controls.left = 0;

  if (keycode == KEY_D || keycode == KEY_RIGHT)
    rect.controls.right = 0;

  if (keycode == KEY_SPACE)
    rect.controls.space = 0;

  if (!rect.controls.left && !rect.controls.right && !rect.controls.space)
    rect.controls.any = 0;
}

static void UpdateState(void)
{
  // handle keys
  if (rect.controls.left)
    rect.angle = NormalizeDegrees(rect.angle - rect.rotation_speed);
  
  if (rect.controls.right)
    rect.angle = NormalizeDegrees(rect.angle + rect.rotation_speed);

  // update rect state
  if (rect.controls.any)
  {
    rect.stopped = 1;
    rect.last_time = GL.time.current;
  }
  else
  {
    rect.stopped = 0;

    unsigned long delta_time =
      GetDeltaTime(&rect.last_time, &GL.time.current);

    if (delta_time >= (BILLION / 2)) // waiting time
    rect.angle = NormalizeDegrees(rect.angle - rect.rotation_speed / 2);
  }
}

static void RenderFrame(void)
{
  GL_ClearWindow();
  Rect_Draw(&rect);
}

int main(void)
{
  GL_Init();

  GL_CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GL.color.black);
  GL_SetFrameRate(FRAME_RATE);
  GL_SetWindowTitle(WINDOW_TITLE);
  GL_SetFont("10x20");

  GL_SetExposeHandler(ExposeHandler);
  GL_SetKeyPressHandler(KeyPressHandler);
  GL_SetKeyReleaseHandler(KeyReleaseHandler);

  GL_Loop(UpdateState, RenderFrame);

  GL_Quit();
}
