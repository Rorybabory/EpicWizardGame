#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H
#include <map>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include "animatedObject.h"
#include "luaLibrary.h"
#include <LuaBridge/LuaBridge.h>
#include "Component.h"
#include <glfreetype/TextRenderer.hpp>
struct TextData {
    glm::vec2 pos;
    std::string text;
    glm::vec4 color;
    glfreetype::font_data font;
};
class GUIComponent : public Component {
public:
    GUIComponent(luabridge::LuaRef& componentTable) {
        std::string fontRef = componentTable["font"];
        auto colorRef = componentTable["color"];
        auto sizeRef = componentTable["size"];
        glm::vec3 color = glm::vec3(colorRef["r"], colorRef["g"], colorRef["b"]);
        this->textSize = sizeRef;
        Font.init((fontRef).c_str(), sizeRef);
    }
    int CalculateLocX(float val, float size) {
        float size2 = size - (size / 10);
        return (int)((((val / 1.0f) * size) / 2) + (size / 2));
    }
    int CalculateLocY(float val, float size) {
        float size2 = size - (size / 10);
        return (int)((((val / 1.0f) * size) / 2) + (size / 2)) - (val * textSize);
    }
    void setText(std::string tag, std::string text, glm::vec2 pos, glm::vec4 color) {
        TextData data;
        data.text = text;
        data.pos = pos;
        data.color = color;
        //data.font.init("./res/Avara.ttf", 35);
        textMap[tag] = data;
    }
    void Draw() {
        for (auto const& x : textMap) {
            glColor4f(x.second.color.r, x.second.color.g, x.second.color.b, x.second.color.a);
            glfreetype::print(Font, CalculateLocX(x.second.pos.x, (float)800), CalculateLocY(x.second.pos.y, (float)600), x.second.text.c_str());
            glColor4f(x.second.color.r, x.second.color.g, x.second.color.b, x.second.color.a-(x.second.color.a/1.35));
            glfreetype::print(Font, CalculateLocX(x.second.pos.x, (float)800), CalculateLocY(x.second.pos.y, (float)600)-4, x.second.text.c_str());
        }
    }
    void freeData() {
        Font.clean();
    }
protected: 
private:
    std::map<std::string, TextData> textMap;

    glfreetype::font_data Font;
    int textSize = 0;

};
#endif
