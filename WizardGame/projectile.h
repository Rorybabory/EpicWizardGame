#pragma once
#include "transform.h"
#include <glm/glm.hpp>
#include "CollisionSystem.h"

struct Projectile {
	Transform transform;
	bool destroy;
	Box projB;
	glm::vec3 forward;
	glm::vec3 lastDir;
	int counter;
	int counter2;
	bool boxDestroy;
	bool hasInit;
	int delay;
	int timesBounced = 0;
	std::vector<glm::vec3> lastPos;
	int recordPosCount = 0;
};