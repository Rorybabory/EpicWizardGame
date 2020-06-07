#ifndef MAP_H
#define MAP_H

#include "luaLibrary.h"

#include <LuaBridge/LuaBridge.h>
#include <vector>
#include "entitySystem.h"
#include <iostream>
#include <fstream>

class Map {
public:
    void writeToFile() {
        std::string path = "./res/maps/";
        path += eSystem.mapPath;
        path += ".lua";
        ofstream mapFile;
        mapFile.open(path);
        mapFile << eSystem.mapPath << " = {\n";
        for (Prop* prop : eSystem.props) {
            mapFile << "    {\n";
            mapFile << "        type = \"prop\",\n";
            mapFile << "        file = \"" << prop->name << "\",\n";
            mapFile << "        scale = " << prop->meshScale << ",\n";
            mapFile << "        x = " << prop->objects[0]->getPos().x << ",\n";
            mapFile << "        y = " << prop->objects[0]->getPos().z << ",\n";
            if (prop->collision == true) {
                mapFile << "        collides = true\n";
            }else {
                mapFile << "        collides = false\n";
            }
            mapFile << "    },\n";
        }
        for (Entity* entity : eSystem.entities) {
            mapFile << "    {\n";
            mapFile << "        type = \"entity\",\n";
            mapFile << "        file = \"" << entity->type << "\",\n";
            mapFile << "        x = " << entity->pos.x << ",\n";
            mapFile << "        y = " << entity->pos.z << ",\n";
            mapFile << "    },\n";
        }
        mapFile << "}\n";
        mapFile.close();
    }
    void Update(lua_State * L) {
        eSystem.Update(L);
        if (eSystem.save == true) {
            writeToFile();
            eSystem.save = false;
        }
        if (eSystem.load == true) {
            restartMap(eSystem.mapPath,L);
            eSystem.load = false;
        }
    }
    void restartMap(std::string file, lua_State * L) {
        std::string path = "./res/maps/";
        path += eSystem.mapPath;
        path += ".lua";

        luah::loadScript(L, path);
        luabridge::LuaRef mapRef = luabridge::getGlobal(L, eSystem.mapPath);
        eSystem.clearScene();
        eSystem.entities.clear();
        eSystem.props.clear();
        for (int i = 0; i < mapRef.length(); ++i) {
            luabridge::LuaRef item = mapRef[i + 1];
            if (item["type"] == "prop") {
                std::string file = item["file"];
                float scale = item["scale"];
                float x = item["x"];
                float y = item["y"];
                bool collide = item["collides"];
                eSystem.addProp(file, glm::vec3(scale, scale, scale), glm::vec3(x, 0.0f, y), collide);
            }
            else if (item["type"] == "entity") {
                std::string file = item["file"];
                float x = item["x"];
                float y = item["y"];
                eSystem.addEntityAtPos("res/scripts/entities.lua", file, glm::vec2(x, y), L);
            }
            else {
                std::cout << "ERROR: MAP: " << fileName << " HAS INVALID TYPED ITEM..." << "\n";
            }
        }

    }
  void init(std::string fileName, lua_State* L) {
      this->fileName = fileName;
    luah::loadScript(L,"./res/maps/" + fileName + ".lua");
    luabridge::LuaRef mapRef = luabridge::getGlobal(L,fileName.c_str());

    for (int i = 0; i < mapRef.length(); ++i) {
      luabridge::LuaRef item = mapRef[i+1];
      if (item["type"] == "prop") {
        std::string file = item["file"];
        float scale = item["scale"];
        float x = item["x"];
        float y = item["y"];
        bool collide = item["collides"];
        eSystem.addProp(file,glm::vec3(scale,scale,scale),glm::vec3(x,0.0f,y),collide);
      }else if (item["type"] == "entity") {
        std::string file = item["file"];
        float x = item["x"];
        float y = item["y"];
        eSystem.addEntityAtPos("res/scripts/entities.lua",file, glm::vec2(x,y),L);
      }else {
        std::cout << "ERROR: MAP: " << fileName << " HAS INVALID TYPED ITEM..." << "\n";
      }
    }
  }
  EntitySystem eSystem;
  std::string fileName;
private:
};

#endif
