#pragma once
#include <glm/glm.hpp>
#include "object.h"
#include "camera.h"

class HealthBar {
public:
	float RADS_TO_DEGS(float radians) {
		return radians * 180 / 3.1415926535;
	}
	float DEGS_TO_RADS(float degrees) {
		return degrees * 3.1415926535 / 180;
	}
	HealthBar() {
		object = Object("./res/cube.obj", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), "./res/basicShader", false);
	}
	void setValue(float value) { this->targetValue = value; }
	float getValue() { return value; }
	void setColor(glm::vec4 color) { object.setColor(color); }
	void Draw(Camera mainCam, glm::vec3 pos) {
		if (hasInit == false) {
			hasInit = true;
			
		}
		if (targetValue != value) {
			if (targetValue > value) {
				value += 0.02;
			} else {
				value -= 0.02;
			}
		}
		//value += ((float)(targetValue != value)) *((float)(targetValue > value)) * 0.02f;
		float tempRot = 0.0;
		glm::vec2 targetPos = glm::vec2(mainCam.m_position.x, mainCam.m_position.z);
		targetPos.x = targetPos.x - pos.x;
		targetPos.y = targetPos.y - pos.z;
		float targetAngle = RADS_TO_DEGS(-(atan2(targetPos.y, targetPos.x)) + 1.57f);
		float rotDegs = RADS_TO_DEGS(tempRot);
		rotDegs = targetAngle;
		object.setPos(pos);
 		object.setRot(glm::vec3(0.0, DEGS_TO_RADS(rotDegs), 0.0));
		object.setScale(glm::vec3(value*2.5, 0.2f, 0.2f));
		object.Draw(mainCam);
	}
private:
	bool hasInit = false;
	Object object;
	float value = 1.0f;
	float targetValue = 1.0f;
};