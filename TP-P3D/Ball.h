#ifndef BALL_H
#define BALL_H

#include "Utils.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include <GL/glew.h>

struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    std::string diffuseTexturePath;
    GLuint diffuseTexture;
};

class Ball {
public:
    Ball(const std::string& path, const glm::vec3& initialPosition);
    ~Ball();

    glm::vec3 position;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    Material* currentMaterial;

    GLuint vao;             // Vertex Array Object
    GLuint vboVertices;     // Vertex Buffer Object para vértices
    GLuint vboNormals;      // Vertex Buffer Object para normais
    GLuint vboUVs;          // Vertex Buffer Object para UVs
    GLuint eboIndices;      // Element Buffer Object para índices

    float getHeight() const;

private:
    void loadOBJ(const std::string& path);
    void loadMTL(const std::string& path);

    void setupBuffers();
};

#endif // BALL_H
