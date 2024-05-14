#include "Ball.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Ball::Ball(const std::string& path) {
    loadOBJ(path);
}

void Ball::loadOBJ(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << path << std::endl;
        return;
    }

    std::string dir = path.substr(0, path.find_last_of('/'));
    std::string line;
    std::string currentMaterial;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (prefix == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            uvs.push_back(uv);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "f") {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            char slash;
            for (int i = 0; i < 3; ++i) {
                ss >> vertexIndex[i] >> slash >> uvIndex[i] >> slash >> normalIndex[i];
                vertexIndices.push_back(vertexIndex[i] - 1);
                uvIndices.push_back(uvIndex[i] - 1);
                normalIndices.push_back(normalIndex[i] - 1);
            }
        }
        else if (prefix == "usemtl") {
            ss >> currentMaterial;
        }
        else if (prefix == "mtllib") {
            std::string mtlFile;
            ss >> mtlFile;
            loadMTL(dir + "/" + mtlFile);
        }
    }

    file.close();
}

void Ball::loadMTL(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the MTL file: " << path << std::endl;
        return;
    }

    std::string line;
    Material currentMaterial;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "newmtl") {
            if (!currentMaterial.name.empty()) {
                materials[currentMaterial.name] = currentMaterial;
            }
            ss >> currentMaterial.name;
            currentMaterial = Material();
        }
        else if (prefix == "Ka") {
            ss >> currentMaterial.ambient.r >> currentMaterial.ambient.g >> currentMaterial.ambient.b;
        }
        else if (prefix == "Kd") {
            ss >> currentMaterial.diffuse.r >> currentMaterial.diffuse.g >> currentMaterial.diffuse.b;
        }
        else if (prefix == "Ks") {
            ss >> currentMaterial.specular.r >> currentMaterial.specular.g >> currentMaterial.specular.b;
        }
        else if (prefix == "Ns") {
            ss >> currentMaterial.shininess;
        }
        else if (prefix == "map_Kd") {
            ss >> currentMaterial.diffuseTexturePath;

            std::string texturePath = path.substr(0, path.find_last_of('/')) + "/" + currentMaterial.diffuseTexturePath;
            int width, height, nrChannels;

            unsigned char* imageData = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

            if (!imageData) {
                std::cerr << "Failed to load texture: " << texturePath << " - " << stbi_failure_reason() << std::endl;
                continue;
            }
            if (imageData) {
                glGenTextures(1, &currentMaterial.diffuseTexture);
                glBindTexture(GL_TEXTURE_2D, currentMaterial.diffuseTexture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, imageData);
                // glGenerateMipmap(GL_TEXTURE_2D);
                stbi_image_free(imageData);
            }
            else {
                std::cerr << "Failed to load texture: " << texturePath << std::endl;
            }
        }
    }

    if (!currentMaterial.name.empty()) {
        materials[currentMaterial.name] = currentMaterial;
    }

    file.close();
}
