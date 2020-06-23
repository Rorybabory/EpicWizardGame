#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "mesh.h"
#include "shader.h"
#include "transform.h"
#include "camera.h"
#include "texture.h"
struct Particle {
    glm::vec3 Position, Velocity, Rotation;
    
    glm::vec4 Color;
    float     Life;
    bool    enabled = false;
    Particle(glm::vec3 Position, glm::vec3 Velocity, glm::vec4 Color, float Life)
        : Position(0.0f), Rotation(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {
        this->Position = Position;
        this->Velocity = Velocity;
        this->Color = Color;
        this->Life = Life;
    }
    Particle() {
        this->Position = glm::vec3(0.0f);
        this->Velocity = glm::vec3(0.0f);
        this->Color = glm::vec4(0.0f);
        this->Life = 0.0;
    }
};

class Emitter
{
public:
    Emitter(glm::vec3 Velocity, float Life, int max_particles, float Randomness, bool gravity);
    ~Emitter() {
        std::cout << "deleted emitter\n";
        particles.clear();
    }
    void addParticles(int num, glm::vec3 pos, glm::vec4 Color);
    int getFirstInactiveParticle();
    void clearParticles();
    void drawParticles(Camera camera);
    void updateParticles();
    void updateVelocity(int i);
    void setModel(std::string filename);
    std::vector<Particle> particles;
    glm::vec3 Velocity;
    float Life;
    float Randomness;
    float gravitySpeed = 0.1;
    bool gravity = true;
    Mesh mesh;
    Transform transform;
    Shader shader;
    Texture texture;
    bool hasInit = false;
private:
    int max_particles;
    int num_particles;
};
