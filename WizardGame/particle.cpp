#include "particle.h"
float random(float min, float max) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random * range) + min;
}
Emitter::Emitter(glm::vec3 Velocity, float Life, int max_particles, float Randomness, bool gravity) : mesh("./res/particle.obj", false),
																													   shader("./res/basicShader"){
	this->Velocity = Velocity;
	this->Life = Life;
	this->max_particles = max_particles;
	this->Randomness = Randomness;
	particles.resize(max_particles);
}
int Emitter::getFirstInactiveParticle() {
	for (int i = 0; i < max_particles; i++) {
		if (particles[i].enabled == false) {
			return i;
		}
	}
	return 0;
}
void Emitter::clearParticles() {
	for (int i = 0; i < max_particles; i++) {
		particles[i].enabled = false;
	}
}
void Emitter::addParticles(int num, glm::vec3 pos, glm::vec4 Color) {
	for (int i = 0; i <= num; i++) {
		int first = getFirstInactiveParticle();
		particles[first].enabled = true;
		particles[first].Position = pos;
		particles[first].Life = Life;
		particles[first].Color = Color;
		particles[first].Velocity = Velocity + glm::vec3(random(-Randomness, Randomness), random(-Randomness, Randomness), random(-Randomness, Randomness));
		particles[first].Rotation = glm::vec3(random(0.0,3.0), random(0.0, 3.0), random(0.0, 3.0));
	}
}
void Emitter::drawParticles(Camera camera) {
	shader.Bind();
	int ColorLocation = glGetUniformLocation(shader.m_program, "color");
	for (int i = 0; i < max_particles; i++) {
		if (particles[i].enabled == true) {
			transform.setPos(particles[i].Position);
			transform.setRot(particles[i].Rotation);
			glUniform4f(ColorLocation, particles[i].Color.r, particles[i].Color.g, particles[i].Color.b, particles[i].Color.a);
			shader.Update(transform, camera);
			mesh.Draw();
		}
	}
	shader.UnBind();
}
void Emitter::updateVelocity(int i) {
	if (particles[i].Velocity.x > 0.0f) {
		particles[i].Velocity.x -= gravitySpeed / 2.0;
	}
	else if (particles[i].Velocity.x < 0.0f) {
		particles[i].Velocity.x += gravitySpeed / 2.0;
	}
	if (particles[i].Velocity.z > 0.0f) {
		particles[i].Velocity.z -= gravitySpeed / 2.0;
	}
	else if (particles[i].Velocity.z < 0.0f) {
		particles[i].Velocity.z += gravitySpeed / 2.0;
	}
	if (gravity == true) {
		particles[i].Velocity.y -= gravitySpeed;
	}
}

void Emitter::updateParticles() {
	for (int i = 0; i < max_particles; i++) {
		if (particles[i].enabled == true) {
			updateVelocity(i);
			particles[i].Position += particles[i].Velocity;
			particles[i].Life -= 1.0;
			if (particles[i].Life <= 0.0) {
				particles[i].enabled = false;
			}
		}
	}
}