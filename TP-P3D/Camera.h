#ifndef CAMERA_H
#define CAMERA_H

#include "Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Define os valores padr�o para a c�mera
const float ZOOM = 12.0f;
const float ANGLE = 0.0f;

class Camera {
public:
    // Construtor com vetores de posi��o e orienta��o customizados
    Camera(float zoom = ZOOM, float angle = ANGLE);

    glm::mat4 getProjection() const;

    // Retorna a matriz de visualiza��o da c�mera
    glm::mat4 getViewMatrix() const;

    glm::mat4 getMvp() const;

    // Processa o movimento do mouse para orientar a c�mera
    void processMouseMovement(float xoffset, bool isLeftMouseButtonPressed);

    // Processa o scroll do mouse para alterar o zoom da c�mera
    void processMouseScroll(float yoffset);

private:
    float zoom;
    float angle;
};

#endif