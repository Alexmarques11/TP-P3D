#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    std::string diffuseTexturePath;
    GLuint diffuseTexture;

    // Construtor padrão
    Material() : name(""), ambient(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)), shininess(0.0f), diffuseTexturePath(""), diffuseTexture(0) {}
};

#endif // MATERIAL_H