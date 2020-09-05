#include <iostream>
#include <fstream>

#include <map>
#include <vector>
#include "animatedMesh.h"
#include "camera.h"
#include "text.h"
#include <glm/glm.hpp>
#include "GUIComponent.h"
#include "Ability.h"
#include "display.h"
#include "FrameBuffer.h"
#include "imgui_sdl.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include "Globals.h"
float Width = 800;
float Height = 600;

bool resetFramebuffer = false;
bool resetWindow = false;
bool resetCamera = false;
bool resetText = false;

std::vector<std::string> loadedScripts;
lua_State* LPointer;
float red = 0.0;
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
std::vector<std::string> playerAbilitiesDesc;
bool drawScene = true;
glm::vec2 enemyTarget;
bool drawUI = false;
bool close = false;

q3Scene* scenePointer;

bool resetScripts = false;


bool shakeScreen = true;
void closeProgram() { close = true; }

int getHighScore() {
	std::fstream myfile("./res/save.dat", std::ios_base::in);
	int a;
	while (myfile >> a)
	{
	}
	a = (a >> 1);
	myfile.close();
	return a;
}
void setHighScore(int val) {
	ofstream myfile;
	myfile.open("./res/save.dat");
	val = (val << 1);
	myfile << val;
	myfile.close();

	std::cout << "SET THE HIGH SCORE TO " << val << "\n\n\n\n\n\n\n";
	myfile.close();
}

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
