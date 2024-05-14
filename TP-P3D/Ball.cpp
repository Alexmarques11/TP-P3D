#include "Ball.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Ball::Ball(const std::string& path) {
    loadOBJ(path);
    setupBuffers();
}

void Ball::loadOBJ(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << path << std::endl;
        return;
    }

    std::string dir = path.substr(0, path.find_last_of('/'));
    std::string line;

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
    currentMaterial = new Material();

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "newmtl") {
            ss >> currentMaterial->name;
        }
        else if (prefix == "Ka") {
            ss >> currentMaterial->ambient.r >> currentMaterial->ambient.g >> currentMaterial->ambient.b;
        }
        else if (prefix == "Kd") {
            ss >> currentMaterial->diffuse.r >> currentMaterial->diffuse.g >> currentMaterial->diffuse.b;
        }
        else if (prefix == "Ks") {
            ss >> currentMaterial->specular.r >> currentMaterial->specular.g >> currentMaterial->specular.b; 
        }
        else if (prefix == "Ns") {
            ss >> currentMaterial->shininess;
        }
        else if (prefix == "map_Kd") {
            ss >> currentMaterial->diffuseTexturePath;

            std::string texturePath = path.substr(0, path.find_last_of('/')) + "/" + currentMaterial->diffuseTexturePath;
            int width, height, nrChannels;

            unsigned char* imageData = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

            if (!imageData) {
                std::cerr << "Failed to load texture: " << texturePath << " - " << stbi_failure_reason() << std::endl;
                continue;
            }
            if (imageData) {
                glGenTextures(1, &currentMaterial->diffuseTexture);
                glBindTexture(GL_TEXTURE_2D, currentMaterial->diffuseTexture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, imageData);

                // Set texture parameters here
                glBindTexture(GL_TEXTURE_2D, currentMaterial->diffuseTexture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                std::cout << "Loaded texture: " << texturePath << " with ID: " << currentMaterial->diffuseTexture << std::endl;
                stbi_image_free(imageData);
            }
            else {
                std::cerr << "Failed to load texture: " << texturePath << std::endl;
            }
        }
    }

    file.close();
}


void Ball::setupBuffers() {
    // Gera um Vertex Array Object (VAO) e vincula-o
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Gera um Vertex Buffer Object (VBO) para os vértices e o vincula
    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    // Copia os dados dos vértices para o buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    // Define o layout do atributo de vértice (posição) no índice 0 do VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);  // Ativa o atributo de vértice no índice 0

    // Gera um VBO para as normais e o vincula
    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    // Copia os dados das normais para o buffer
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    // Define o layout do atributo de vértice (normal) no índice 1 do VAO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);  // Ativa o atributo de vértice no índice 1

    // Gera um VBO para as coordenadas de textura (UVs) e o vincula
    glGenBuffers(1, &vboUVs);
    glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
    // Copia os dados das coordenadas de textura para o buffer
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
    // Define o layout do atributo de vértice (coordenadas de textura) no índice 2 do VAO
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);  // Ativa o atributo de vértice no índice 2

    // Gera um Element Buffer Object (EBO) para os índices e o vincula
    glGenBuffers(1, &eboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndices);
    // Copia os dados dos índices para o buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int), vertexIndices.data(), GL_STATIC_DRAW);

    // Desvincula o VAO para evitar modificações acidentais
    glBindVertexArray(0);
}

Ball::~Ball() {
    // Limpar os buffers OpenGL
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboNormals);
    glDeleteBuffers(1, &vboUVs);
    glDeleteBuffers(1, &eboIndices);
    glDeleteVertexArrays(1, &vao);
}
