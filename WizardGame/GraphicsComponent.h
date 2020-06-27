#ifndef GCOMPONENT_H
#define GCOMPONENT_H
#include <map>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include "animatedObject.h"

#include <LuaBridge/LuaBridge.h>
#include "Component.h"

class GraphicsComponent : public Component {
public:
  GraphicsComponent(luabridge::LuaRef& componentTable) {

    auto folderRef = componentTable["folder"];


    if (folderRef.isString()) {
      setFolder(folderRef.cast<std::string>());

    }else{
      std::cout << "Error, Folder Name is not a string!" << '\n';
    }
    std::cout << " created entity: " << "\n";

  }
  void setFolder(const std::string& folder) {
    this->folder = folder;
    std::cout << "hasent run object\n";
    object.reset(this->folder, colorFlash);
    std::cout << "run object\n";

  }
  std::string getFolder() const {
    return folder;
  }
  void setAnimID(int animID) {
    this->animID = animID;
  }
  int getAnimID() {
    return animID;
  }
  void Draw(Camera camera) {
    object.Draw(camera, animID);
  }
  void Update(double speedModifier) {
    object.Update(animID, speedModifier);
    frame = object.frame;
    //std::cout << "playing default: " << playingDefault << "\n";
    if (playingDefault == true) {
      animID = AnimationTags["default"];
    }else {
      testingCount++;
    }
    if (frame >= object.duration) {
        playingDefault = true;
    }
    //std::cout << "duration: " << object.duration << "\n";
    object.setColor(color);
    object.setColorFlash(colorFlash);
  }
  void playAnim(int id) {
    if (id != animID) {
      lastAnimID = animID;
      animID = id;
      NonLoop = true;
      frame = 0;
    }
  } 
  void resetFrame() {
    object.frame = 0;
  }
  void playAnimTag(std::string tag) {
    playingDefault = false;

    animID = AnimationTags[tag];
    testingCount = 0;
  }
  void UpdateSlow(double speedModifier) {
      object.isSlow = true;
      if (slowCount == 4) {
          object.Update(animID, speedModifier);
          slowCount = 0;
      }
      frame = object.frame;
      //std::cout << "playing default: " << playingDefault << "\n";
      if (playingDefault == true) {
          animID = AnimationTags["default"];
      }
      else {
          testingCount++;
      }
      if (frame >= object.duration) {
          playingDefault = true;
      }
      //std::cout << "duration: " << object.duration << "\n";
      object.setColor(colorFlash);
      object.isSlow = false;
      slowCount++;
  }
  void UpdateSlowSlow(double deltaTime) {
    // if (slowCount == 3) {
    //   object.Update(deltaTime*4.0,animID);
    //   slowCount = 0;
    // }else {
    //   slowCount++;
    // }
  }
  bool isAnimationPlaying(std::string tag) {
    return (AnimationTags[tag] == animID);
  }
  void setAnimationWithTag(std::string tag, int id) {
    AnimationTags[tag] = id;
  }
  void runDebug() {
    //std::cout << "Zombie model is: " << folder << "\n";
    object.runDebug();
  }
  void setColorFlash(glm::vec4 color) {
    this->colorFlash = color;
    //object.setColor(color);
  }
  glm::vec4 getColorFlash() {
    return colorFlash;
  }
  void setColor(glm::vec4 color) {
      this->color = color;
  }
  glm::vec4 getColor() {
      return color;
  }
  void setPos(glm::vec3 pos) {
    object.setPos(pos);
  }
  glm::vec3 getPos() {
    return object.getPos();
  }
  void setRot(glm::vec3 rot) {
    object.setRot(rot);
  }
  glm::vec3 getRot() {
    return object.getRot();
  }
  void setScale(float scale) {
    object.setScale(glm::vec3(scale,scale,scale));
  }
  bool NonLoop = false;
  animatedObject object;
  int lastAnimID;
  int frame;
  bool playingDefault = true;
  glm::vec4 colorFlash = glm::vec4(0.0f,0.7f,1.0f,1.0f);
  glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    std::string folder;

protected:
private:
  std::string defaultAnim;
  int slowCount = 0;
  int animID = 0;

  int testingCount = 0;

  std::map<std::string, int> AnimationTags;
};
#endif
