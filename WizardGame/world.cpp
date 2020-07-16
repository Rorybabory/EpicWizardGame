#include "world.h"
#include "imgui_sdl.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include <future>

extern double FPS;
void World::Update() {
  time = SDL_GetTicks();
  if (printDelta==true) {
    printDelta=false;
  }else {
    printDelta = true;
  }
  deltaTime = time-lastTime;
  if (deltaTime != 0) {
    if (1000 / deltaTime < 100) {
      FPS = 1000 / deltaTime;
    }
  }
  if (1000/maxFPS > deltaTime) {
    SDL_Delay(1000 / maxFPS - SDL_GetTicks() + lastTime);
  }else {
    lastTime = time;
    time = 0;
  }
  testMap.Update(L);
  testMap.eSystem.checkForConsole();
  
  auto spawn = std::async(&EntitySystem::spawnEntities, &testMap.eSystem, L);
  testMap.checkLoad(L);
}
void World::Draw() {
  testMap.eSystem.Draw(deltaTime);
}
World::~World() {}