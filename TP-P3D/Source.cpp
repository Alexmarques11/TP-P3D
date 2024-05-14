#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
#include <vector>

#include <Windows.h>

#define GLEW_STATIC
#include <GL\glew.h>

#include <GLFW\glfw3.h>
#include <gl\GL.h>

#include "Utils.h"
#include "Ball.h"
#include "Table.h"
#include "Camera.h"

#include <glm/glm.hpp> // vec3, vec4, ivec4, mat4, ...
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective, ...
#include <glm/gtc/type_ptr.hpp> // value_ptr


Camera camera;

/**
* \brief Função para inicializar o OpenGL
*/
void initOpenGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Define a cor de fundo
	glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade para renderização 3D

	// Descomentar para ativar o Face Culling
	//glEnable(GL_CULL_FACE);
}


/**
* \brief Função de callback para eventos de rolagem do mouse
* 
* \param window
* \param xoffset
* \param yoffset
*/
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processMouseScroll(yoffset);
}


/**
* \brief Função de callback para eventos de movimento do cursor
*
* \param window
* \param xoffset
* \param yoffset
*/
void cursorPositionCallback(GLFWwindow* window, double xoffset, double yoffset) {

	bool isLeftMouseButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	camera.processMouseMovement(xoffset, isLeftMouseButtonPressed);
}


/**
* \brief Função para renderizar o modelo 3D na tela
*
* \param table
* \param mvp
*/
void renderTable(Table table, glm::mat4 mvp) {

	float* vertex_stream = static_cast<float*>(glm::value_ptr(table.getVertices().front()));
	std::vector<glm::vec3> colors = table.getColors();

	// Desenha quad em modo imediato
	glBegin(GL_QUADS);
	for (int nv = 0; nv < 6 * 4 * 3; nv += 3) {
		// Define a cor para o vértice atual
		glColor3f(colors[nv / (4 * 3)].r, colors[nv / (4 * 3)].g, colors[nv / (4 * 3)].b);
		glm::vec4 vertex = glm::vec4(vertex_stream[nv], vertex_stream[nv + 1], vertex_stream[nv + 2], 1.0f);

		// Cálculo das coordenadas de recorte
		glm::vec4 transformed_vertex = mvp * vertex;
		
		// Divisão de Perspetiva
		glm::vec4 normalized_vertex = transformed_vertex / transformed_vertex.w;
		
		// Desenho do vértice
		glVertex3f(normalized_vertex.x, normalized_vertex.y, normalized_vertex.z);
	}
	glEnd();
}


/**
void renderBall(Ball ball, glm::mat4 mvp) {

	std::vector<glm::vec3> vertices = ball.getVertices();
	std::vector<unsigned int> indices = ball.getVertexIndices();

	// Desenha os triângulos da bola
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < indices.size(); ++i) {
		glm::vec4 vertex = glm::vec4(vertices[indices[i]], 1.0f);
		glm::vec4 transformed_vertex = mvp * vertex;
		glm::vec4 normalized_vertex = transformed_vertex / transformed_vertex.w;
		glVertex3f(normalized_vertex.x, normalized_vertex.y, normalized_vertex.z);
	}
	glEnd();
}
*/


void renderBall(Ball ball, glm::mat4 mvp) {
	std::vector<glm::vec3> vertices = ball.getVertices();
	std::vector<glm::vec2> uvs = ball.uvs;
	std::vector<unsigned int> indices = ball.getVertexIndices();
	std::unordered_map<std::string, Material> materials = ball.getMaterials();

	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < indices.size(); ++i) {
		unsigned int vertexIndex = indices[i];
		glm::vec3 vertex = vertices[vertexIndex];
		glm::vec2 uv = uvs[vertexIndex];
		std::string materialName = ball.getCurrentMaterial();
		Material material = materials[materialName];

		//log material diffuse
		//std::cout << "Material Diffuse: " << material.diffuse.r << " " << material.diffuse.g << " " << material.diffuse.b << std::endl;

		//glColor3f(material.diffuse.r, material.diffuse.g, material.diffuse.b);
		glm::vec4 transformed_vertex = mvp * glm::vec4(vertex, 1.0f);
		glm::vec4 normalized_vertex = transformed_vertex / transformed_vertex.w;
		glVertex3f(normalized_vertex.x, normalized_vertex.y, normalized_vertex.z);
	}
	glEnd();
}




int main(void) {
	// Inicialização do GLFW
	if (!glfwInit()) {
		std::cerr << "Erro ao inicializar o GLFW" << std::endl;
		return -1;
	}

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "P3D - Trabalho Pratico", NULL, NULL);
	if (!window) {
		std::cerr << "Erro ao criar a janela GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Tornar a janela GLFW atual
	glfwMakeContextCurrent(window);

	// Inicializar o OpenGL
	initOpenGL();

	// Definir callbacks para eventos de input
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);

	// Carregar a mesa
	Table table;
	Ball ball("PoolBalls/Ball1.obj");

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// MVP
		glm::mat4 mvp = camera.getMvp();

		// Renderizar a mesa
		renderTable(table, mvp);

		// Renderizar a bola
		renderBall(ball, mvp);

		// Troca os buffers e verifica eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}