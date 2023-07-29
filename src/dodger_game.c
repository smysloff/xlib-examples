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

#define WINDOW_TITLE  "Battle Town"
#define WINDOW_WIDTH  721
#define WINDOW_HEIGHT 401
#define FRAME_RATE    60

#define ENTITY_SIZE   20
#define PLAYER_SPEED  2
#define ENEMY_SPEED   1
#define AGGRO_SIZE    8

#define COLOR_RED     0xFFFF0000
#define COLOR_GREEN   0xFF11EE11
#define COLOR_BLUE    0xFF3333AA
#define COLOR_GRAY    0xFF333333

const unsigned short INNER_WIDTH  = WINDOW_WIDTH - 1;
const unsigned short INNER_HEIGHT = WINDOW_HEIGHT - 1;
const unsigned short HALF_WIDTH   = INNER_WIDTH / 2;
const unsigned short HALF_HEIGHT  = INNER_HEIGHT / 2;
const int            HALF_SIZE    = ENTITY_SIZE / 2;
const unsigned short AGGRO_RADIUS = ENTITY_SIZE * AGGRO_SIZE;

typedef struct Controls
{
  unsigned up;
  unsigned down;
  unsigned left;
  unsigned right;
  unsigned any;
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
    GL_DrawLine(x, 0, x, INNER_HEIGHT);

  for (register unsigned y = 0; y < WINDOW_HEIGHT; y += cell_size)
    GL_DrawLine(0, y, INNER_WIDTH, y);
  
  GL_SetForeground(GL.color.white);
}

static void Soldier_draw(Soldier* soldier)
{
  GL_SetForeground(soldier->color);
  GL_FillRectangle(
    soldier->x - HALF_SIZE, soldier->y - HALF_SIZE,
    soldier->size, soldier->size);
  GL_SetForeground(GL.color.white);
}

static void Player_init()
{
  player.x = player.mov_x = HALF_WIDTH;
  player.y = player.mov_y = HALF_HEIGHT;
  player.size = ENTITY_SIZE;
  player.speed = PLAYER_SPEED;
  player.color = COLOR_GREEN;
}

static void Enemies_init()
{
  GL_Point positions[8] = {
    { ENTITY_SIZE, ENTITY_SIZE },
    { INNER_WIDTH - ENTITY_SIZE, ENTITY_SIZE },
    { INNER_WIDTH - ENTITY_SIZE, INNER_HEIGHT - ENTITY_SIZE },
    { ENTITY_SIZE, INNER_HEIGHT - ENTITY_SIZE },

    { HALF_WIDTH, ENTITY_SIZE },
    { INNER_WIDTH - ENTITY_SIZE, HALF_HEIGHT },
    { HALF_WIDTH, INNER_HEIGHT - ENTITY_SIZE },
    { ENTITY_SIZE, HALF_HEIGHT }
  };

  size_t count = GetSizeOfArray(enemies);

  for (register size_t i = 0; i < count; ++i)
  {
    enemies[i].size = ENTITY_SIZE;
    enemies[i].speed = ENEMY_SPEED;
    enemies[i].color = COLOR_BLUE;
    enemies[i].x = positions[i].x;
    enemies[i].y = positions[i].y;
    enemies[i].mov_x = positions[i].x;
    enemies[i].mov_y = positions[i].y;
  }
}

static void Player_update()
{
  if (player.stop) return;

  if (controls.any)
  {
    if (controls.left) player.x -= player.speed;
    if (controls.right) player.x += player.speed;

    if (controls.up) player.y -= player.speed;
    if (controls.down) player.y += player.speed;

    if (player.x - HALF_SIZE < 0) player.x = HALF_SIZE;
    if (player.x + HALF_SIZE > INNER_WIDTH)
      player.x = INNER_WIDTH - HALF_SIZE;

    if (player.y - HALF_SIZE < 0) player.y = HALF_SIZE;
    if (player.y + HALF_SIZE > INNER_HEIGHT)
      player.y = INNER_HEIGHT - HALF_SIZE;

    player.mov_x = player.x;
    player.mov_y = player.y;
  }
  else
  {
    if (player.x < player.mov_x) player.x += player.speed;
    if (player.x > player.mov_x) player.x -= player.speed;

    if (player.y < player.mov_y) player.y += player.speed;
    if (player.y > player.mov_y) player.y -= player.speed;
  }

}

static void Enemies_update()
{
  size_t count = GetSizeOfArray(enemies);

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
      (abs(player.x - e->x) < AGGRO_RADIUS)
      && (abs(player.y - e->y) < AGGRO_RADIUS)
    ) {
      e->mov_x = player.x;
      e->mov_y = player.y;
      e->color = COLOR_RED;
    }

    if (e->x == e->mov_x && e->y == e->mov_y)
    {
      e->mov_x = (rand() % (WINDOW_WIDTH - e->size * 2)) + e->size;
      e->mov_y = (rand() % (WINDOW_HEIGHT - e->size * 2)) + e->size;
      e->color = COLOR_BLUE;
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

  if (controls.up || controls.down
      || controls.left || controls.right)
    controls.any = 1;
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

  if (!controls.up && !controls.down
      && !controls.left && !controls.right)
    controls.any = 0;
}


static void ButtonPressHandler(XButtonEvent* event)
{
  player.mov_x = event->x;
  player.mov_y = event->y;
}

static void ButtonReleaseHandler(XButtonEvent* event)
{
  (void) event;
}

static void UpdateState(void)
{
  Player_update();
  Enemies_update();
}

static void RenderFrame(void)
{
  Grid_draw(COLOR_GRAY, ENTITY_SIZE);
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
  GL_SetWindowFixed();
  GL_SetFont("10x20");

  GL_SetExposeHandler(ExposeHandler);
  GL_SetKeyPressHandler(KeyPressHandler);
  GL_SetKeyReleaseHandler(KeyReleaseHandler);
  GL_SetButtonPressHandler(ButtonPressHandler);
  GL_SetButtonReleaseHandler(ButtonReleaseHandler);

  GL_Loop(UpdateState, RenderFrame);

  GL_Quit();
}
