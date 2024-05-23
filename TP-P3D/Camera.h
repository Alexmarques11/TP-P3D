#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera {

public:
    // Variáveis de membro públicas
    GLfloat zoom;
    glm::vec2 clickPos;
    glm::vec2 prevClickPos;
    glm::vec3 rotationAngles;
    glm::mat4 model;
    glm::mat4 proj;
    glm::mat4 view;

    // Construtor
    Camera(float zoom = 10.0f, const glm::vec3& rotationAngles = glm::vec3(0.0f), const glm::mat4& model = glm::mat4(1.0f), const glm::mat4& proj = glm::mat4(1.0f));

    // Funções de callback
    void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
    void mouseMovementCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    // Função utilitária para a câmera
    glm::mat4 getViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);
    glm::mat4 getMatrizZoom();
    void setupCamera(const glm::vec3& position, const glm::vec3& target, float aspectRatio);
};

#endif // CAMERA_H
