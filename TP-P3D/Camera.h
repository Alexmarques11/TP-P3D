#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
	// Variáveis de membro públicas
	GLfloat zoom;            // Nível de zoom da câmera
	glm::vec2 clickPos;      // Posição do clique do mouse
	glm::vec2 prevClickPos;  // Posição anterior do clique do mouse
	glm::vec3 rotationAngles; // Ângulos de rotação da câmera (pitch, yaw, roll)
	glm::mat4 model;        // Matriz de modelo da câmera (transformações do objeto)
	glm::mat4 proj;        // Matriz de projeção da câmera (perspectiva)
	glm::mat4 view;        // Matriz de visualização da câmera (posição e orientação)

	// Construtor
	Camera(float zoom = 10.0f, const glm::vec3& rotationAngles = glm::vec3(0.0f), const glm::mat4& model = glm::mat4(1.0f), const glm::mat4& proj = glm::mat4(1.0f));

	// Funções de callback para eventos do mouse e scroll
	void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
	void mouseMovementCallback(GLFWwindow* window, double xpos, double ypos);
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	// Funções utilitárias para a câmera
	glm::mat4 getViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up); // Calcula a matriz de visualização
	glm::mat4 getMatrizZoom();                            // Calcula a matriz de zoom
	void setupCamera(const glm::vec3& position, const glm::vec3& target, float aspectRatio); // Configura a câmera
};

#endif // CAMERA_H
