#ifndef AOBJECT_H
#define AOBJECT
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <experimental/filesystem>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "obj_loader.h"
#include "transform.h"
#include "animatedMesh.h"
#include <future>

namespace fs = std::experimental::filesystem;
extern std::map<std::string, std::vector<animatedMesh*>> storedObjectData;
class animatedObject {
public:
  void eraseSubStr(std::string & mainStr, const std::string & toErase){
    size_t pos = mainStr.find(toErase);
    if (pos != std::string::npos){
      mainStr.erase(pos,toErase.length());
    }
  }
  animatedObject () {};
  void reset(const std::string& fileName, glm::vec4 c) {
    color = c;
    //shader.InitShader("./res/aShader");
    transform.m_scale = glm::vec3(0.3f,0.3f,0.3f);
    transform.m_rot = glm::vec3(-1.57f,0.0f,0.0f);
    transform.m_pos = glm::vec3(0.0f,-2.0f,0.0f);
    /*for (const auto & entry : fs::directory_iterator(fileName)) {
      std::string temp = absolute(entry.path()).string();
      eraseSubStr(temp,fs::current_path().string());
      eraseSubStr(temp,"/");
      
      if (temp[0] == '\\') {
          temp.erase(0, 1);
      }
      if (temp.find(".png") == std::string::npos) {
        files.push_back(temp);
        animations.push_back(new animatedMesh(temp));
      }else {
        tex.InitTex(temp);
        std::cout << "INIT MESH TEXTURE" << std::endl;
      }

    }*/
    folder = fileName;
  }
  void calculateMesh() {
    if (storedObjectData.find(folder) != storedObjectData.end()) {
      animations = storedObjectData[folder];
    }else {
      storedObjectData[folder] = animations;
    }
    calculatedMesh = true;
  }
  animatedObject (std::string fileName, glm::vec4 c) {
    color = c;
    shader.InitShader("./res/aShader");
    // shaderOutline.InitShader("./res/outline");
    transform.m_scale = glm::vec3(0.3f,0.3f,0.3f);
    transform.m_rot = glm::vec3(-1.57f,0.0f,0.0f);
    transform.m_pos = glm::vec3(0.0f,-2.0f,0.0f);
    for (const auto & entry : fs::directory_iterator(fileName)) {
      std::string temp = entry.path().string();
      eraseSubStr(temp,fs::current_path().string());
      eraseSubStr(temp,"/");
      //animatedMesh tempMesh = animatedMesh("./res/char/charNew.dae");
      files.push_back(temp);
      animations.push_back(new animatedMesh(temp));

    }
    folder = fileName;
  }
  void init() {
      shader.InitShader("./res/aShader");
      for (const auto& entry : fs::directory_iterator(folder)) {
          std::string temp = absolute(entry.path()).string();
          eraseSubStr(temp, fs::current_path().string());
          eraseSubStr(temp, "/");
          if (temp[0] == '\\') {
              temp.erase(0, 1);
          }
          if (temp.find(".png") == std::string::npos) {
              files.push_back(temp);
              animations.push_back(new animatedMesh(temp));
          }
          else {
              tex.InitTex(temp);
              std::cout << "INIT MESH TEXTURE" << std::endl;
          }
      }
  }
  void Draw(Camera camera, int id) {
      if (hasInit == false) {
          init();
          hasInit = true;
      }
      if (calculatedMesh == false) {
          return;
      }
    shader.Bind(color, colorFlash);
    tex.Bind(0);
    shader.Update(transform, camera);
    animations[id]->Draw();
    tex.UnBind();
    shader.UnBind();
  }
  bool Update(int id, double speedModifier) {
      if (animations.size()<=0) {
          return false;
    }
    duration = animations[id]->Duration;
    if (id != lastAnimID) {
      lastAnimID = id;
      auto calculate = std::async(&animatedObject::calculateMesh, this);

//      calculateMesh();
    }
    if (calculatedMesh == false) {
        return false;
    }

    frame++;

    //if (isSlow == true) {
    //    frame+=3;
    //}
    //else {
    //}
    bool stopped = animations[id]->Update(frame);
    if (stopped) {
      frame = 0;
      // animations[id]->frame = 0;
    }
      minNoPos = glm::vec3(transform.GetModelColl() * glm::vec4(noTransMin, 1.0));
      maxNoPos = glm::vec3(transform.GetModelColl() * glm::vec4(noTransMax, 1.0));
      min = glm::vec3(transform.GetModelColl() * glm::vec4(noTransMin, 1.0));
      max = glm::vec3(transform.GetModelColl() * glm::vec4(noTransMax, 1.0));

      // minNoPos = noTransMin;
      // maxNoPos = noTransMax;
      // min = noTransMin;
      // max = noTransMax;


    idStored = id;
    return stopped;
  }
  void runDebug() {
    std::cout << "folder: " << folder << "\n";
    //animations[idStored]->runDebug();
  }
  glm::vec3 getPos() {return transform.getPos();}
  glm::vec3 getRot() {return transform.getRot();}
  glm::vec3 getScale() {return transform.getScale();}
  void setScale(glm::vec3 scale) {transform.setScale(scale);}
  void setRot(glm::vec3 rot) {transform.setRot(rot);}
  void setPos(glm::vec3 position) {transform.setPos(position);}
  void setModel(std::string fileName);
  void setColor(glm::vec4 color) { this->color = color; }
  void setColorFlash(glm::vec4 colorFlash) { this->colorFlash = colorFlash; }
  glm::vec4 color;
  glm::vec4 colorFlash;
  Transform transform;

  glm::vec3 noTransMin = glm::vec3(-4.9751f, 0.0f, 0.7091f);
  glm::vec3 noTransMax = glm::vec3(4.896242f, 10.493537f,-1.040303f);

  glm::vec3 min = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 max = glm::vec3(0.0f,0.0f,0.0f);

  glm::vec3 minNoPos = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 maxNoPos = glm::vec3(0.0f,0.0f,0.0f);

  int lastAnimation = 0;
  int frame = 0;
  std::string folder;
  int lastAnimID = 99;
  Texture tex;
  int duration = 0;
  int idStored = 0;
  bool isSlow = false;
  bool hasInit = false;
  bool calculatedMesh = false;
protected:
private:
const double maxFPS = 60.0;
const double maxPeriod = 1.0/maxFPS;
double lastTime = 0.0;
double time;
Shader shader;
glm::vec3 rotation;
Shader shaderOutline;
// std::string folder;
std::vector<animatedMesh*> animations;
std::vector<std::string> files;
};
#endif
