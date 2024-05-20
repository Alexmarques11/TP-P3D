#include "Camera.h"

Camera::Camera(float zoom, float angle) {
    this->zoom = zoom;
    this->angle = angle;
}

glm::mat4 Camera::getProjection() const {
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.f);
    return projection;
}

glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 5.0f, zoom),	// Posição da câmara no mundo
        glm::vec3(0.0f, 0.0f, -1.0f),	// Direção para a qual a câmara esta apontada
        glm::vec3(0.0f, 1.0f, 0.0f)		// Vector vertical
    );
    return view;
}

// get Mvp
glm::mat4 Camera::getTableMvp() const {
    glm::mat4 view = getViewMatrix();
    glm::mat4 projection = getProjection();

    glm::mat4 modelMatrix = glm::mat4(1.0f);	
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -5.0f, 0.0f));  
    modelMatrix = glm::rotate(modelMatrix, angle, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));

    glm::mat4 mvp = projection * view * modelMatrix;
    return mvp;
}


glm::mat4 Camera::getBallMvp(const Ball& ball, const Table& table) const {
 
    float ballYPosition = ball.getHeight() + table.getHeight() - 5.0f;

    glm::mat4 view = getViewMatrix();
    glm::mat4 projection = getProjection();

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::rotate(modelMatrix, angle, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(ball.position.x, ballYPosition, ball.position.z));

    glm::mat4 mvp = projection * view * modelMatrix;
    return mvp;
}



void Camera::processMouseMovement(float xoffset, bool isLeftMouseButtonPressed) {

    if (isLeftMouseButtonPressed) {
        angle = xoffset / 200.0f;
    }
}


/**
* \brief Função de callback para eventos de rolagem do mouse
*
* \param window
* \param xoffset
* \param yoffset
*/
void Camera::processMouseScroll(float yoffset) {
    // Zoom in
    if (yoffset == 1)
        zoom += fabs(zoom) * 0.1f;

    // Zoom out
    else if (yoffset == -1)
        zoom -= fabs(zoom) * 0.1f;
}

