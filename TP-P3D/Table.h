#ifndef TABLE_H
#define TABLE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Lights.h"

class Table {
public:
    Table(GLuint tableProgram, Camera* camera, Lights* lights);
    ~Table();

    void Render();

private:
    GLuint VAO, VBO, EBO;

    GLuint tableProgram;
    Camera* cameraPtr;
    Lights* lightsPtr;

    void Load();
};

#endif // TABLE_H
