#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
	// Vari�veis de membro p�blicas
	GLfloat zoom;            // N�vel de zoom da c�mera
	glm::vec2 clickPos;      // Posi��o do clique do mouse
	glm::vec2 prevClickPos;  // Posi��o anterior do clique do mouse
	glm::vec3 rotationAngles; // �ngulos de rota��o da c�mera (pitch, yaw, roll)
	glm::mat4 model;        // Matriz de modelo da c�mera (transforma��es do objeto)
	glm::mat4 proj;        // Matriz de proje��o da c�mera (perspectiva)
	glm::mat4 view;        // Matriz de visualiza��o da c�mera (posi��o e orienta��o)

	// Construtor
	Camera(float zoom = 10.0f, const glm::vec3& rotationAngles = glm::vec3(0.0f), const glm::mat4& model = glm::mat4(1.0f), const glm::mat4& proj = glm::mat4(1.0f));

	// Fun��es de callback para eventos do mouse e scroll
	void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
	void mouseMovementCallback(GLFWwindow* window, double xpos, double ypos);
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	// Fun��es utilit�rias para a c�mera
	glm::mat4 getViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up); // Calcula a matriz de visualiza��o
	glm::mat4 getMatrizZoom();                            // Calcula a matriz de zoom
	void setupCamera(const glm::vec3& position, const glm::vec3& target, float aspectRatio); // Configura a c�mera
};

#endif // CAMERA_H
