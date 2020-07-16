#pragma once
#include "mesh.h"
#include "shader.h"
#include "transform.h"
#include "camera.h"
extern std::vector<glm::vec2> entityPos;

class Minimap {
public:
	Minimap() : mesh("./res/postRender.obj", false), shader("./res/basicShader") {
		camera.InitCam(glm::vec3(0, 0, -5), 70.0, 800.0f / 600.0f, 0.01f, 2000.0f);
		glm::vec3 rot = glm::vec3(0.0, 3.14, 0.0);
		transform.setRot(rot);
	}
	void DrawSquare(glm::vec3 color) {
		shader.Bind(glm::vec4(color,1.0), glm::vec4(0.0));
		shader.Update(transform, camera);
		mesh.Draw();
	}
	void Draw() {
		
		transform.m_pos = glm::vec3(-mPos.x, mPos.y, 0.0);
		transform.m_scale = glm::vec3(0.55, 0.55, 0.0);
		DrawSquare(glm::vec3(0.0, 0.0, 0.0));
		transform.m_pos.z -= 0.02;
		transform.m_scale = glm::vec3(0.50, 0.50, 0.0);
		DrawSquare(glm::vec3(0.3, 0.3, 0.3));
		transform.m_scale = glm::vec3(0.05, 0.05, 0.0);

		float order = -0.03f;
		for (glm::vec2 pos : entityPos) {
			transform.m_pos = glm::vec3(-mPos.x+(pos.x/600), mPos.y + (pos.y/600), order);

			DrawSquare(glm::vec3(1.0, 0.0, 0.0));
		}
	}
private:
	glm::vec2 mPos = glm::vec2(-1.75,-1.5);
	Mesh mesh;
	Shader shader;
	Transform transform;
	Camera camera;
};