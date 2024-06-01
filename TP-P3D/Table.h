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
	Table(GLuint tableProgram, Camera* camera, Lights* lights); // Construtor da mesa
	~Table(); // Destrutor da mesa

	void Render(); // Renderiza a mesa

private:
	GLuint VAO, VBO, EBO;  // Vertex Array Object, Vertex Buffer Object e Element Buffer Object

	GLuint tableProgram;    // Programa de shader da mesa
	Camera* cameraPtr;   // Ponteiro para a câmera
	Lights* lightsPtr;    // Ponteiro para as luzes

	void Load(); // Carrega os dados da mesa (vértices, índices, etc.)
};

#endif // TABLE_H
