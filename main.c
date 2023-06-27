#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "raylib.h"

typedef struct Game
{
  float x;
  float y;
  float dx;
  float dy;
} Game;

typedef Game (*FreshFunc)();
typedef Game (*UpdateFunc)(Game, float);
typedef void (*RenderFunc)(Game, float);

const char* game_module_file_path = "./libgame.so";

void* loadGameModule(FreshFunc* fresh, UpdateFunc* update, RenderFunc* render)
{
  void* gameModule = dlopen(game_module_file_path, RTLD_NOW | RTLD_LOCAL);
  if (!gameModule)
  {
    fprintf(stderr, "Failed to open game module: %s\n", dlerror());
    return NULL;
  }

  printf("Loaded %s\n", game_module_file_path);

  *fresh = (FreshFunc)dlsym(gameModule, "fresh");
  *update = (UpdateFunc)dlsym(gameModule, "update");
  *render = (RenderFunc)dlsym(gameModule, "render");

  return gameModule;
}

int main()
{
  // Initialize function pointers
  FreshFunc fresh = NULL;
  UpdateFunc update = NULL;
  RenderFunc render = NULL;

  // Open the game module
  void *gameModule = loadGameModule(&fresh, &update, &render);
  if (!gameModule)
  {
    return 1;
  }
  
  // Initialize Raylib
  int width = 800;
  int height = 600;
  const char *title = "Game";
  InitWindow(width, height, title);

  // Main loop
  Game game = fresh();
  while (!WindowShouldClose())
  {
    // Reload game module on 'R' key press
    if (IsKeyPressed(KEY_R))
    {
      dlclose(gameModule);
      gameModule = loadGameModule(&fresh, &update, &render);
      if (!gameModule)
      {
        return 1;
      }
    }

    // Update and render game
    float dt = GetFrameTime();
    if (update && render)
    {
      game = update(game, dt);
      render(game, dt);
    }

    // End frame
    EndDrawing();
  }

  // Close Raylib window
  CloseWindow();

  // Close game module
  dlclose(gameModule);

  return 0;
}
