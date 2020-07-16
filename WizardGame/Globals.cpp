#include <iostream>
#include <map>
#include <vector>
#include "animatedMesh.h"
#include "camera.h"
#include "text.h"
#include <glm/glm.hpp>
#include "GUIComponent.h"
float red = 0.5;
float cullDistance = 70;
glm::vec2 levelSize = glm::vec2(290, 290);
int globalVariable = 69;
std::map<std::string, std::vector<animatedMesh*>> storedObjectData;
double FPS = 60.0;
int screenInverted = 0;
float brightness = 0;
std::vector<Text*> texts;
std::vector<glm::vec2> textPositions;
std::vector<std::string> textStrings;
std::vector<glm::vec3> colors;
std::string playerTag;
std::vector<glm::vec2> entityPos;
glm::vec3 hsv = glm::vec3(0.0);
std::vector<std::string> mods;
std::map<std::string, float> GlobalFloatsVars;
std::map<std::string, bool> GlobalBoolsVars;
bool restartFile = false;
int entityCount = 0;
std::map<std::string, TextData> textMap;
char mapPath[255] = "newMap";
glm::vec4 screenColor;
std::vector<std::string> playerAbilities;
bool drawScene = true;

void addTextBox(std::string text, glm::vec2 pos, glm::vec3 color, int scale) {
	Text * object = new Text(scale, "./res/Ubuntu-B.ttf");
	textPositions.push_back(pos);
	textStrings.push_back(text);
	texts.push_back(object);
	colors.push_back(color);
}
void clearText() {
	texts.clear();
	textPositions.clear();
	textStrings.clear();
	colors.clear();
}
void drawText() {
	for (int i = 0; i < texts.size(); i++) {
		texts[i]->Draw(textStrings[i],textPositions[i],glm::vec4(colors[i].r,colors[i].g,colors[i].b,1.0f),800,600);
	}
}
