#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "raylib.h"

typedef struct Game
{
  float x;
  float y;
  float dx;
  float dy;
} Game;

int text_size = 18;
int frames = 0;

float player_size = 64.0;
Color player_color = RED;
float gx = 0.0;
float gy = 2000.0;
float time_scale = 2.0;
float dampness = 0.75;
float jump_force = 500.0;
float pull_strength = 1500.0;
float cur_vel = 0.0;

bool isDragging = false; // Not currently used, but could be useful for other things
bool showDebugInfo = true;

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
  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
  {
    isDragging = true;
    // Makes the player move towards the mouse
    float dx = GetMouseX() - (game.x + player_size / 2.0);
    float dy = GetMouseY() - (game.y + player_size / 2.0);
    float dist = sqrt(dx * dx + dy * dy);
    float force = pull_strength / dist;
    game.dy -= gy * dt;
    game.dx += dx * force * dt;
    game.dy += dy * force * dt;

    // Draws a line from the player to the mouse
    DrawLine((int)(game.x + player_size / 2.0), (int)(game.y + player_size / 2.0), (int)GetMouseX(), (int)GetMouseY(), WHITE);
  }

  isDragging = false;

  float width = (float)GetScreenWidth();
  float height = (float)GetScreenHeight();

  game.dx += gx * dt;
  game.dy += gy * dt;

  game.dx -= game.dx * dampness * dt;
  game.dy -= game.dy * dampness * dt;

  float nx = game.x + game.dx * dt * time_scale;
  float ny = game.y + game.dy * dt * time_scale;

  if (nx < 0.0 || nx + player_size >= width)
  {
    game.dx = -game.dx;
  }
  else
  {
    game.x = nx;
  }

  if (ny < 0.0 || ny + player_size >= height)
  {
    game.dy = -game.dy;
  }
  else
  {
    game.y = ny;
  }
  
  return game;
}

void debugInfo(Game game, float dt) {
  int fps = GetFPS();
  char fpsText[16];
  snprintf(fpsText, sizeof(fpsText), "FPS: %i", fps);
  DrawText(fpsText, 10, 10, text_size, WHITE);

  char mousePosText[32];
  snprintf(mousePosText, sizeof(mousePosText), "Mouse: (%i, %i)", (int)GetMouseX(), (int)GetMouseY());
  DrawText(mousePosText, 10, 40, text_size, WHITE);

  char gameText[64];
  snprintf(gameText, sizeof(gameText), "dx: %f, dy: %f", game.dx, game.dy);
  DrawText(gameText, 10, 70, text_size, WHITE);

  frames++;
  char velText[64];
  if (frames % (int)(0.1 / dt) == 0)
  {
    cur_vel = sqrt(game.dx * game.dx + game.dy * game.dy);
  }
  snprintf(velText, sizeof(velText), "Velocity: %f", cur_vel);
  DrawText(velText, 10, 100, text_size, WHITE);

  // Force vectors
  DrawLine((int)(game.x + player_size / 2.0), (int)(game.y + player_size / 2.0), (int)(game.x + player_size / 2.0 + game.dx), (int)(game.y + player_size / 2.0 + game.dy), GREEN);
}

void render(Game game, float dt)
{
  BeginDrawing();
  ClearBackground((Color){24, 24, 24, 255});

  int x = (int)game.x;
  int y = (int)game.y;
  int w = (int)player_size;
  int h = w;
  DrawRectangle(x, y, w, h, player_color);

  if (showDebugInfo) {
    debugInfo(game, dt);
  }

  EndDrawing();
}
