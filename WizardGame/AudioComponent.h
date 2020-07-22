#pragma once
#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H
#include <map>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include "Audio.h"
#include <LuaBridge/LuaBridge.h>
#include "Component.h"
#include "camera.h"
class AudioComponent : public Component {
public:
    AudioComponent(luabridge::LuaRef& componentTable) {

    }
    void setFile(std::string file) {
        sound.setFile(file);
    }
    void play() {
        sound.play();
    }
protected:
private:
    Sound sound;
};
#endif
