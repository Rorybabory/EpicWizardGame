#ifndef OBJCOMPONENT_H
#define OBJCOMPONENT_H
#include "Component.h"
#include <glm/glm.hpp>
#include <string>
#include "object.h"
#include <LuaBridge/LuaBridge.h>

class ObjectComponent : public Component {
public:
    ObjectComponent(luabridge::LuaRef& componentTable) {
        auto folderRef = componentTable["folder"];
        auto colorRef = componentTable["color"];
        float r = colorRef["r"];
        float g = colorRef["g"];
        float b = colorRef["b"];
        float a = colorRef["a"];
        object.initObject(folderRef.cast<std::string>(), glm::vec4(r, g, b, a), "./res/basicShader", false);
        color = glm::vec4(r, g, b, a);
    }
    
    
    void Draw(Camera* c) {
        object.Update();
        object.Draw(*c);
    }
    void setScale(glm::vec3 sc) { object.setScale(sc); }
    void setRot(glm::vec3 rot) { object.setRot(rot); }
    void setPos(glm::vec3 pos) { object.setPos(pos); }
    void setColor(glm::vec4 c) { object.setColor(c); }
    glm::vec3 getPos() { return object.getPos(); }
    Object object;
    glm::vec4 color;
private:
    
};

#endif