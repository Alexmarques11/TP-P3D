#ifndef BALL_H
#define BALL_H

#include <GL/glew.h> 
#include <string>
#include <vector>    
#include <glm/glm.hpp>
#include "Camera.h"
#include "Lights.h"

class Ball {

private:

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;

    static const float BALL_RADIUS;
    const float SPEED = 0.1f;

    Camera* cameraPtr;
    Lights* lightsPtr;

    GLuint VAO;
    GLuint VBO;
    GLuint ShaderProgram;
    GLuint textureIndex;

    void LoadMTL(char* mtl_model_filepath);
    void LoadTexture(const char* textureFileName);

    // Função para verificar colisão
    bool IsColliding(const std::vector<Ball>& balls);

public:

    //Variaveis para os dados do ficheiro obj
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    glm::vec3 position;
    glm::vec3 orientation;
    bool isMoving;

    //Construtor
    Ball(const glm::vec3& initialPosition, GLuint textureIndex, GLuint shaderProgram, Camera* camera, Lights* lights, bool isMoving = false, glm::vec3 orientation = glm::vec3(0, 0, 0));

    //Declarar as funções
    void Load(const std::string obj_model_filepath);
    void Install();
    void Render(glm::vec3 position, glm::vec3 orientation);
    void Update(float deltaTime, const std::vector<Ball>& balls);

    static std::vector<glm::vec3> GetBallInitialPositions();
};


#endif 
