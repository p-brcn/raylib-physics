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
typedef void (*RenderFunc)(Game);

int main()
{
  // Open the game module
  void *gameModule = dlopen("./libgame.so", RTLD_NOW | RTLD_LOCAL);
  if (!gameModule)
  {
    fprintf(stderr, "Failed to open game module: %s\n", dlerror());
    return 1;
  }

  // Load the fresh function from the game module
  FreshFunc fresh = (FreshFunc)dlsym(gameModule, "fresh");
  if (!fresh)
  {
    fprintf(stderr, "Failed to get fresh function from game module: %s\n", dlerror());
    dlclose(gameModule);
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
      gameModule = dlopen("./libgame.so", RTLD_NOW | RTLD_LOCAL);
      if (!gameModule)
      {
        fprintf(stderr, "Failed to reload game module: %s\n", dlerror());
        return 1;
      }
      fresh = (FreshFunc)dlsym(gameModule, "fresh");
      if (!fresh)
      {
        fprintf(stderr, "Failed to get fresh function from reloaded game module: %s\n", dlerror());
        dlclose(gameModule);
        return 1;
      }
      printf("Loaded libgame.so\n");
    }

    // Update and render game
    float dt = 0.016f;
    UpdateFunc update = (UpdateFunc)dlsym(gameModule, "update");
    RenderFunc render = (RenderFunc)dlsym(gameModule, "render");
    if (update && render)
    {
      game = update(game, GetFrameTime());
      render(game);
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
