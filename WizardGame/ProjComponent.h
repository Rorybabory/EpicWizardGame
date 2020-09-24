#ifndef PCOMPONENT_H
#define PCOMPONENT_H
#include <map>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include "animatedObject.h"
#include <LuaBridge/LuaBridge.h>
#include "Component.h"
#include "Projectile.h"
extern q3Scene* scenePointer;
struct ProjData {
    Mesh mesh;
    Shader shader;
};
class ProjectileComponent : public Component {
public:
    float yawFromForward(glm::vec3 forward)
    {
        return atan2(forward.z, forward.x);
    }
    float random(float min, float max) {
        float random = ((float)rand()) / (float)RAND_MAX;
        float range1 = max - min;
        return (random * range1) + min;
    }
  ProjectileComponent(luabridge::LuaRef& componentTable) : trailShader("./res/basicShader"){
    auto modelRef = componentTable["model"];
    auto colorRRef = componentTable["colorR"];
    auto colorGRef = componentTable["colorG"];
    auto colorBRef = componentTable["colorB"];
    auto speedRef = componentTable["speed"];
    auto rangeRef = componentTable["range"];
    auto heightRef = componentTable["height"];
    auto delayRef = componentTable["delay"];
    if (modelRef.isString() ||
     colorRRef.isNumber() ||
      colorGRef.isNumber() ||
       colorBRef.isNumber() ||
        speedRef.isNumber() ||
        rangeRef.isNumber() ||
        delayRef.isNumber() ||
        heightRef.isNumber()) {
      setColor(glm::vec4(colorRRef.cast<float>(),colorGRef.cast<float>(),colorBRef.cast<float>(),1.0f));
      setSpeed(speedRef.cast<float>());
      setModel(modelRef.cast<std::string>());
      setRange(rangeRef.cast<float>());
      height = heightRef.cast<float>();
      delay = delayRef.cast<int>();
    }else{
      std::cout << "ERROR: PROJECTILE DATA IS INVALID TYPE..." << '\n';
    }
    ObjectFile = new Object(model, color, "./res/projShader", glm::vec3(0, 0, 0), false);
    ObjectFile->setScale(glm::vec3(3.0f, 3.0f, 3.0f));
  }
  void setModel(std::string model) {
    this->model = model;
  }
  void Draw(Camera camera) {
    if (!objects.empty()) {
      //projShader.Bind(color, glm::vec4(0.0));
      ObjectFile->shader.Bind(color, glm::vec4(0.0));
      glLineWidth(10.5);
      for (int i = 0; i < objects.size(); i++) {
          ObjectFile->Draw(camera, objects[i]->transform);      }

      ObjectFile->shader.UnBind();
      //trailShader.Bind();
      //trailShader.Update();
      //for (int i = 0; i < objects.size(); i++) {
      //    glColor3f(color.r, color.g, color.b);
      //    glBegin(GL_LINES);
      //    for (glm::vec3 pos : objects[i]->lastPos) {
      //        glVertex3f(pos.x, pos.y, pos.z);
      //    }
      //    glEnd();

      //}
      //trailShader.UnBind();
    }
  }
  glm::vec3 RadiansToFront(glm::vec3 radians) {
    glm::vec3 front;
    front.x = cos(radians.y) * cos(radians.z);
    front.y = sin(radians.y);
    front.z = cos(radians.y) * sin(radians.z);

    return glm::normalize(front);
  }
  glm::vec3 FrontToRadians(glm::vec3 front) {
    glm::vec3 radians;
    radians.y = sin(front.x);
    radians.x = cos(front.x);
    radians.z = cos(front.x);
    return radians;
  }
  // glm::vec3 makeRadians() {
  //
  // }
  void Update() {
    for (int i = 0; i < objects.size(); i++) {
          glm::vec3 posToSet = glm::vec3(objects[i]->projB.getPos().x, objects[i]->projB.getPos().y, objects[i]->projB.getPos().z);
          objects[i]->transform.setPos(posToSet);
          //objects[i]->lastPos.push_back(objects[i]->transform.getPos());
          //objects[i]->recordPosCount++;
          //if (objects[i]->recordPosCount > 12) {
          //    
          //    objects[i]->recordPosCount = 0;
          //}
          if (objects[i]->destroy == false) {
              objects[i]->delay++;
              if (objects[i]->delay > range) {
                  objects[i]->delay = 0;
              }
          }
          objects[i]->counter++;
          if (objects[i]->counter > range - 30) {
              objects[i]->transform.m_scale = glm::vec3(objects[i]->transform.getScale().x - 0.04);
          }
          if (objects[i]->transform.m_scale.x < 0.0) {
              objects.erase(objects.begin()+i);
              objects[i]->destroy = true;
          }
          if (objects[i]->destroy == true) {
              objects[i]->counter2++;
              if (objects[i]->counter2 >= 2) {
                  objects[i]->boxDestroy = true;
              }
          }
    }
  }
  void collUpdate() {
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->projB.body->SetTransform(q3Vec3(objects[i]->transform.getPos().x, objects[i]->transform.getPos().y, objects[i]->transform.getPos().z), q3Vec3(0.0f, 0.0f, 0.0f), 0.0f);
    }
  }
  bool checkCollisionWithBox(glm::vec2 pos, glm::vec2 scale) {
    // for (o : objects) {
    //   glm::vec2 newPos = glm::vec2(pos.x-(scale.x/2.0f),pos.y-(scale.y/2.0f));
    //   if (newPos.x < o->getPos().x + 1 &&
    //       newPos.x + scale.x > o->getPos().x &&
    //       newPos.y < o->getPos().y + 1 &&
    //       newPos.y + scale.y > o->getPos().y) {
    //         o->destroy = true;
    //         return true;
    //       }
    // }
    return false;
  }
  bool Fire(glm::vec2 startPos,glm::vec3 forward) {
    delayCount++;
    if (delayCount>delay) {
      delayCount = 0;
      color.a = 1.0;
      
      Projectile * o;
      o = new Projectile();
      glm::vec3 start = glm::vec3(startPos.x + (forward.x * 7.0f), height + (forward.y * 2.0f), startPos.y + (forward.z * 7.0f));
      o->transform.setPos(start);
      //std::cout << "Firing at X: " << start.x << " Y: " << start.y << " Z: " << start.z << "\n";
      o->forward = forward;
      glm::vec3 sc = glm::vec3(2.3f, 2.3f, 2.3f);
      o->transform.setScale(sc);
      o->projB.init(glm::vec3(2.3f, 2.3f, 2.3f), start, scenePointer, eDynamicBody);
      // o->projB.setPos(glm::vec3(o->projB.getPos().x-o->forward.x*5.0f,o->projB.getPos().y,o->projB.getPos().z-o->forward.z*5.0f));
      o->projB.body->SetLinearVelocity(q3Vec3(o->forward.x * speed * SpeedMultiplier * 100.0f, o->forward.y * speed * SpeedMultiplier * 100.0f, o->forward.z * speed * SpeedMultiplier * 100.0f));
      objects.push_back(o);
      //o->startPos = glm::vec3(startPos.x,height,startPos.y);
      return true;
    } 
    return false;
  }
  bool FireMultiple(glm::vec2 startPos, glm::vec3 forward, int num) {
      delayCount++;
      if (delayCount > delay) {
          delayCount = 0;

          for (int i = 0; i <= num; i++) {
              
              color.a = 1.0;
              Projectile o;
              glm::vec3 start = glm::vec3(startPos.x + (forward.x * 7.0f), height + (forward.y * 2.0f), startPos.y + (forward.z * 7.0f));
              o.transform.setPos(start);
              o.forward = forward;
              glm::vec3 sc = glm::vec3(2.0f, 2.0f, 2.0f);
              o.transform.setScale(sc);
              objects.push_back(&o);
          }
          return true;
      }
      return false;
  }
  bool FireInstant(glm::vec2 startPos, glm::vec3 forward) {
        color.a = 1.0;
        Projectile o;
        glm::vec3 start = glm::vec3(startPos.x + (forward.x * 7.0f), height + (forward.y * 2.0f), startPos.y + (forward.z * 7.0f));
        o.transform.setPos(start);
        o.forward = forward;
        glm::vec3 sc = glm::vec3(2.0f, 2.0f, 2.0f);
        o.transform.setScale(sc);

        objects.push_back(&o);
        return true;
  }
  void setSpeedMultiplier(double SpeedMultiplier) {
    this->SpeedMultiplier = SpeedMultiplier;
  }
  void setRange(float range) {
    this->range = range;
  }
  void setColor(glm::vec4 color) {
    this->color = color;
  }
  glm::vec4 getColor() {
    return color;
  }
  void setScale(float scale) {
    //object.setScale(glm::vec3(scale,scale,scale));
  }
  void setSpeed(float speed) {
    this->speed = speed;
  }
  void setSpread(float spread) {
      this->spread = spread;
  }

std::vector<Projectile*> objects;
double SpeedMultiplier;
float speed;
float height;
int delay;
int delayCount;
bool emitDelay = false;

protected:
private:
  float spread = 0.000;
  int data = 0;
  std::string owner;
  glm::vec4 color = glm::vec4(0.0f,0.7f,1.0f,1.0f);
  std::string folder;
  float range;
  std::string model;
  bool hasInit = false;
  Mesh projMesh;
  Shader projShader;
  Shader trailShader;

  Object* ObjectFile;
};
#endif
