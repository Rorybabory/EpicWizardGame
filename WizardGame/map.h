#ifndef MAP_H
#define MAP_H

#include "luaLibrary.h"

#include <LuaBridge/LuaBridge.h>
#include <vector>
#include "entitySystem.h"
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
extern std::vector<std::string> mods;
extern bool restartFile;
class Map {
public:
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
            bool exist = std::experimental::filesystem::exists("./mods/" + folder + "/" + removeWord(input, "./res"));
            if (exist) {
                temp = "./mods/" + folder + "/" + removeWord(input, "./res");
            }

        }
        return temp;
    }
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
    void checkLoad(lua_State* L) {
        for (Entity* e : eSystem.entities) {
            if (e->map != "") {
                //std::cout << "LOADING MAP: " << e->map << "\n\n\n\n\n\n\n\n";
                restartMap(e->map, L);

            }
        }
        if (eSystem.load == true) {
            //eSystem.restartText();
            restartMap(eSystem.mapPath, L);
            eSystem.load = false;
        }
        
    }
    void Update(lua_State * L) {
        auto update = std::async(&EntitySystem::Update, &eSystem, L);

        //eSystem.Update(L);
        if (eSystem.save == true) {
            writeToFile();
            eSystem.save = false;
        }
        
        
    }
    void restartMap(std::string file, lua_State * L) {
        std::string path = "./res/maps/";
        path += file;
        path += ".lua";

        luah::loadScript(L, path);
        luabridge::LuaRef mapRef = luabridge::getGlobal(L, file.c_str());
        eSystem.clearScene();
        eSystem.entities.clear();
        eSystem.props.clear();
        eSystem.resetFloor();
        for (int i = 0; i < mapRef.length(); ++i) {
            luabridge::LuaRef item = mapRef[i + 1];
            if (item["type"] == "prop") {
                std::string file = item["file"];
                float scale = item["scale"];
                float x = item["x"];
                float y = item["y"];
                bool collide = item["collides"];
                float rot = item["rot"];

                eSystem.addProp(convertPath(file), glm::vec3(scale, scale, scale), glm::vec3(x, 0.0f, y), rot, collide);
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
        float rot = item["rot"];
        eSystem.addProp(convertPath(file),glm::vec3(scale,scale,scale),glm::vec3(x,0.0f,y), rot, collide);
      }else if (item["type"] == "entity") {
        std::string file = item["file"];
        float x = item["x"];
        float y = item["y"];
        eSystem.addEntityAtPos("res/scripts/entities.lua", file, glm::vec2(x,y),L);
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
