// ==================================================================== //
//  _   _ _    ___ ___       ____ _   _      _   _ ___  _    ____  __   //
//   \ /  |     |  |__)      |__   \ /   /\  |\ /| |  \ |    |__  (__`  //
//    X   |     |  |  \      |      X   /__\ | V | |__/ |    |       \  //
//  _/ \_ |__/ _|_ |__/      |___ _/ \_ |  | |   | |    |__/ |___ \__/  //
//                                                                      //
// ==================================================================== //
// file:    dodger_game.c                                               //
// author:  Alexander Smyslov                                           //
// e-mail:  kokoc.smyslov@yandex.ru                                     //
// website: https://github.com/smysloff/xlib-examples                   //
// license: GPL-3.0                                                     //
// ==================================================================== //

#include "gl.h"
#include <time.h>

const unsigned short WINDOW_WIDTH  = 721;
const unsigned short WINDOW_HEIGHT = 401;
const char* restrict WINDOW_TITLE  = "Battle Town";
const unsigned long  FRAME_RATE    = 60;
const int            ENTITY_SIZE   = 20;


typedef struct Controls
{
  unsigned up;
  unsigned down;
  unsigned left;
  unsigned right;
} Controls;

typedef struct Soldier
{
  int x, y;
  int size;
  int speed; // double ?
  GL_Color color;
  int mov_x, mov_y;
  unsigned stop;
} Soldier;

static Controls controls;
static Soldier player;
static Soldier enemies[8];

static void Grid_draw(GL_Color color, unsigned cell_size)
{
  GL_SetForeground(color);

  for (register unsigned x = 0; x < WINDOW_WIDTH; x += cell_size)
    GL_DrawLine(x, 0, x, WINDOW_HEIGHT - 1);

  for (register unsigned y = 0; y < WINDOW_HEIGHT; y += cell_size)
    GL_DrawLine(0, y, WINDOW_WIDTH - 1, y);
  
  GL_SetForeground(GL.color.white);
}

static void Soldier_draw(Soldier* soldier)
{
  int hSize = soldier->size / 2;
  GL_SetForeground(soldier->color);
  GL_FillRectangle(
    soldier->x - hSize, soldier->y - hSize,
    soldier->size, soldier->size);
  GL_SetForeground(GL.color.white);
}

static void Player_init()
{
  player.x = WINDOW_WIDTH / 2;
  player.y = WINDOW_HEIGHT / 2;
  player.size = ENTITY_SIZE;
  player.speed = 2;
  player.color = 0xFF11EE11; // => 0x11EE11 or "#11ee11" ?
}

static void Enemies_init()
{
  GL_Point positions[8] = {
    { ENTITY_SIZE, ENTITY_SIZE },
    { (WINDOW_WIDTH - 1) - ENTITY_SIZE, ENTITY_SIZE },
    { (WINDOW_WIDTH - 1) - ENTITY_SIZE, (WINDOW_HEIGHT - 1) - ENTITY_SIZE },
    { ENTITY_SIZE, (WINDOW_HEIGHT - 1) - ENTITY_SIZE },

    { (WINDOW_WIDTH - 1) / 2, ENTITY_SIZE },
    { (WINDOW_WIDTH - 1) - ENTITY_SIZE, (WINDOW_HEIGHT - 1) / 2 },
    { (WINDOW_WIDTH - 1) / 2, (WINDOW_HEIGHT - 1) - ENTITY_SIZE },
    { ENTITY_SIZE, (WINDOW_HEIGHT - 1) / 2 }

  };

  size_t count = GetSizeOfArray(enemies);

  for (register size_t i = 0; i < count; ++i)
  {
    enemies[i].size = 20;
    enemies[i].speed = 1;
    enemies[i].color = 0xFF3333AA;
    enemies[i].x = positions[i].x;
    enemies[i].y = positions[i].y;
    enemies[i].mov_x = positions[i].x;
    enemies[i].mov_y = positions[i].y;
  }
}

static void Player_update()
{
  if (player.stop) return;

  int hSize = player.size / 2;

  if (controls.left) player.x -= player.speed;
  if (controls.right) player.x += player.speed;

  if (controls.up) player.y -= player.speed;
  if (controls.down) player.y += player.speed;

  if (player.x - hSize < 0) player.x = hSize;
  if (player.x + hSize > WINDOW_WIDTH - 1) player.x = WINDOW_WIDTH - 1 - hSize;

  if (player.y - hSize < 0) player.y = hSize;
  if (player.y + hSize > WINDOW_HEIGHT - 1) player.y = WINDOW_HEIGHT - 1 - hSize;

}

static void Enemies_update()
{
  size_t count = GetSizeOfArray(enemies);
  int dist = enemies[0].size * 10;

  for (register size_t i = 0; i < count; ++i)
  {
    Soldier* e = enemies + i;

    if (
      (abs(player.x - e->x) < e->size)
      && (abs(player.y - e->y) < e->size)
    ) {
      player.stop = 1;
    }

    if (
      (abs(player.x - e->x) < dist)
      && (abs(player.y - e->y) < dist)
    ) {
      e->mov_x = player.x;
      e->mov_y = player.y;
      e->color = 0xFFFF0000;
    }

    if (e->x == e->mov_x && e->y == e->mov_y)
    {
      e->mov_x = (rand() % (WINDOW_WIDTH - e->size * 2)) + e->size;
      e->mov_y = (rand() % (WINDOW_HEIGHT - e->size * 2)) + e->size;
      e->color = 0xFF3333AA;
    }

    if (e->x < e->mov_x) e->x += e->speed;
    if (e->x > e->mov_x) e->x -= e->speed;

    if (e->y < e->mov_y) e->y += e->speed;
    if (e->y > e->mov_y) e->y -= e->speed;
  }
}

static void Player_draw()
{
  Soldier_draw(&player);
}

static void Enemies_draw()
{
  size_t count = GetSizeOfArray(enemies);
  
  for (register size_t i = 0; i < count; ++i)
    Soldier_draw(enemies + i);
}

static void ExposeHandler(void)
{
  GL_SetWindowVisible();
}

static void KeyPressHandler(XKeyEvent* event)
{
  switch (event->keycode)
  {
    case KEY_ESCAPE:
      GL_StopLoop(); break;
    
    case KEY_W:
    case KEY_UP:
      controls.up = 1; break;

    case KEY_A:
    case KEY_LEFT:
      controls.left = 1; break;

    case KEY_S:
    case KEY_DOWN:
      controls.down = 1; break;

    case KEY_D:
    case KEY_RIGHT:
      controls.right = 1; break;
  }
}

static void KeyReleaseHandler(XKeyEvent* event)
{
  switch (event->keycode)
  {
    case KEY_W:
    case KEY_UP:
      controls.up = 0; break;

    case KEY_A:
    case KEY_LEFT:
      controls.left = 0; break;

    case KEY_S:
    case KEY_DOWN:
      controls.down = 0; break;

    case KEY_D:
    case KEY_RIGHT:
      controls.right = 0; break;
  }
}


static void ButtonPressHandler(XButtonEvent* event)
{
  printf("button %d (%d %d)\n", event->button, event->x, event->y);
}

static void ButtonReleaseHandler(XButtonEvent* event)
{
  (void) event;
  //printf("button %d (%d %d)\n", event->button, event->x, event->y);
}

static void UpdateState(void)
{
  Player_update();
  Enemies_update();
}

static void RenderFrame(void)
{
  Grid_draw(0xFF333333, 20);
  Player_draw();
  Enemies_draw();
}

int main(void)
{
  srand(time(NULL));

  Player_init();
  Enemies_init();

  GL_Init();
  
  GL_CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GL.color.black);
  GL_SetWindowTitle(WINDOW_TITLE);
  GL_SetFrameRate(FRAME_RATE);
  GL_SetFont("10x20");

  GL_SetExposeHandler(ExposeHandler);
  GL_SetKeyPressHandler(KeyPressHandler);
  GL_SetKeyReleaseHandler(KeyReleaseHandler);
  GL_SetButtonPressHandler(ButtonPressHandler);
  GL_SetButtonReleaseHandler(ButtonReleaseHandler);

  GL_Loop(UpdateState, RenderFrame);

  GL_Quit();
}
