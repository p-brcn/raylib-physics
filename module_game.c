#include <stdio.h>
#include <raylib.h>

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
float dampness = 0.75;

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

  float width = (float)GetScreenWidth();
  float height = (float)GetScreenHeight();

  game.dx += gx * dt;
  game.dy += gy * dt;

  float nx = game.x + game.dx * dt;
  float ny = game.y + game.dy * dt;

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

  EndDrawing();
}