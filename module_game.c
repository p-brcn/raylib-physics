#include <stdio.h>
#include "raylib.h"

typedef struct Game
{
  float x;
  float y;
  float dx;
  float dy;
} Game;

float player_size = 64.0;
Color player_color = RED;
float gx = 0.0;
float gy = 2000.0;
float time_scale = 2.0;
float dampness = 0.75;
float jump_force = 500.0;
float mouse_x = 0.0;
float mouse_y = 0.0;
float spring_fac = 100.0;

Game fresh()
{
  Game game;
  game.x = 0.0;
  game.y = 0.0;
  game.dx = 100.0;
  game.dy = 100.0;
  return game;
}

Game update(Game game, float dt)
{
  if (IsKeyPressed(KEY_SPACE))
  {
    game.dy -= jump_force;
  }

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    mouse_x = GetMouseX();
    mouse_y = GetMouseY();
  }

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    float new_mouse_x = GetMouseX();
    float new_mouse_y = GetMouseY();

    game.dx += ((new_mouse_x - game.x) * spring_fac) * dt;
    game.dy += ((new_mouse_y - game.y) * spring_fac*2) * dt;

    mouse_x = new_mouse_x;
    mouse_y = new_mouse_y;
  }

  float width = (float)GetScreenWidth();
  float height = (float)GetScreenHeight();

  game.dx += gx * dt;
  game.dy += gy * dt;

  float nx = game.x + game.dx * dt * time_scale;
  float ny = game.y + game.dy * dt * time_scale;

  if (nx < 0.0 || nx + player_size >= width)
  {
    game.dx = -dampness * game.dx;
  }
  else
  {
    game.x = nx;
  }

  if (ny < 0.0 || ny + player_size >= height)
  {
    game.dy = -dampness * game.dy;
  }
  else
  {
    game.y = ny;
  }

  return game;
}

void render(Game game)
{
  BeginDrawing();
  ClearBackground((Color){24, 24, 24, 255});

  int x = (int)game.x;
  int y = (int)game.y;
  int w = (int)player_size;
  int h = w;
  DrawRectangle(x, y, w, h, player_color);

  int fps = GetFPS();
  char fpsText[16];
  snprintf(fpsText, sizeof(fpsText), "FPS: %i", fps);
  DrawText(fpsText, 10, 10, 20, WHITE);

  char mousePosText[32];
  snprintf(mousePosText, sizeof(mousePosText), "Mouse: (%i, %i)", (int)GetMouseX(), (int)GetMouseY());
  DrawText(mousePosText, 10, 40, 20, WHITE);
  
  EndDrawing();
}
