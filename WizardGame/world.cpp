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
  // if (printDelta == true) {
  //   std::cout << "deltaTime: " << deltaTime << std::endl;
  // }
  // std::cout << SDL_GetTicks() << '\n';
  deltaTime = time-lastTime;
  // UpdateMessages();
  //std::cout << "deltaTime: " << deltaTime << '\n';
  // floor.Update();
  //enemy.Update(player, deltaTime);
  //enemy2.Update2(player);
  //player.EnemyUpdate(enemy);
  //player.EnemyUpdate(enemy2);
  // player.Update();
  // e->setPlayerPointer(&player);
  // e->Update(L);
  // pManager.Update();
  //throne.Update();
  //obj.transform.lookAt(player.GetCamera().m_position);
  //obj.Update(deltaTime,0);
  if (deltaTime != 0) {
    if (1000 / deltaTime < 100) {
      FPS = 1000 / deltaTime;
      // std::cout << "Current FPS:" << 1000 / deltaTime << '\n';
    }
  }
  if (1000/maxFPS > deltaTime) {
    SDL_Delay(1000 / maxFPS - SDL_GetTicks() + lastTime);
  }else {
    lastTime = time;
    time = 0;
  }

  //testMap.Update(L);
  testMap.eSystem.checkForConsole();
  auto f = std::async(&Map::Update, &testMap, L);

  //std::cout << deltaTime << '\n';
  // UpdateMessages();
  // if (house.isColliding(eSystem.playerPos)) {
  //   std::cout << "character is colliding with house" << '\n';
  // }
  //std::cout << "is colliding with throne: " << throne.CheckCollision(player.GetCamera().m_position) << '\n';
}
void World::Draw() {
  // floor.Draw(testMap.eSystem.getMainCam());
  // auto gfxc = e->get<GraphicsComponent>();
  // gfxc->Update(deltaTime);
  // gfxc->Draw(player.GetCamera());
  
  // std::cout << (int) testMap.eSystem.getMainCam().m_position.x << " " << (int) testMap.eSystem.getMainCam().m_position.z << std::endl;
  //auto f = std::async(&EntitySystem::Draw, &testMap.eSystem, deltaTime);
  testMap.eSystem.Draw(deltaTime);
  menu.Draw(800,600);
  
}
World::~World() {}