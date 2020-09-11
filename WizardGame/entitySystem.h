#ifndef ESYSTEM_H
#define ESYSTEM_H
#include "entity.h"
#include "luaLibrary.h"
#include "skybox.h"
#include "camera.h"
#include "messaging.h"
#include "CollisionSystem.h"
#include <vector>
#include <string>
#include <math.h>
#include <qu3e/q3.h>
#include <stdio.h>
#include <cmath>
#include "prop.h"
#include "ProjComponent.h"
#include "imgui_sdl.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include <chrono>
#include <cmath> 
#include "Timer.h"
#include "Minimap.h"
extern double FPS;
extern int entityCount;
extern bool drawScene;
extern std::vector<std::string> loadedScripts;
extern q3Scene* scenePointer;
extern bool resetText;
struct drawData {
    bool isConsoleOpen = false;
    std::vector<Prop*> propsPointer;
    std::vector<Entity*> entitiesPointer;
    bool frozen = false;
    bool isLevelEditorOpen = false;
    Object * clickObject;
    double deltaTime = 0.0;
    Camera* mainCam;
};
struct entitySpawnData {
    std::string script;
    std::string type;
    glm::vec2 pos;
    lua_State* L;
};
extern int globalVariable;
extern void addTextBox(std::string text, glm::vec2 pos, glm::vec3 color, int scale);
extern void clearText();
extern void drawText();
extern bool restartFile;
extern std::vector<std::string> mods;
extern std::vector<glm::vec2> entityPos;
extern lua_State* LPointer;
extern void setHighScore(int val);
extern int getHighScore();

class EntitySystem {
public:
    void addFunctions(lua_State* L) {
        using namespace luabridge;
        getGlobalNamespace(L)
            .beginNamespace("util")
            .addFunction("getDistance", &getDistance)
            .endNamespace()
            .beginClass<Entity>("Entity")
            // .addFunction("Damage",&Entity::Damage)
            .addFunction("setAnimation", &Entity::setAnimation)
            .addFunction("playAnimation", &Entity::playAnimation)
            .addFunction("getAnimation", &Entity::getAnimation)
            .addFunction("setPos", &Entity::setPos)
            .addFunction("getX", &Entity::getX)
            .addFunction("getY", &Entity::getY)
            .addFunction("getZ", &Entity::getZ)
            .addFunction("setColorFlash", &Entity::setColorFlash)
            .addFunction("setColor", &Entity::setColor)
            .addFunction("lookAt", &Entity::lookAt)
            .addFunction("lookAtPlayer", &Entity::lookAtPlayer)
            .addFunction("moveForward", &Entity::moveForward)
            .addFunction("moveBackward", &Entity::moveBackward)
            .addFunction("Strafe", &Entity::Strafe)
            .addFunction("write", &Entity::write)
            .addFunction("random", &Entity::random)
            .addFunction("randomInt", &Entity::randomInt)
            .addFunction("setScale", &Entity::setScale)
            .addFunction("getPlayerDistance", &Entity::getPlayerDistance)
            .addFunction("getKeyDirectionX", &Entity::getKeyDirectionX)
            .addFunction("getKeyDirectionY", &Entity::getKeyDirectionY)
            .addFunction("FPSControllerUpdate", &Entity::FPSControllerUpdate)
            .addFunction("kill", &Entity::kill)
            .addFunction("getHP", &Entity::getHP)
            .addFunction("setHP", &Entity::setHP)
            .addFunction("addCounter", &Entity::addCounter)
            .addFunction("updateCounter", &Entity::updateCounter)
            .addFunction("getCount", &Entity::getCount)
            .addFunction("setCount", &Entity::setCount)
            .addFunction("killProgram", &Entity::killProgram)
            .addFunction("spawnEntity", &Entity::spawnEntity)
            .addFunction("Fire", &Entity::Fire)
            .addFunction("FireMultiple", &Entity::FireMultiple)
            .addFunction("FireInstant", &Entity::FireInstant)
            .addFunction("getFloat", &Entity::getFloat)
            .addFunction("setFloat", &Entity::setFloat)
            .addFunction("getGlobalFloat", &Entity::getGlobalFloat)
            .addFunction("setGlobalFloat", &Entity::setGlobalFloat)
            .addFunction("getGlobalBool", &Entity::getGlobalBool)
            .addFunction("setGlobalBool", &Entity::setGlobalBool)
            .addFunction("getBool", &Entity::getBool)
            .addFunction("setBool", &Entity::setBool)
            .addFunction("getString", &Entity::getString)
            .addFunction("setString", &Entity::setString)
            .addFunction("getAnimFrame", &Entity::getAnimFrame)
            .addFunction("getKeyPressed", &Entity::getKeyPressed)
            .addFunction("setCanBeHit", &Entity::setCanBeHit)
            .addFunction("getCanBeHit", &Entity::getCanBeHit)
            .addFunction("setFOV", &Entity::setFOV)
            .addFunction("TopDownUpdate", &Entity::TopDownUpdate)
            .addFunction("isFirstPerson", &Entity::isFirstPerson)
            .addFunction("lookAtNearest", &Entity::lookAtNearest)
            .addFunction("getDistanceFromNearest", &Entity::getDistanceFromNearest)
            .addFunction("getDistanceFromNearestNot", &Entity::getDistanceFromNearestNot)
            .addFunction("getDistanceFromNearestEnt", &Entity::getDistanceFromNearestEnt)
            .addFunction("getPositionFromNearestX", &Entity::getPositionFromNearestX)
            .addFunction("getPositionFromNearestY", &Entity::getPositionFromNearestY)
            .addFunction("doesEntityExist", &Entity::doesEntityExist)
            .addFunction("setGlobalFrozen", &Entity::setGlobalFrozen)
            .addFunction("getGlobalFrozen", &Entity::getGlobalFrozen)
            .addFunction("setFrozen", &Entity::setFrozen)
            .addFunction("getFrozen", &Entity::getFrozen)
            .addFunction("getNearestEntWithName", &Entity::getNearestEntWithName)
            .addFunction("getNearestEntity", &Entity::getNearestEntity)
            .addFunction("getDistanceBetweenTwoPointsAPI", &Entity::getDistanceBetweenTwoPointsAPI)
            .addFunction("Shake", &Entity::Shake)
            .addFunction("isPlayer", &Entity::isPlayer)
            .addFunction("setInverted", &Entity::setInverted)
            .addFunction("pushInsideLevel", &Entity::pushInsideLevel)
            .addFunction("restartCollisionBox", &Entity::restartCollisionBox)
            .addFunction("playDamageAnimation", &Entity::playDamageAnimation)
            .addFunction("setDamageAnimation", &Entity::setDamageAnimation)
            .addFunction("UpdateKeyPresses", &Entity::UpdateKeyPresses)
            .addFunction("setAnimationTag", &Entity::setAnimationTag)
            .addFunction("playAnimationTag", &Entity::playAnimationTag)
            .addFunction("isAnimationPlaying", &Entity::isAnimationPlaying)
            .addFunction("getLastX", &Entity::getLastX)
            .addFunction("getLastY", &Entity::getLastY)
            .addFunction("getLastZ", &Entity::getLastZ)
            .addFunction("setCollisionBox", &Entity::setCollisionBox)
            .addFunction("Delay", &Entity::Delay)
            .addFunction("setUIText", &Entity::setUIText)
            .addFunction("getPaused", &Entity::getPaused)
            .addFunction("setPaused", &Entity::setPaused)
            .addFunction("getDefaultSpeed", &Entity::getDefaultSpeed)
            .addFunction("damageNearest", &Entity::damageNearest)
            .addFunction("damageNearestEnt", &Entity::damageNearestEnt)
            .addFunction("setBrightness", &Entity::setBrightness)
            .addFunction("getProjCount", &Entity::getProjCount)
            .addFunction("setProjCount", &Entity::setProjCount)
            .addFunction("Emit", &Entity::Emit)
            .addFunction("copyPlayerRot", &Entity::copyPlayerRot)
            .addFunction("setRot", &Entity::setRot)
            .addFunction("setPlayerTag", &Entity::setPlayerTag)
            .addFunction("getPlayerTag", &Entity::getPlayerTag)
            .addFunction("resetFrame", &Entity::resetFrame)
            .addFunction("setHue", &Entity::setHue)
            .addFunction("setSaturation", &Entity::setSaturation)
            .addFunction("setValue", &Entity::setValue)
            .addFunction("setParticleSpread", &Entity::setParticleSpread)
            .addFunction("setParticleModel", &Entity::setParticleModel)
            .addFunction("playSound", &Entity::playSound)
            .addFunction("setCollide", &Entity::setCollide)
            .addFunction("getDefaultAnim", &Entity::getDefaultAnim)
            .addFunction("getAnimationID", &Entity::getAnimationID)
            .addFunction("hasAnimation", &Entity::hasAnimation)
            .addFunction("showHealth", &Entity::showHealth)
            .addFunction("setHPColor", &Entity::setHPColor)
            .addFunction("getType", &Entity::getTypeLua)
            .addFunction("setText", &Entity::setText)
            .addFunction("getEntityCount", &Entity::getEntityCount)
            .addFunction("setTextColor", &Entity::setTextColor)
            .addFunction("getMoveDirX", &Entity::getMoveDirX)
            .addFunction("getMoveDirY", &Entity::getMoveDirY)
            .addFunction("getArrowDirX", &Entity::getArrowDirX)
            .addFunction("getArrowDirY", &Entity::getArrowDirY)
            .addFunction("setMapTarget", &Entity::setMapTarget)
            .addFunction("swapMap", &Entity::swapMap)
            .addFunction("round", &Entity::Round)
            .addFunction("addAbility", &Entity::addAbility)
            .addFunction("getAbility", &Entity::getAbility)
            .addFunction("setScreenColor", &Entity::setScreenColor)
            .addFunction("setDrawScene", &Entity::setDrawScene)
            .addFunction("stopProgram", &Entity::stopProgram)
            .addFunction("getAbilityCount", &Entity::getAbilityCount)
            .addFunction("clearAbilities", &Entity::clearAbilities)
            .addFunction("getAbilityDescription", &Entity::getAbilityDescription)
            .addFunction("setSpread", &Entity::setSpread)
            .addFunction("setProjColor", &Entity::setProjColor)
            .addFunction("setProjDelay", &Entity::setProjDelay)
            .addFunction("setObjectPos", &Entity::setObjectPos)
            .addFunction("setObjectColor", &Entity::setObjectColor)
            .addFunction("setObjectScale", &Entity::setObjectScale)
            .addFunction("setEnemyTarget", &Entity::setEnemyTarget)
            .addFunction("getObjectX", &Entity::getObjectX)
            .addFunction("getObjectY", &Entity::getObjectY)
            .addFunction("getObjectZ", &Entity::getObjectZ)
            .addFunction("setUICam", &Entity::setUICam)
            .addFunction("setDrawUI", &Entity::setDrawUI)
            .addFunction("setProjRange", &Entity::setProjRange)
            .addFunction("setTextSize", &Entity::setTextSize)
            .addFunction("setImage", &Entity::setImage)
            .addFunction("setImageTransform", &Entity::setImageTransform)
            .addFunction("setImageDraw", &Entity::setImageDraw)
            .addFunction("setMouseCapture", &Entity::setMouseCapture)
            .addFunction("setScreenResolution", &Entity::setScreenResolution)
            .addFunction("damageWithinADistance", &Entity::damageWithinADistance)
            .addFunction("setObjectModel", &Entity::setObjectModel)
            .addFunction("setScreenShake", &Entity::setScreenShake)
            .addFunction("setHighScore", &Entity::setHighscore)
            .addFunction("getHighScore", &Entity::getHighscore)
            .addFunction("sin", &Entity::sinFunc)
        .endClass();
    }
    std::string removeWord(std::string str, std::string word)
    {
        if (str.find(word) != std::string::npos)
        {
            size_t p = -1;
            std::string tempWord = word + " ";
            while ((p = str.find(word)) != std::string::npos)
                str.replace(p, tempWord.length(), "");
            tempWord = " " + word;
            while ((p = str.find(word)) != std::string::npos)
                str.replace(p, tempWord.length(), "");
        }
        return str;
    }
    std::string convertPath(std::string input) {
        std::string temp = input;
        for (std::string folder : mods) {
            std::string fileToCheck = std::string("./mods/" + folder + "/" + removeWord(input, "./res"));
            //std::cout << "checking for mod file: " << fileToCheck << "\n";
            bool exist = std::experimental::filesystem::exists(fileToCheck);
            if (exist) {
                temp = "./mods/" + folder + "/" + removeWord(input, "./res");
            }
        }
        return temp;
    }
   void drawConsoleWindow() {
        ImGui::Begin("Console Window");
        ImGui::SetWindowSize(ImVec2(600, 600));
        ImGui::Text("This is a console command window");
        ImGui::Text(commandLog.c_str());
        ImGui::InputText("", command, 255);
        ImGui::End();
   }
   void drawPropEditor() {
       ImGui::Begin("ADD PROP");
       ImGui::SetWindowSize(ImVec2(200, 500));
       ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
       ImGui::InputText("", propEditor, 255);
       ImGui::SliderInt("Size", &propSize, 0, 50, "%d");
       ImGui::PushItemWidth(30);
       ImGui::InputInt("Rotation", &propRotation, 0);
       ImGui::Checkbox("Collides", &collideCheckbox);
       if (ImGui::Button("Create Prop") == true) {
           std::cout << "PROP CREATED\n";
           addProp(propEditor, glm::vec3((float)propSize), glm::vec3(clickPos.x, 0.0f, clickPos.y), glm::radians((float)propRotation), collideCheckbox);
       }
       if (ImGui::Button("Undo") == true) {
           props[props.size() - 1]->b.setPos(glm::vec3(-999.0f, -999.0f, -999.0f));
           props.erase(props.begin() + props.size() - 1);
       }
       ImGui::End();
   }
   void drawCursorEditor() {
       ImGui::Begin("Set Cursor Positon");
       ImGui::PushItemWidth(30);

       ImGui::SetWindowSize(ImVec2(800, 100));
       ImGui::SetWindowPos(ImVec2(0.0f, 500));
       ImGui::InputInt("X", &cursorPosX, 0);
       ImGui::InputInt("Y", &cursorPosY, 0);
       if (ImGui::Button("Set Position") == true) {
           clickObject.setPos(glm::vec3((float) cursorPosX, 0.0f, (float)cursorPosY));
       }
       ImGui::End();
   }
   void drawEntityEditor() {
       ImGui::Begin("ADD ENTITY");
       ImGui::SetWindowSize(ImVec2(200, 500));
       ImGui::SetWindowPos(ImVec2(600.0f, 0.0f));

       ImGui::InputText("", entityEditor, 255);
       if (ImGui::Button("Create Entity") == true) {
           addEntityAtPos("res/scripts/entities.lua", entityEditor, clickPos, L);
       }
       if (ImGui::Button("Undo") == true) {
           entities[entities.size() - 1]->collider.setPos(glm::vec3(-999.0f, -999.0f, -999.0f));
           entities.erase(entities.begin() + entities.size() - 1);
       }
       ImGui::End();
   }
   void drawSaveLoad() {
       ImGui::Begin("Save Load");
       ImGui::SetWindowSize(ImVec2(400, 100));
       ImGui::SetWindowPos(ImVec2(200.0f, 0.0f));
       ImGui::InputText("", mapPath, 255);
       if (ImGui::Button("Save") == true) {
           save = true;
       }
       if (ImGui::Button("Load") == true) {
           load = true;
       }
       ImGui::End();
   }
   void drawLevelEditor() {
       drawPropEditor();
       drawEntityEditor();
       drawSaveLoad();
       drawCursorEditor();
   }
   void updateLevelEditor() {
       drawLevelEditor();
       SDL_Event e;
       SDL_PollEvent(&e);
       int xMouse;
       int yMouse;
       SDL_GetMouseState(&xMouse, &yMouse);
       float xPlacePos = (float)xMouse;
       float yPlacePos = (float)yMouse;
       xPlacePos -= Width / 2.0;
       yPlacePos -= Height / 2.0;
       xPlacePos = -xPlacePos/1.7f;
       yPlacePos = -yPlacePos/1.7f;
       xPlacePos *= 2.0;
       yPlacePos *= 2.0;
       Uint8* keys;
       keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
       if (keys[SDL_SCANCODE_SPACE] == true) {
           clickPos.x = xPlacePos;
           clickPos.y = yPlacePos;
           clickObject.setPos(glm::vec3(clickPos.x, 0.0f, clickPos.y));
       }
   }

  int roundToNearest(int value, int nearest) {
    return value + ((nearest-(value % nearest)) % nearest);
  }
  void addEntity(std::string script, std::string type, lua_State* L) {
    //luaL_openlibs(L);
    luah::loadScript(L,script);
    luabridge::LuaRef entityRef = luabridge::getGlobal(L,"entities");
    for (int i = 0; i < entityRef.length(); ++i) {
      std::string name = entityRef[i+1].cast<std::string>();
      luah::loadScript(L,"res/scripts/" + name + ".lua");
    }
    Entity * e = loadEntity(L,type);
    e->setUpCollider(&scene,e->scaleColl);
    e->LPointer = L;
    //e->OnStart(L);
    entities.push_back(e);
  }
  void addProp(std::string folder,glm::vec3 scale,glm::vec3 pos1, float rot, bool collide) {
    Prop * p = new Prop(folder,scale,pos1, rot, &scene, collide);
    p->b.setPos(pos1);
    props.push_back(p);
  }
  void addEntityAtPos(std::string script, std::string type, glm::vec2 pos, lua_State* L) {
      Entity* e;
          
          if (L != nullptr && functionsAdded == false) {
              functionsAdded = true;
              addFunctions(L);
          }
      
      {
          Timer t = Timer("adding entity " + type);
          e = loadEntity(L, type);
          
      }
      std::cout << "\n\n\n\n\n\n";
          //std::cout << "loaded entity with type: " << type << "\n";
          if (type != "player") {
              e->setPos(pos.x, 0.0f, pos.y);
          }
          else {
              e->setPos(pos.x, 0.0f, pos.y);
              e->setCamPos(glm::vec3(pos.x, -15.0f, pos.y));
          }
          e->setUpCollider(&scene, e->scaleColl);
          e->OnStart(L);
          entities.push_back(e);
          e->startingPos = pos;
          e->pos = glm::vec3(pos.x, 0.0f, pos.y);
          this->L = L;
  }
  void addEntityAtPos2(std::string script, std::string type, glm::vec2 pos, lua_State* L) {
      entitySpawnData data;
      data.script = script;
      data.type = type;
      data.pos = pos;
      data.L = L;
      spawnData.push_back(data);
  }
  void UpdateDistances() {
    for (Entity * mainEnt : entities) {
      float smallestDist = 10000.0f;
      for (Entity * targetEnt : entities) {
          float dist = sqrt(pow(mainEnt->pos.x-targetEnt->pos.x,2)+
                            pow(mainEnt->pos.z-targetEnt->pos.z,2)*1.0f);
          if (dist < smallestDist) {
            smallestDist = dist;
            mainEnt->nearestEntityDist = smallestDist;
            if (targetEnt->type == "player") {
              mainEnt->nearestEntityPos = playerPos;
            }else {
              mainEnt->nearestEntityPos = targetEnt->pos;
            }
            mainEnt->nearestEntityType = targetEnt->type;
          }
      }
    }
  }
  Entity* getEntityByName(std::string name) {
    for (Entity * entity : entities) {
      if (strcmp(entity->getType().c_str(),name.c_str())) {
        return entity;
      }
    }
  }
  void restartCollision() {
    scene = q3Scene(1.0/60.0);
    for (Entity * e : entities) {
      e->setUpCollider(&scene, glm::vec3(e->scale,10,e->scale));
    }
    std::cout << "Scene Restarting..." << std::endl;

    for (Prop * prop : props) {
      prop->setUpCollider(&scene);
    }
  }
  void resetFloor() {
      floor.init(glm::vec3(1000.0f, 1.0, 1000.0f), glm::vec3(0.0f, 0.0f, 0.0f), &scene, eStaticBody);
  }
  EntitySystem ()  : scene(1.0/60.0), clickObject("./res/X.obj",glm::vec4(1.0f,0.0f,0.0f,1.0f),"./res/basicShader",false) {
    floor.init(glm::vec3(1000.0f, 1.0, 1000.0f), glm::vec3(0.0f, 0.0f, 0.0f), &scene, eStaticBody);
    scene.SetIterations(4);
    topDownCamera.InitCam(glm::vec3(0, 1000, 0), 70.0, 800.0f / 600.0f, 0.01f, 2000.0f);
    topDownCamera.Pitch(1.57);

    LPointer = L;
    //mapOfConsoleCommands["print"] = &runPrintCommand;
    //mapOfConsoleCommands.emplace("help", &EntitySystem::helpCommand);
    //mapOfConsoleCommands.emplace("spawnEntity", &EntitySystem::spawnEntityCommand);
    //mapOfConsoleCommands.emplace("setSpeed", &EntitySystem::setSpeedCommand);
    //mapOfConsoleCommands.emplace("killEntity", &EntitySystem::killEntityCommand);
    //mapOfConsoleCommands.emplace("godmode", &EntitySystem::GodModeCommand);
    //mapOfConsoleCommands.emplace("setProjColor", &EntitySystem::ProjColorCommand);
    //mapOfConsoleCommands.emplace("printNum", &EntitySystem::printNumCommand);
    // luaL_openlibs(L);
    // luah::loadScript(L,script);
    // luabridge::LuaRef entityRef = luabridge::getGlobal(L,"entities");
    // for (int i = 0; i < entityRef.length(); ++i) {
    //   animatedObject * ao = new animatedObject();
    //   std::string name = entityRef[i+1].cast<std::string>();
    //   luah::loadScript(L,"res/scripts/" + name + ".lua");
    //   Entity * e = loadEntity(L,name);
    //   e->setUpCollider(&scene,glm::vec3(5.0f,500.0f,5.0f));
    //   e->OnStart(L);
    //   startEnt.push_back(e);
    // }
  }
  Entity* getEntityByID(int id) {
    return entities[id];
  }
  void clearScene() {
      scene.RemoveAllBodies();
  }
  void checkSpawnedEntities(lua_State* L) {
    for (Entity * e : entities) {
      for (SpawnData sD : e->spawnedEntityData) {
        //auto addEnt = std::async(&EntitySystem::addEntityAtPos, this, "res/scripts/entities.lua", sD.name, sD.pos, L);
        
        addEntityAtPos("res/scripts/entities.lua",sD.name,sD.pos,L);
      }
      if (e->spawnedEntityData.size()>0) {
        e->spawnedEntityData.clear();
      }
    }
  }
  void checkForCollisionError() {
    for (Entity * e : entities) {
      if (e->type == "player" && e->hasCollisionError == true) {
        // restartCollision();
        e->hasCollisionError = false;
        std::cout << "COLLISION ERROR FOUND. POSITION IS NULL" << std::endl;
      }
    }
  }
  void DumpToFile() {
    FILE * pFile;
    char buffer [100];
#pragma warning(disable:4996)
    pFile = fopen("res/scripts/dumps/dump1.txt","w+");
    if (pFile == NULL) {
      std::cout << "error opening file..." << '\n';
    }
    /*for (Entity * e : entities) {
      if (e->type == "player") {
        e->collider.body->Dump(pFile,1);
      }
    }*/
  }

  void spawnEntities(lua_State * L) {
      for (entitySpawnData data : spawnData) {
          Entity* e = loadEntity(L, data.type);
          std::cout << "added entity with type: " << data.type << "\n";

          if (data.type != "player") {
              e->setPos(data.pos.x, 0.0f, data.pos.y);
          }
          else {
              e->setPos(data.pos.x, 0.0f, data.pos.y);
              e->setCamPos(glm::vec3(data.pos.x, -15.0f, data.pos.y));
          }
          e->setUpCollider(&scene, e->scaleColl);

          e->OnStart(L);

          entities.push_back(e);
          e->startingPos = data.pos;
          e->pos = glm::vec3(data.pos.x, 0.0f, data.pos.y);
          this->L = L;
      }
      spawnData.clear();
  }
  void restartText() {
      for (Entity* e : entities) {
          auto guiC = e->get<GUIComponent>();
          guiC->freeData();
      }
  }
  void Update(lua_State* L) {
    //checkCollision();
    checkSpawnedEntities(L);
    // UpdateDistances();
    setHeroTarget();
    checkForCollisionError();
    //checkForConsole();
    scenePointer = &scene;
    scene.setTimescale(1.0 / (FPS));
    entityCount = entities.size();
    double timer = SDL_GetTicks();
    if (isConsoleOpen || isLevelEditorOpen) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
  //  clearText();
  //  addTextBox(*text,glm::vec2(-0.9f,0.9f), glm::vec3(1.0f,1.0f,0.0f), 30);
    if (FPS < 50.0) {
        std::cout << "FPS: " << FPS << std::endl;
    }
    
    if (printDelta==true) {
      printDelta=false;
    }else {
      printDelta = true;
    }
    if (printDelta == true) {
      if ((1000/FPS)/16.6666<2.0) {
        speedMultiplier = (1000/FPS)/20;
      }
    }
    // speedMultiplier = ((-((FPS/60)-1.0))*2.0)+1.0;
    // std::cout << "Global int: " << globalVariable << std::endl;
    // if (dumped==false) {
    //   DumpToFile();
    //   dumped = true;
    // }
    if (resetText == true) {
        bool hasReset = true;
        for (int i = 0; i < entities.size(); i++) {
            auto guiC = entities[i]->get<GUIComponent>();
            if (guiC != NULL) {
                if (guiC->resetFont == false) {
                    hasReset = false;
                }
            }
        }
        if (hasReset == true) {
            resetText = false;
        }
    }
    
    for (int i = 0; i<entities.size(); i++) {
      //entityPos[i] = glm::vec2(entities[i]->pos.x, entities[i]->pos.z);
      
      // if (entities[i]->type == "player") {
      //   std::cout << entities[i]->collider.body->GetTransform( ).position.z << "\n";
      // }
        
      entities[i]->allEntities = &entities;
      entities[i]->scenePointer = &scene;
      entities[i]->speedModifier = speedMultiplier;
      auto projc = entities[i]->get<ProjectileComponent>();
      auto sgraphicsc = entities[i]->get<StaticGraphicsComponent>();
      // if (sgraphicsc != NULL && !sgraphicsc->canMove && entities[i]->type == "player") {
      //   entities[i]->lastPosStatic = entities[i]->pos;
      // }
      if (projc != NULL) {
        projc->collUpdate();
        projc->setSpeedMultiplier(speedMultiplier);
      }
      
      if (entities[i]->type == "player") {
        if (entities[i]->killed == true) {
            if (entities[i]->FloatsVars["Score"] > getHighScore()) {
                setHighScore(entities[i]->FloatsVars["Score"]);
                std::cout << "SET NEW HIGH SCORE TO: " << entities[i]->FloatsVars["Score"] << "\n";
            }
            load = true;
        }
      }
      if (entities[i]->killed == true) {
          //delete entities[i];
          entities[i]->emitter.particles.clear();
          free(entities[i]);
          //entityPos.erase(entityPos.begin() + i);
          entities.erase(entities.begin() + i);
          //std::cout << "killed entity\n";
      }
      if (entities[i]->hasCollision) {
          if (entities[i]->type != "player" || entities[i]->type != "AIHero") {
              entities[i]->collider.setPos(glm::vec3(entities[i]->pos.x, 0.0f, entities[i]->pos.z));
          }
          else {

              entities[i]->collider.setPos(glm::vec3(playerPos.x, 0.0f, playerPos.z));
          }
      }
      // if (entities[i]->killed == true) {
      //   // scene.RemoveBody(entities[i]->collider.body);
      //   entities.erase(entities.begin()+i);
      //   delete entities[i];
      // }
        entities[i]->lastPos = entities[i]->pos;
    }
    scene.Step();
    for (Entity * e : entities) {
        if (e->map != "") {
            std::string val = e->map;
            mapPath[255] = *(val.c_str());
        }
        
      if (e->modText == true) {
        text = e->textPointer;
        e->modText = false;
      }
      if (e->hasCollision) {
          if (e->type != "player") {
              e->pos = glm::vec3(e->collider.getPos().x, 0.0f, e->collider.getPos().z);
          }
          else {
              e->pos = glm::vec3(e->collider.getPos().x, 0.0f, e->collider.getPos().z);
              //e->setCamPos(glm::vec3(e->collider.getPos().x,2.0f,e->collider.getPos().z));
              //e->collider.setPos(e->getCamPos());
          }

      }
      collisions++;
      if (collisions == 2) {
        collisions = 0;
      }
      e->collider.resetVelocity();

      e->collider.body->SetToAwake();
      auto sgraphicsc = e->get<StaticGraphicsComponent>();
      if (sgraphicsc != NULL && !sgraphicsc->canMove) {
        e->pos = e->lastPosStatic;
      }


      auto projc = e->get<ProjectileComponent>();
      if (e->type == "AIHero") {
        if ( e->collider.getVelocity().x != 0.0f
          && e->collider.getVelocity().z != 0.0f
          && e->collider.getVelocity().y != 0.0f) {
          std::cout << "ERROR....." << std::endl;
        }
      }
      if (projc != NULL) {
        for (int i = 0; i < projc->objects.size(); i++) {
          if (projc->objects[i]->boxDestroy == true) {
            scene.RemoveBody(projc->objects[i]->projB.body);
            delete projc->objects[i];
            projc->objects.erase(projc->objects.begin()+i);
          }
        }
        if (frozen == false) {
            projc->Update();
        }
      }
      if (e->type == "player") {
        if (e->globalFrozen == true) {
          frozen = true;
        }else {
          if (frozen == true) {
            for (Entity * e2 : entities) {
              auto projc2 = e2->get<ProjectileComponent>();
              if (projc2 != NULL) {
                projc2->delayCount = 0;
              }
            }
          }
          frozen = false;
        }
        if (e->pos.x < -1000.0f || e->pos.x > 1000.0f ||
            e->pos.z < -1000.0f || e->pos.z > 1000.0f) {
          // e->setCamPos(glm::vec3(0.0f,10.0f,0.0f));
          auto cameraC = e->get<CameraComponent>();
          if (cameraC->isFirstPerson == false) {
            e->setCamPos(glm::vec3(0.0f,500.0f,0.0f));
          }else {
            e->pos = glm::vec3(0.0f,10.0f,0.0f);
          }
          std::cout << "TELEPORTED PLAYER ENTITY" << std::endl;
        }
      }
      if (projc != NULL) {
        if (!projc->objects.empty()) {
          
          // for (o : projc->objects) {
          //   o->projB.init(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.0f,0.0f,0.0f),&scene,eDynamicBody);
          // }
          // for (p : props) {
          //   bool coll = projc->checkCollisionWithBox(glm::vec2(p->pos.x,p->pos.z),glm::vec2(p->b.sc.x,p->b.sc.z));
          //   if (coll) {
          //     count++;
          //     std::cout << "BULLET COLLISION " << count << '\n';
          //   }
          // }
        }
      }
      // if (!messager.getMessage().Vec2Vector.empty()) {
      //   e->UpdateProj(messager.getMessage().Vec2Vector);
      // }
      e->entities = &entities;
      e->setPlayerPos(playerPos);
      if (isConsoleOpen || isLevelEditorOpen) {
          e->isPaused = true;
      }
      else {
          e->isPaused = false;
      }
      isTopDown = isLevelEditorOpen;
      /*if (e->type == "mainMenu") {
          auto guiComp = e->get<GUIComponent>();
          if (guiComp != NULL) {
              std::cout << "main menu has GUI Component\n";
          }
          else {
              std::cout << "main menu has no GUI Component\n";
          }
      }*/
      if (frozen == false && isConsoleOpen == false && isLevelEditorOpen == false) {
        e->Update(L,&scene);
      }else {
        if (e->type == "player" && isConsoleOpen == false && isLevelEditorOpen == false) {
          e->Update(L,&scene);
        }
        if (e->type == "mainMenu" && isConsoleOpen == false && isLevelEditorOpen == false) {
            e->Update(L, &scene);
        }
      }
       bool coll = false;
       for (Entity * eTarget : entities) {
         auto projc = eTarget->get<ProjectileComponent>();
         
         if (projc != NULL) {
           for (Projectile * o : projc->objects) {
               
            // o->Draw(getMainCam());
            if (eTarget->type != e->type) {
              // float dist = sqrt(pow(e->pos.x-o->getPos().x,2)+
              //                    pow(e->pos.z-o->getPos().z,2)*1.0f);
              float dist3d = sqrt(pow(e->pos.x - o->transform.getPos().x, 2) +
                                  pow(e->pos.y - o->transform.getPos().y, 2) +
                                  pow(e->pos.z - o->transform.getPos().z, 2) * 1.0);
              // std::cout << "PROJECTILE Y: " << o->getPos().y << " MIN Y: " << e->projMin.y << "\n";
              if (e->getCollisionWithPoint(o->transform.getPos()) == true && e->type != eTarget->type && !e->dead) {
                if (e->canBeHit && frozen == false && o->destroy == false && e->canBeHit == true) {
                  coll = true;
                  o->destroy = true;
                  e->hitCount = 1;
                  eTarget->Hit(1,e);
                }
              }
            }
          }
        }
      }
      e->projColliding = coll;
    } // end of loop
    //std::cout << SDL_GetTicks() << "\n";

  }
  Entity* getEntity(std::string type) {
      for (Entity* e : entities) {
          if (e->type == type) {
              return e;
          }
      }
  }
  void runPrintCommand(std::vector<std::string> tokens) {
      std::string tempString;
      for (int i = 1; i < tokens.size(); i++) {
          tempString += tokens[i];
          tempString += " ";
      }
      tempString += "\n";
      commandLog += tempString;

  }
  void spawnEntityCommand(std::vector<std::string> tokens) {
      addEntity("res/scripts/entities.lua", tokens[1], L);
      commandLog += "added entity ";
      commandLog += tokens[1];
      commandLog += "\n";
  }
  void setSpeedCommand(std::vector<std::string> tokens) {
      for (Entity * e : entities) {
          if (e->type == "player") {
              e->mainSpeed = ::atof(tokens[1].c_str());
          }
      }
  }
  void killEntityCommand(std::vector<std::string> tokens) {
      for (Entity* e : entities) {
          if (e->type == tokens[1]) {
              e->kill();
              commandLog += "killed entity type: " + tokens[1] + "\n";
          }
      }
  }
  void GodModeCommand(std::vector<std::string> tokens) {
      for (Entity* e : entities) {
          if (e->type == "player") {
              e->godmode = true;
          }
      }
  }
  void ProjColorCommand(std::vector<std::string> tokens) {
      for (Entity* e : entities) {
          if (e->type == "player") {
              auto projc = e->get<ProjectileComponent>();
              float r = ::atof(tokens[1].c_str());
              float g = ::atof(tokens[2].c_str());
              float b = ::atof(tokens[3].c_str());
              projc->setColor(glm::vec4(r, g, b, 1.0f));
          }
      }
  }
  void printNumCommand(std::vector<std::string> tokens) {
      float temp = ::atof(tokens[1].c_str());
      commandLog += std::to_string(temp) + "\n";
  }
  void helpCommand(std::vector<std::string> tokens) {
      commandLog += "print\n";
      commandLog += "spawnEntity\n";
      commandLog += "setSpeed\n";
      commandLog += "killEntity\n";
      commandLog += "godmode\n";
      commandLog += "setProjColor\n";
      commandLog += "printNum\n";
  }
  void runCommand() {
      std::cout << "run command\n";
      std::string cmd = std::string(command);
      std::vector <std::string> tokens;
      stringstream check1(cmd);
      string intermediate;
      while (std::getline(check1, intermediate, ' '))
      {
          tokens.push_back(intermediate);
      }
            if (tokens[0] == "print") { runPrintCommand(tokens); 
      }else if (tokens[0] == "help") { helpCommand(tokens); 
      }else if (tokens[0] == "spawnEntity") { spawnEntityCommand(tokens); 
      }else if (tokens[0] == "setSpeed") { setSpeedCommand(tokens); 
      }else if (tokens[0] == "killEntity") { killEntityCommand(tokens); 
      }else if (tokens[0] == "godmode") { GodModeCommand(tokens); 
      }else if (tokens[0] == "setProjColor") { ProjColorCommand(tokens);
      }else if (tokens[0] == "printNum") { printNumCommand(tokens); }

  }
  void checkForConsole() {
      Uint8* keys;
      keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
      SDL_Event e;
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
//      SDL_PollEvent(&e);
      for (int i = 0; i <= 255; i++) {
          io.KeysDown[i] = false;
      }
      while (SDL_PollEvent(&e))
      {
          switch (e.type)
          {
          case SDL_TEXTINPUT:
              io.AddInputCharacter(e.text.text[0]);
              break;
          case SDL_KEYUP:
              io.KeysDown[e.key.keysym.scancode] = false;
              io.KeyCtrl = (e.key.keysym.mod & KMOD_CTRL) != 0;
              io.KeyShift = (e.key.keysym.mod & KMOD_SHIFT) != 0;
              break;
              // etc...
          case SDL_KEYDOWN:
              int key = e.key.keysym.sym & ~SDLK_SCANCODE_MASK;
              bool isKeyDown = (e.type == SDL_KEYDOWN);
              if (key == 8) {
                  //std::cout << "backspace\n";
                  io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = true;
              }
              if (key == 13) {
                  runCommand();
              }
          }
      }
      if (keys[SDL_SCANCODE_BACKSLASH]) {
          if (isConsoleOpen == true && canKey == true) {
              isConsoleOpen = false;
              canKey = false;
              SDL_SetRelativeMouseMode(SDL_TRUE);
          }
          else if (isConsoleOpen == false && canKey == true) {
              isConsoleOpen = true;
              canKey = false;
          }
      } else {
          canKey = true;
      }
      if (keys[SDL_SCANCODE_EQUALS]) {
          if (isLevelEditorOpen == true && canKey2 == true) {
              isLevelEditorOpen = false;
              canKey2 = false;
              SDL_SetRelativeMouseMode(SDL_TRUE);
          }
          else if (isLevelEditorOpen == false && canKey2 == true) {
              isLevelEditorOpen = true;
              canKey2 = false;
          }
      }
      else {
          canKey2 = true;
      }
  }
  
  void DrawASync(drawData data) {
      if (isConsoleOpen == false) {
          for (Prop* p : data.propsPointer) {
              p->Draw(getMainCam());
              p->Update();
          }
      }
      for (Entity* e : data.entitiesPointer) {
          auto projc = e->get<ProjectileComponent>();
          if (projc != NULL) {
              projc->Draw(getMainCam());
          }
      }
  }
  void DrawUI() {
      for (Entity* e : entities) {
          e->DrawUI();
      }
  }
  void Draw(double deltaTime) {
    if (isConsoleOpen == false) {
        for (Prop* p : props) {
            p->Draw(getMainCam());
            p->Update();
        }
    }
    for (Entity * e : entities) {
        auto projc = e->get<ProjectileComponent>();
        if (projc != NULL) {
            projc->Draw(getMainCam());
        }

      if (!isLevelEditorOpen) {
          e->Draw(getMainCam(), frozen);
      }else {
          float posX = -(e->pos.x * 3.4);
          float posY = (e->pos.z * 3.4);
      }
    }
    if (isConsoleOpen) { drawConsoleWindow();}
    if (isLevelEditorOpen) {
        updateLevelEditor();
        glDisable(GL_DEPTH_TEST);
        clickObject.Draw(getMainCam());
        clickObject.setScale(glm::vec3(3.0f));
        glEnable(GL_DEPTH_TEST);
    }
    
  }

  void drawText() {
      glPushMatrix();
      glLoadIdentity();
      for (Entity* e : entities) {
          e->DrawText();
      }
      glPopMatrix();
  }
  void setHeroTarget() {
    glm::vec3 target;
    for (Entity * e : entities) {
      e->isPaused = isConsoleOpen;
      auto camc = e->get<CameraComponent>();
      if (camc != NULL) {
        if (camc->isFirstPerson == true) {
          target = camc->camera.m_position;
          playerPos = target;
          break;
        }else {
          for (Entity * e2 : entities) {
            if (e2->type == "AIHero") {
              target = e2->pos;
              playerPos = target;
              break;
            }
          }
          break;
        }
      }
    }
    for (Entity * e : entities) {
      e->playerPos = target;
    }
  }
  Camera getMainCam() {
    for (Entity * e : entities) {
      auto camc = e->get<CameraComponent>();
      if (camc) {
        // playerPos = camc->getCamera().m_position;
          if (isTopDown) {
              return topDownCamera;
          }else {
              return camc->getCamera();
          }
      }
    }
    Camera badCam;
    //std::cout << "No entity with a camera" << '\n';
    return badCam;
  }
  bool scriptsLoaded = false;
  //bind lua functions for entities
  bool functionsAdded = false;
  //vector storing player pos.
  glm::vec3 playerPos;
  bool frozen = false;
  bool printDelta = false;
  bool firstCollisionUpdate = true;
  bool isConsoleOpen = false;
  bool isLevelEditorOpen = false;
  bool mouseOpen;
  char command[255] = "Input Command Here";
  char propEditor[255] = "";
  char entityEditor[255] = "";
  char mapPath[255] = "mainMap";

  int propSize = 0;
  int propRotation = 0;
  std::string commandLog = "type commands here.\n type help for command list \n";
  //Minimap minimap;
  //vector storing all entitys in this system
  std::vector<Entity*> entities;
  //vector storing all props in this system
  std::vector<Prop*> props;

  std::vector<entitySpawnData> spawnData;

  std::vector<Entity*> startEnt;
  glm::vec2 clickPos;
  std::string playerTag;
  Object clickObject;
  bool save = false;
  bool load = false;
  bool collideCheckbox = true;
  // cursor positions from level editor sliders
  int cursorPosX;
  int cursorPosY;
  // a map of commands being stored with std::string and a function pointer

private:
  std::string * text;
  // has collision been dumped
  bool dumped = false;
  // the collision scene for qu3e
  q3Scene scene;
  int count;
  int collisions = 0;
  double speedMultiplier = 1.0;
  double FPSToUse = 60;
  std::vector<double> FPS_List;
  //The collision box for the map floor
  Box floor;
  bool canKey = true;
  bool canKey2 = true;
  lua_State* L;
  //Camera for top down level editor
  Camera topDownCamera;
  bool isTopDown = true;
};
#endif
