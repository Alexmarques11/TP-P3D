#ifndef LIGHT_H
#define LIGHT_H

#include "Utils.h"
#include "Material.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct AmbientLight {
    glm::vec3 color;    // Cor da luz ambiente
    float intensity;    // Intensidade da luz ambiente
};

class Light {
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
        : position(pos), direction(dir), ambient(ambient), diffuse(diffuse), specular(specular) {}
};

void HandleKeyboardInput(int key, int action);
void UpdateLightSources();

#endif // LIGHT_H
