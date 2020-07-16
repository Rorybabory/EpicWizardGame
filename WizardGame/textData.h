#pragma once
#include "luaLibrary.h"

#include <glfreetype/TextRenderer.hpp>
#include <string>
#include <glm/glm.hpp>

struct TextData {
    glm::vec2 pos;
    std::string text;
    glm::vec4 color;
    glfreetype::font_data Font;
    int textSize = 0;

};