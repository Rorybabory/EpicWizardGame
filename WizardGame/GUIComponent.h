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
#include "TextRenderer.h"
#include "texture.h"
extern float Width;
extern float Height;
extern bool resetText;

struct TextData {
    glm::vec2 pos;
    std::string text;
    glm::vec4 color;
};
struct ImageData {
    Transform* transform;
    Texture* tex;
    bool draw = true;
};
struct StoredImageData {
    glm::vec2 pos;
    glm::vec2 scale;
};

class GUIComponent : public Component {
public:
    GUIComponent(luabridge::LuaRef& componentTable) : shader("./res/guiShader"), imageMesh("./res/postRender.obj", true) {
        if (componentTable.isNil() == false) {
            std::string fontRef = componentTable["font"];
            auto colorRef = componentTable["color"];
            auto sizeRef = componentTable["size"];
            glm::vec3 color = glm::vec3(colorRef["r"], colorRef["g"], colorRef["b"]);
            this->textSize = sizeRef;
            this->font = fontRef;
            resetSize();

        }
        else {
            std::string fontRef = "./res/fonts/PolygonParty.ttf";
            auto sizeRef = 42;
            glm::vec3 color = glm::vec3(0, 0, 1);
            this->textSize = sizeRef;
            this->font = fontRef;
            resetSize();
        }
        
    }
    int CalculateLocX(float val, float size) {
        float newVal = val * Width;
        newVal = (Width / 2.0) + (newVal / 2.0);
        return (int)newVal;
    }
    int CalculateLocY(float val, float size) {
        float size2 = size - (size / 10);
        return (int)((((val / 1.0f) * size) / 2.0f) + (size / 2.0f)) - (val * (textSize * (Width / 800.0f)));
    }
    void setText(std::string tag, std::string text, glm::vec2 pos, glm::vec4 color) {
        TextData data;
        data.text = text;
        data.pos = pos;
        data.color = color;
        //data.font.init(font.c_str(), textSize);
        textMap[tag] = data;
    }
    void setImage(std::string tag, std::string src) {
        if (imageMap.count(tag)) {
            if (imageMap[tag].tex->fileName == src) {
                imageMap[tag].transform->m_pos = glm::vec3(stData.pos.x, stData.pos.y, 0.0);
                imageMap[tag].transform->m_scale = glm::vec3(stData.scale.x, stData.scale.y, 1.0);
            }
            else {
                ImageData data;
                data.tex = new Texture(src);
                data.transform = new Transform();
                data.transform->m_pos = glm::vec3(stData.pos.x, stData.pos.y, 0.0);
                data.transform->m_scale = glm::vec3(stData.scale.x, stData.scale.y, 1.0);
                imageMap[tag] = data;
            }
        }
        else {
            ImageData data;
            data.tex = new Texture(src);
            data.transform = new Transform();
            data.transform->m_pos = glm::vec3(stData.pos.x, stData.pos.y, 0.0);
            data.transform->m_scale = glm::vec3(stData.scale.x, stData.scale.y, 1.0);
            imageMap[tag] = data;
        }
        
    }
    
    void setImageTransform(glm::vec2 pos, glm::vec2 scale) {
        stData.pos = pos;
        stData.scale = scale;
    }
    void resetSize() {
        float size = (Width / 800.0f);
        size *= textSize;
        std::cout << "SIZE: " << size << "\n\n\n\n";
        Font.init(font.c_str(), size);
    }
    void Draw() {
        if (resetText == true) {
            resetSize();
            resetText = false;
        }
        for (auto const& x : textMap) {
            glColor4f(x.second.color.r, x.second.color.g, x.second.color.b, x.second.color.a);
            glfreetype::print(Font, CalculateLocX(x.second.pos.x, (float)Width), CalculateLocY(x.second.pos.y, (float)Height), x.second.text.c_str());

            glColor4f(x.second.color.r, x.second.color.g, x.second.color.b, x.second.color.a*0.2);
            glfreetype::print(Font, CalculateLocX(x.second.pos.x, (float)Width), CalculateLocY(x.second.pos.y, (float)Height)-((Width / 800.0f)*6), x.second.text.c_str());
        }
        if (imageMap.empty() == false) {
            shader.Bind();
            for (auto const& x : imageMap) {
                if (x.second.draw == true) {
                    shader.Update(*x.second.transform);
                    x.second.tex->Bind(0);
                    imageMesh.Draw();
                    x.second.tex->UnBind();
                }
            }
            shader.UnBind();
        }
    }
    void setDraw(std::string tag, bool draw) {
        imageMap[tag].draw = draw;
    }
    void freeData() {
        //Font.clean();
        shader.~Shader();
        imageMesh.~Mesh();
    }
    void setTextSize(int tsize) {
        textSize = tsize;
    }
    bool resetFont = false;
protected:
private:
    //std::string texToSet;
    //bool setTex = false;
    std::map<std::string, TextData> textMap;
    std::map<std::string, ImageData> imageMap;
    glm::vec4 color;
    int textSize = 35;
    std::string font;
    glfreetype::font_data Font;
    Texture tex;
    Mesh imageMesh;
    Shader shader;
    Transform transform;
    StoredImageData stData;
};
#endif
