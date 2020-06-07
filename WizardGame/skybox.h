#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include "stb_image.h"
#include "shader.h"
#include <iostream>
// thanks learnOpenGL.com!
class SkyboxMesh {
public:
	SkyboxMesh();
	void Render();
	void BindVAO();
private:
	unsigned int skyboxVAO, skyboxVBO;

};
class Skybox
{
public:
	unsigned int loadSkybox(std::vector<std::string> faces);
	void Draw(Camera camera);
	Skybox() : shader("./res/shaders/skybox") {
		std::vector<std::string> fileNames;
		fileNames.emplace_back("./res/skybox/right.png");
		fileNames.emplace_back("./res/skybox/left.png");
		fileNames.emplace_back("./res/skybox/up.png");
		fileNames.emplace_back("./res/skybox/down.png");
		fileNames.emplace_back("./res/skybox/back.png");
		fileNames.emplace_back("./res/skybox/front.png");
		textureID = loadSkybox(fileNames);
		shader.Bind();

		glUniform1i(glGetUniformLocation(shader.m_program, "skybox"), 0);

	}
private:
	SkyboxMesh mesh;
	Shader shader;
	unsigned int textureID;

};

