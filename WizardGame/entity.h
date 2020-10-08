#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(suppress : 4996)
#include <iostream>
#include <map>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include <math.h>
#include <cmath>
#include "text.h"
#include <stdlib.h>
#include "animatedObject.h"
#include "luaLibrary.h"
#include "particle.h"
#include <LuaBridge/LuaBridge.h>
#include <memory>
#include "ObjectComponent.h"
#include "GraphicsComponent.h"
#include "StaticGraphicsComponent.h"
#include "NpcComponent.h"
#include "GUIComponent.h"
#include "CameraComponent.h"
#include "AudioComponent.h"
#include "Component.h"
#include <qu3e/q3.h>
#include "CollisionSystem.h"
#include "ProjComponent.h"
#include "Audio.h"
#include "HealthBar.h"
#include <map>
#include "Ability.h"
#include "Timer.h"
#include "Globals.h"
#ifndef ENTITY_H
#define ENTITY_H

extern glm::vec2 enemyTarget;
extern float brightness;
extern float cullDistance;
extern float red;
extern std::map<std::string, float> GlobalFloatsVars;
extern std::map<std::string, bool> GlobalBoolsVars;
extern glm::vec4 screenColor;
extern bool drawUI;
extern std::string playerTag;
extern std::vector<std::string> playerAbilities;
extern std::vector<std::string> playerAbilitiesDesc;
extern bool drawScene;
extern float Width;
extern float Height;
extern double FPS;
extern q3Scene* scenePointer;
extern lua_State * LPointer;
extern bool resetWindow;
extern bool resetFramebuffer;
extern bool resetCamera;
extern bool resetText;
extern bool resetScripts;
extern bool shakeScreen;

extern void setHighScore(int val);
extern int getHighScore();
extern void readEntityFile(lua_State* L);
static float getDistance(float x, float y, float x2,float y2) {
  float result = sqrt(pow(x-x2, 2) +
                  pow(y-y2, 2));
  return result;
}
class SpawnData {
public:
  SpawnData(std::string name, glm::vec2 pos) {
    this->pos = pos;
    this->name = name;
  }
  glm::vec2 pos;
  std::string name;
private:
};

class Entity {
public:
    //thanks stack exchange
    template <typename T> int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }
    glm::vec3 AnglesFromVectors(glm::vec3 forward, glm::vec3 up)
    {
        // Yaw is the bearing of the forward vector's shadow in the xy plane.
        float yaw = atan2(forward.y, forward.x);

        // Pitch is the altitude of the forward vector off the xy plane, toward the down direction.
        float pitch = -asin(forward.z);

        // Find the vector in the xy plane 90 degrees to the right of our bearing.
        float planeRightX = sin(yaw);
        float planeRightY = -cos(yaw);

        // Roll is the rightward lean of our up vector, computed here using a dot product.
        float roll = asin(up[0] * planeRightX + up[1] * planeRightY);
        // If we're twisted upside-down, return a roll in the range +-(pi/2, pi)
        if (up.z < 0)
            roll = sign(roll) * 3.14 - roll;

        return glm::vec3(yaw * 180 / 3.14, pitch * 180 / 3.14, roll * 180 / 3.14);
    }

  float sign (glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
  }
  bool PointInTriangle (glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3) { //Thanks stack overflow!
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
  }


  float RADS_TO_DEGS(float radians) {
    return radians * 180/3.1415926535;
  }
  float DEGS_TO_RADS(float degrees) {
    return degrees * 3.1415926535/180;
  }
  int FindTurnSide(int current, int target)
  {
    int diff = target - current;
    if(diff < 0) {
      diff += 360;
    }
    if(diff > 180) {
          return -1;
    } else {
          return 1;
    } 
  }
  // virtual Entity* clone() = 0;
  ~Entity() {
    for (auto& c : components) {
      delete c.second;
    }
    delete &emitter;
    std::cout << "deleted entity\n";

  }
  Entity(lua_State* L) : UpdateFunction(L), OnStartFunction(L), HitFunction(L), 
  //CollisionObject("./res/cube.obj",glm::vec4(0.0f,1.0f,0.0f,1.0f),"./res/basicShader",true),
  //                                                              CollisionObject2("./res/cube.obj",glm::vec4(1.0f,0.0f,0.0f,1.0f),"./res/basicShader",true),
                                                                emitter(glm::vec3(0.0), 100, 100, 1.0, true){
      Timer addingUICam = Timer("adding UI camera");
      UICam.InitCam(glm::vec3(0, 0, 0), 70, Width / Height, 0.01f, 1000.0f);
      UICam.Pitch(1.57);
  }
  void CompileLuaFunctions(lua_State* L) {
    std::string UpdateString = type + "_Update";
    UpdateFunction = luabridge::getGlobal(L,UpdateString.c_str());
    std::string OnStartString = type + "_Start";
    OnStartFunction = luabridge::getGlobal(L,OnStartString.c_str());
    std::string HitString = type + "_Hit";
    HitFunction = luabridge::getGlobal(L, HitString.c_str());
  }
  void Hit(int hits, Entity * ent) {
    if (!dead) {
      // std::cout << "YOU HIT A " << type << " AT " << pos.x << " " << pos.z << " with " << hits << " hits" << '\n';
      std::string HitString = type + "_Hit";
      try {
        //auto hit = luabridge::getGlobal(L,HitString.c_str());
          HitFunction(this,ent,hits);
      }catch(luabridge::LuaException const& e) {
        std::cout << "MISSING HIT FUNCTION..." << '\n';
      }
    }
  }
  void checkCulling() {
      if (allEntities != nullptr) {
          glm::vec3 playerPos = glm::vec3(0.0f);
          for (Entity* e : *allEntities) {
              if (e->type == "player") {
                  playerPos = e->pos;
                  break;
              }
          }
          float dist = getDistanceBetweenTwoPoints(glm::vec2(pos.x, pos.z), glm::vec2(playerPos.x, playerPos.z));
          if (dist < cullDistance) {
              cull = false;
          }
          else {
              cull = true;
          }
      }
  }
  void DrawText() {
      auto guiC = get<GUIComponent>();
      if (guiC != NULL) {
          guiC->Draw();
      }
  }
  void DrawUI() {
      auto gfxc = get<GraphicsComponent>();
      if (gfxc != NULL && gfxc->useUICam == true) {
          gfxc->Update(speedModifier);
          gfxc->Draw(UICam, hitCount);
      }
  }
  void Draw(Camera cam, bool f) {
    auto gfxc = get<GraphicsComponent>();
    auto objc = get<ObjectComponent>();
    if (objc != nullptr) {
        objc->Draw(&cam);
    }
    checkCullingCount++;
    if (checkCullingCount > 20) {
        checkCullingCount = 0;
        checkCulling();
    }
    
    emitter.drawParticles(cam);

    if (gfxc != NULL && !dead) {
      if (f == false && frozen == false && isPaused == false && gfxc->useUICam == false) {
        gfxc->Update(speedModifier);
      }
      if (isPaused != true) {
          if (gfxc->useUICam == false) {
              gfxc->Draw(cam, hitCount);   
          }
      }
    }
    auto sgfxc = get<StaticGraphicsComponent>();
    if (sgfxc != NULL && !dead) {
      sgfxc->Update();
      sgfxc->Draw(cam);
    }
    if (type != "player" && showHP == true) {
        bar.Draw(cam, pos + glm::vec3(0.0, projMax.y + 1.0, 0.0));
    }
  }
  bool getCollisionWithPoint(glm::vec3 point) {
    return  (point.x >= projMin.x-3.0f && point.x <= projMax.x+3.0f) &&
            (point.y >= projMin.y && point.y <= projMax.y) &&
            (point.z >= projMin.z-3.0f && point.z <= projMax.z+3.0f);
  }
  void Update(lua_State* L, q3Scene * scene) {
      
    emitter.updateParticles();
    pushInsideLevel();
    auto graphics = get<GraphicsComponent>();
    auto cameraC = get<CameraComponent>();
    auto sgraphics = get<StaticGraphicsComponent>();
    if (hitCount > 0.0) {
        hitCount -= 0.01;
    }
    textPointer = new std::string("DANK");
    if (godmode == true) {
        hp = 99;
    }
    if (type == "player") {
        red = getDistanceFromNearestEnemy();
        if (red < 40) {
            red /= 160.0;
            red = 0.25 - red;
        } else {
            red = 0.0;
        }
    }
    if (cameraC != NULL) {
      cameraC->camera.m_position = pos;
      cameraC->UpdateCamera();
    }
    if (hp < 1) {
      kill();
    }
    if (!readyToFire) {
      fireCount++;
      if (fireCount == 30) {
        readyToFire = true;
        fireCount=0;
      }
    }
    if (sgraphics != NULL) {
        if (pos.y < 0.0f) {pos.y = 0.0f;}
        if (sgraphics->color.x > 0.0) {sgraphics->color.x-=0.03;}
        if (sgraphics->color.y > 0.0) {sgraphics->color.y-=0.03;}
        if (sgraphics->color.z > 0.0) {sgraphics->color.z-=0.03;}
        if (sgraphics->color.x < 0.0) {sgraphics->color.x = 0.0;}
        if (sgraphics->color.y < 0.0) {sgraphics->color.y = 0.0;}
        if (sgraphics->color.z < 0.0) {sgraphics->color.z = 0.0;}
    }
    if (graphics != NULL) {
        
      if (graphics->colorFlash.x > 0.0) {graphics->colorFlash.x-=0.03;}
      if (graphics->colorFlash.y > 0.0) {graphics->colorFlash.y-=0.03;}
      if (graphics->colorFlash.z > 0.0) {graphics->colorFlash.z-=0.03;}
      if (graphics->colorFlash.x < 0.0) {graphics->colorFlash.x = 0.0;}
      if (graphics->colorFlash.y < 0.0) {graphics->colorFlash.y = 0.0;}
      if (graphics->colorFlash.z < 0.0) {graphics->colorFlash.z = 0.0;}
      projMin = glm::vec3(graphics->object.transform.GetModelColl() * glm::vec4(Min, 1.0));
      projMax = glm::vec3(graphics->object.transform.GetModelColl() * glm::vec4(Max, 1.0));
      if (projMin.x > 0.0f) {projMin.x = -projMin.x;}
      if (projMin.y > 0.0f) {projMin.y = -projMin.y;}
      if (projMin.z > 0.0f) {projMin.z = -projMin.z;}

      if (projMax.x < 0.0f) {projMax.x = -projMax.x;}
      if (projMax.y < 0.0f) {projMax.y = -projMax.y;}
      if (projMax.z < 0.0f) {projMax.z = -projMax.z;}
      projMin+=pos;
      projMax+=pos;
      glm::vec3 tempMin = projMin;
      // if (projMin.x > projMax.x) {projMin.x = projMax.x; projMax.x = tempMin.x;}
      // if (projMin.y > projMax.y) {projMin.y = projMax.y; projMax.y = tempMin.y;}
      // if (projMin.z > projMax.z) {projMin.z = projMax.z; projMax.z = tempMin.z;}

      projMin.y += projMax.y;
      projMax.y += projMax.y;

      if (type != "player") {
        graphics->setPos(pos);
        graphics->setRot(rot);
        graphics->setScale(scale);
        // std::cout << "scale X: " << scale.x << "scale Y: " << scale.y << "scale Z: " << scale.z << std::endl;
      }else {
        graphics->setPos(glm::vec3(pos.x,pos.y-15,pos.z));
      }
      min = graphics->object.min;
      max = graphics->object.max;
    }else {
      projMin = (Min+pos);
      projMax = (Max+pos);

      projMin.y += scaleColl.y;
      projMax.y += scaleColl.y;
    }
    if (sgraphics != NULL) {
      pos = glm::vec3(startingPos.x, 0.0f, startingPos.y);
      sgraphics->object.setPos(glm::vec3(startingPos.x,0.0f,startingPos.y));
      sgraphics->object.setRot(glm::vec3(sgraphics->object.getRot().x,rot.y,sgraphics->object.getRot().z));
    }
    std:string UpdateString = type + "_Update";
    try {
      if (!dead && !frozen) {
          {
              UpdateFunction(this);
          }
        
      }

    }catch(luabridge::LuaException const& e) {
      std::cout << "UPDATE FAILED ON ENTITY: " << type << '\n';
    }
    vel = pos-lastPos;
    
  }

  void DumpToFile() {
    #pragma warning(push)
    #pragma warning(disable: 4996) //4996 for _CRT_SECURE_NO_WARNINGS equivalent

    FILE * pFile;
    char buffer [100];
    pFile = fopen("res/scripts/dumps/playerDump.txt","w+");
    if (pFile == NULL) {
      std::cout << "error opening file..." << '\n';
    }
    if (type == "player") {
      collider.body->Dump(pFile,1);
    }
    #pragma warning(pop)
  }
  void OnStart(lua_State* L) {
    try {
      // auto OnStart = luabridge::getGlobal(L,OnStartString.c_str());

      OnStartFunction(this);
      if (type == "player") {
        std::cout << "Player Started" << std::endl;
      }
    }catch(luabridge::LuaException const& e) {
      std::cout << "ON START FAILED" << '\n';
    }
  }
  void setUpCollider(q3Scene * scene, glm::vec3 scale) {
    if (hasCollision) {
          if (type != "player") {
              collider.init(scale, glm::vec3(0.0f, 0.0f, 0.0f), scene, eDynamicBody);
          }
          else {
              collider.init(scale, glm::vec3(0.0f, 0.0f, 0.0f), scene, eDynamicBody);
          }
    }
    else {
        collider.init(scale, glm::vec3(500.0f, 500.0f, 0.0f), scene, eDynamicBody);
    }
  }
 
  float Round(float x) {
      return round(x);
  }
  void setCanBeHit(bool val) {
    this->canBeHit = val;
  }
  bool getCanBeHit() {
      return canBeHit;
  }
  void setFloat(std::string name, float value) {
    FloatsVars[name] = value;
  }
  float getFloat(std::string name) {
    return FloatsVars[name];
  }
  void setGlobalFloat(std::string name, float value) {
      GlobalFloatsVars[name] = value;
  }
  float getGlobalFloat(std::string name) {
      return GlobalFloatsVars[name];
  }
  void setGlobalBool(std::string name, bool value) {
      GlobalBoolsVars[name] = value;
  }
  bool getGlobalBool(std::string name) {
      return GlobalBoolsVars[name];
  }
  void setString(std::string name, std::string value) {
    StringVars[name] = value;
  }
  std::string getString(std::string name) {
    return StringVars[name];
  }

  void setBool(std::string name, bool value) {
    BoolVars[name] = value;
  }
  bool getBool(std::string name) {
    return BoolVars[name];
  }

  void UpdateProj(std::vector<glm::vec2> proj) {
    this->proj = proj;
  }


  void addComponent(std::type_index type, Component* c) {
    components[type] = c;
  }
  void setCamPos(glm::vec3 p) {
    auto cameraC = get<CameraComponent>();
    cameraC->setPos(p);
  }
  glm::vec3 getCamPos() {
    auto cameraC = get<CameraComponent>();
    return cameraC->getCamera().m_position;
  }
  glm::vec3 getCamForward() {
    auto cameraC = get<CameraComponent>();
    return cameraC->getCamera().m_forward;
  }
  glm::vec3 getForward() {
    auto cameraC = get<CameraComponent>();
    if (cameraC != NULL) {
        return cameraC->camera.m_forward;
    } else {
        static const glm::vec3 UP(0.0f, 1.0f, 0.0f);
        glm::mat4 rotation = glm::rotate(rot.y, UP);
        glm::vec3 forward = glm::vec3(glm::normalize(rotation * glm::vec4(glm::vec3(0, 0, 1), 0.0)));
        return forward;
    }
  }
  void DrawProj(Camera camera) {
    auto projC = get<ProjectileComponent>();
    projC->Draw(camera);
    std::cout << "Drew Projectiles" << std::endl;
  }
  //API FUNCTIONS
  void setAchievement(std::string name);
  float sinFunc(float x);
  int getHighscore();
  void setHighscore(int val);
  void setScreenShake(bool val) { shakeScreen = val; }
  void setObjectModel(std::string model);
  void setScreenResolution(float width, float height);
  void setMouseCapture(bool value);
  void setImageDraw(std::string tag, bool draw);
  void setImageTransform(float x, float y, float width, float height);
  void setImage(std::string tag, std::string src);
  void setTextSize(int textSize);
  void setProjRange(int range);
  float getObjectX();
  float getObjectY();
  float getObjectZ();
  void setEnemyTarget(float x, float y);
  void setObjectPos(float x, float y, float z);
  void setObjectScale(float sc);
  void setObjectColor(float r, float g, float b, float a);
  void setProjColor(float r, float g, float b, float a);
  void setProjDelay(int del);
  void FireMultiple(int num);
  void FireInstant();
  void setSpread(float s);
  void clearAbilities() { playerAbilities.clear(); playerAbilitiesDesc.clear();}
  int getAbilityCount() { return playerAbilities.size(); }
  void stopProgram();
  void setDrawScene(bool val);
  void setScreenColor(glm::vec4 ScreenColor);
  void addAbility(std::string ability, std::string description);
  std::string getAbilityDescription(int val);
  std::string getAbility(int val);
  void swapMap() { changeMap = true; }
  void setMapTarget(std::string val) { map = val; resetScripts = true; }
  float getArrowDirX() { return arrowDirection.x; }
  float getArrowDirY() { return arrowDirection.y; }
  float getMoveDirX() { return moveDirection.x; }
  float getMoveDirY() { return moveDirection.y; }
  void setTextColor(float r, float g, float b, float a);
  int getEntityCount();
  void setText(std::string tag, std::string text, float x, float y);
  std::string getTypeLua();
  float getDistanceFromNearestNot(std::string targetType);
  void setHPColor(float r, float g,  float b);
  void showHealth(bool value);
  int getAnimationID(std::string tag);
  std::string getDefaultAnim();
  void setCollide(bool enable);
  void playSound(std::string file);
  void setParticleSpread(float spread);
  void setParticleModel(std::string model);
  void setHue(float hue);
  void setSaturation(float saturation);
  void setValue(float value);
  std::string getPlayerTag();
  void resetFrame();
  void setPlayerTag(std::string tag);
  void copyPlayerRot();
  void setRot(float x, float y, float z);
  float getProjCount();
  void setProjCount(float count);
  void setBrightness(float b);
  void damageNearest(int damage);
  int damageWithinADistance(int damage, int distance);
  void damageNearestEnt(std::string ent, int damage);
  float getDefaultSpeed() { return mainSpeed; }
  void setUIText(std::string text);
  void Delay(int milliseconds);
  void setCollisionBox(float width, float height, float length);
  float getDistanceBetweenTwoPointsAPI(float x, float y, float x2, float y2);
  float getLastX();
  float getLastY();
  float getLastZ();
  void UpdateKeyPresses();
  void setDamageAnimation(int anim);
  void playDamageAnimation();
  void restartCollisionBox();
  void Shake(float amount);
  void setInverted(int i);
  float getDistanceBetweenTwoPoints(glm::vec2 pos1, glm::vec2 pos2);
  void setGlobalFrozen(bool f);
  Entity* getNearestEntWithName(std::string entityName);
  Entity* getNearestEntity();
  bool getGlobalFrozen();
  void setFrozen(bool f);
  bool getFrozen();
  float lookAtNearest(std::string targetType);
  bool doesEntityExist(std::string targetType);
  float getDistanceFromNearest(std::string targetType);
  float getDistanceFromNearestEnt();
  float getDistanceFromNearestEnemy();
  float getPositionFromNearestX(std::string targetType);
  float getPositionFromNearestY(std::string targetType);
  void isFirstPerson(bool firstPerson);
  void setFOV(float FOV);
  int getAnimFrame();
  void playAnimation(int id);
  void spawnEntity(std::string name, float x, float y);
  void killProgram();
  void kill();
  float getPlayerDistance();
  void write(const std::string& str);
  void setScale(float scale);
  float random(float min, float max);
  int randomInt(int min, int max);

  void lookAtPlayer();
  void lookAt(float x, float y);
  void Strafe(float amount);
  void moveForward(float amount);
  void moveBackward(float amount);
  void setAnimation(int id);
  int getAnimation();
  void setPos(float x, float y, float z);
  float getX();
  float getY();
  float getZ();
  void setColorFlash(float r, float g, float b, float a);
  void setColor(float r, float g, float b, float a);
  float getKeyDirectionX();
  float getKeyDirectionY();
  std::string getKeyPressed();
  void TopDownUpdate();
  void FPSControllerUpdate(float speed);
  float getHP();
  void setHP(float hp);
  void addCounter(int start);
  bool updateCounter(int counter,int end);
  int getCount();
  void setCount(int count);
  bool Fire();
  bool isPlayer();
  void pushInsideLevel();
  void setAnimationTag(std::string tag, int animation);
  void playAnimationTag(std::string tag);
  bool isAnimationPlaying(std::string tag);
  void Damage(int numHits);
  bool getPaused() { return isPaused; }
  void setPaused(bool val) { isPaused = val; }
  void Emit(int num, float r, float g, float b, float a);
  bool hasAnimation(std::string anim);
  void setUICam(bool val);
  void setDrawUI(bool val);
  //END API FUNCTIONS

  template <typename T>
  T* get() {
    auto it = components.find(std::type_index(typeid(T)));
    if (it != components.end()) {
      return dynamic_cast<T*>(it->second);
    }
    return nullptr;
  }
  void setType(const std::string& type) {
    this->type = type;
  }
  std::string getType() const {
    return type;
  }

  glm::vec3 getPlayerRot() {
    auto cameraC = get<CameraComponent>();
    return cameraC->getCamera().m_forward;
  }
  void setPlayerPos(glm::vec3 playerPos) {
    this->playerPos = playerPos;
  }
  bool showHP = true;
  int collisionUpdateCounter = 0;
  float nearestEntityDist;
  std::string nearestEntityType;
  glm::vec3 nearestEntityPos;
  std::string keypressed;
  glm::vec3 playerPos;
  bool readyToFire = true;
  int fireCount = 0;
  std::vector<Entity*> * entities;
  bool dead = false;
  bool killed = false;
  bool firing = false;
  bool canFire = true;
  std::string type;
  glm::vec3 min;
  glm::vec3 max;
  bool projColliding = false;
  glm::vec3 vel;
  glm::vec3 lastPos;
  glm::vec3 lastPosStatic;
  glm::vec3 pos;
  glm::vec3 rot;
  std::string fire_type = "fireball";
  float hp;
  int count = 0;
  std::vector<int> counters;
  std::vector<std::string> spawnedEntityNames;
  std::vector<glm::vec2> spawnedEntityPosition;
  std::vector<SpawnData> spawnedEntityData;
  Box collider;
  std::map<std::string,float> FloatsVars;
  std::map<std::string,bool> BoolVars;
  std::map<std::string,std::string> StringVars;
  bool canBeHit = true;
  int countSHIELD = 0;
  std::vector<Entity*> * allEntities;
  glm::vec3 testVel;
  float scale;
  bool hasCollisionError = false;
  bool globalFrozen = false;
  bool frozen = false;
  bool firstColl = false;
  double speedModifier = 1.0;
  luabridge::LuaRef UpdateFunction;
  luabridge::LuaRef OnStartFunction;
  luabridge::LuaRef HitFunction;
  q3Scene * scenePointer;
  glm::vec3 lastCollSize = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 projMin = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 projMax = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 projPos = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 projScale = glm::vec3(0.0f,0.0f,0.0f);
  float largestScale = 0.0f;
  glm::vec3 projPosDiff = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec2 startingPos = glm::vec2(0.0f,0.0f);
  glm::vec3 Min = glm::vec3(-5.0f,-5.0f,-5.0f);
  glm::vec3 Max = glm::vec3(5.0f, 5.0f, 5.0f);
  bool moved = false;
  glm::vec3 scaleColl = glm::vec3(5.0f,5.0f,5.0f);
  int damageAnim = 0;
  int keyCount = 0;
  lua_State* LPointer;
  glm::vec3 playerVel;
  std::string * textPointer;
  bool modText = false;
  bool isPaused;
  float mainSpeed = 0.6f;
  bool godmode = false;
  Emitter emitter;
  int emitCount = 0;
  bool hasCollision = false;
  glm::vec2 velocity;
  Sound sound = Sound("./res/sounds/shoot.wav");
  HealthBar bar;
  float maxHP = -1.0;
  glm::vec2 moveDirection;
  glm::vec2 arrowDirection;
  glm::vec4 currentColor = glm::vec4(0.0,0.0,0.0,1.0);
  std::string map;
  bool changeMap = false;
  glm::vec3 lastCameraUp = glm::vec3(0.0,0.0,1.0);
  bool hasOnStart = false;
  float hitCount = 0;
protected:
private:
  int checkCullingCount = 0;
  bool cull = false;
  Uint8* keys;
  std::map<std::type_index, Component*> components;
  // LuaEntityHandle handle;
  float lastDist = 0.0f;
  bool countData = false;
  
  SDL_Event e;
  std::vector<glm::vec2> proj;
  Camera UICam;
};
template <typename T>
void addComponent(Entity* e, luabridge::LuaRef& componentTable) {
  e->addComponent(std::type_index(typeid(T)), new T(componentTable));
}
static Entity* loadEntity(lua_State* L, const std::string& type) {  
  Timer entityConstructor = Timer("entity constructor");
  auto e = new Entity(L);
  entityConstructor.Stop();
  e->setType(type);
  //e->addFunctions(L);
  
  luabridge::LuaRef entityRef = luabridge::getGlobal(L,type.c_str());
  if (e->type == "mainMenu") {
      //janky workaround to a bug related to going to the main menu
      Timer timer("adding GUI component");
      luabridge::LuaRef guiTable = entityRef[0];
      if (e->type == "mainMenu") {
          std::cout << "adding gui component to main menu\n";
      }
      addComponent<GUIComponent>(e, guiTable);
  }
  for (int i = 0; i < entityRef.length(); ++i) {
      
    std::string componentName = entityRef[i+1]["componentName"];
    std::cout << "component name is " << componentName << "\n";
    if (componentName == "GraphicsComponent") {
        Timer timer("adding graphics component");
        luabridge::LuaRef gcTable = entityRef[i+1];
        addComponent<GraphicsComponent>(e,gcTable);
    }if (componentName == "StaticGraphicsComponent") {
        Timer timer("adding static graphics component");
        luabridge::LuaRef sgcTable = entityRef[i+1];
        addComponent<StaticGraphicsComponent>(e,sgcTable);
    }else if (componentName == "GUIComponent") {
        Timer timer("adding GUI component");
        luabridge::LuaRef guiTable = entityRef[i+1];
        if (e->type == "mainMenu") {
            std::cout << "adding gui component to main menu\n";
        }
        addComponent<GUIComponent>(e,guiTable);
    }else if (componentName == "CameraComponent") {
        Timer timer("adding camera component");
        luabridge::LuaRef camTable = entityRef[i+1];
        addComponent<CameraComponent>(e,camTable);
    }else if (componentName == "BasicComponent") {
        Timer timer("adding basic component");
        luabridge::LuaRef basicTable = entityRef[i+1];
        // e->onStartFunc = basicTable["onStart"];
    }else if (componentName == "ProjComponent") {
        Timer timer("adding projectile component");
        luabridge::LuaRef camTable = entityRef[i + 1];
        addComponent<ProjectileComponent>(e, camTable);
    }else if (componentName == "ObjectComponent") {
        Timer timer("adding object component");
        luabridge::LuaRef objTable = entityRef[i+1];
        addComponent<ObjectComponent>(e,objTable);
    }else if (componentName == "CollisionComponent") {
        Timer timer("adding collision component");
        luabridge::LuaRef collisionTable = entityRef[i+1];
        luabridge::LuaRef scaleTable = collisionTable["scale"];
        luabridge::LuaRef x = scaleTable["x"];
        luabridge::LuaRef z = scaleTable["y"];
        luabridge::LuaRef y = scaleTable["z"];
        e->Min.x = -x.cast<float>();
        e->Min.y = -y.cast<float>();
        e->Min.z = -z.cast<float>();
        e->Max.x = x.cast<float>();
        e->Max.y = y.cast<float>();
        e->Max.z = z.cast<float>();
        e->scaleColl = glm::vec3(x.cast<float>(),z.cast<float>(),y.cast<float>());
        e->hasCollision = true;
    }
    else if (componentName == "AuidioComponent") {
        Timer timer("adding audio component");
        luabridge::LuaRef audioTable = entityRef[i + 1];
        addComponent<AudioComponent>(e, audioTable);

    }
    // std::cout << "Added " << componentName << " to " << type << '\n';
  }
  Timer t = Timer("compiling lua function");
  e->CompileLuaFunctions(L);
  return e;
}

#endif
