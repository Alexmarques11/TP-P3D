#include "camera.h"

Camera::Camera(float zoom, const glm::vec3& rotationAngles, const glm::mat4& model, const glm::mat4& proj) :
    zoom(zoom), rotationAngles(rotationAngles), model(model), proj(proj) {
    clickPos = glm::vec2(0.0f);
    prevClickPos = glm::vec2(0.0f);
    view = glm::mat4(1.0f);
}

void Camera::mouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        clickPos = glm::vec2(xpos, ypos);
        prevClickPos = clickPos;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        rotationAngles.x = 0.0f;
        rotationAngles.y = 0.0f;
    }
    model = glm::rotate(model, glm::radians(rotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::mouseMovementCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        prevClickPos = clickPos;
        clickPos = glm::vec2(xpos, ypos);
        glm::vec2 clickDelta = clickPos - prevClickPos;
        const float sensitivity = 0.004f;
        rotationAngles.y += clickDelta.x * sensitivity;
    }
}

void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset == 1) {
        zoom += fabs(zoom) * 0.1f;
    }
    else if (yoffset == -1) {
        zoom -= fabs(zoom) * 0.1f;
    }
}

glm::mat4 Camera::getViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getMatrizZoom() {
    return glm::scale(glm::mat4(1.0f), glm::vec3(zoom));
}

void Camera::setupCamera(const glm::vec3& position, const glm::vec3& target, float aspectRatio) {
    glm::vec3 camFront = position - target;
    glm::vec3 camRight = glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 up = -glm::cross(camFront, camRight);

    proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    view = getViewMatrix(position, target, up);
}