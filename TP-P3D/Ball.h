#ifndef BALL_H
#define BALL_H

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
    Ball(const std::string& path);
    const std::vector<glm::vec3>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getVertexIndices() const { return vertexIndices; }
    const std::unordered_map<std::string, Material>& getMaterials() const { return materials; }
    const std::string& getCurrentMaterial() const { return currentMaterial; }

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::unordered_map<std::string, Material> materials;
    std::string currentMaterial;

private:
    void loadOBJ(const std::string& path);
    void loadMTL(const std::string& path);

};

#endif // BALL_H
