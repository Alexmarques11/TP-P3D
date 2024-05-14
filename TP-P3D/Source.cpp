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

#include "LoadShaders.h"
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
void renderBall(Ball& ball, glm::mat4 mvp, GLuint program) {

	// Ativar o programa de shader
	glUseProgram(program);

	// Passar a matriz MVP para o shader
	GLuint mvpLocation = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

	// Vincular o VAO da bola
	glBindVertexArray(ball.vao);

	// Ativar e vincular a textura
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ball.currentMaterial->diffuseTexture);
	GLuint diffuseLocation = glGetUniformLocation(program, "diffuseTexture");
	glUniform1i(diffuseLocation, 0);

	// Desenhar a bola
	glDrawElements(GL_TRIANGLES, ball.vertexIndices.size(), GL_UNSIGNED_INT, 0);

	// Desvincular o VAO
	glBindVertexArray(0);

	// Desativar o programa de shader
	glUseProgram(0);

}*/

void renderBall(Ball& ball, glm::mat4 mvp, GLuint program) {
	// Ativar o programa de shader
	glUseProgram(program);

	// Passar as matrizes MVP para o shader
	GLuint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // Assuming no model transformation for now

	GLuint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));

	GLuint projectionLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjection()));

	// Passar informações do material para o shader
	GLuint materialAmbientLoc = glGetUniformLocation(program, "material.ambient");
	glUniform3fv(materialAmbientLoc, 1, glm::value_ptr(ball.currentMaterial->ambient));

	GLuint materialDiffuseLoc = glGetUniformLocation(program, "material.diffuse");
	glUniform3fv(materialDiffuseLoc, 1, glm::value_ptr(ball.currentMaterial->diffuse));

	GLuint materialSpecularLoc = glGetUniformLocation(program, "material.specular");
	glUniform3fv(materialSpecularLoc, 1, glm::value_ptr(ball.currentMaterial->specular));

	GLuint materialShininessLoc = glGetUniformLocation(program, "material.shininess");
	glUniform1f(materialShininessLoc, ball.currentMaterial->shininess);

	// Vincular o VAO da bola
	glBindVertexArray(ball.vao);

	// Ativar e vincular a textura
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ball.currentMaterial->diffuseTexture);
	GLuint diffuseLocation = glGetUniformLocation(program, "diffuseTexture");
	glUniform1i(diffuseLocation, 0);

	// Desenhar a bola
	glDrawElements(GL_TRIANGLES, ball.vertexIndices.size(), GL_UNSIGNED_INT, 0);

	// Desvincular o VAO
	glBindVertexArray(0);

	// Desativar o programa de shader
	glUseProgram(0);
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

	// Inicialização do GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "Erro ao inicializar o GLEW: " << glewGetErrorString(err) << std::endl;
		return -1;
	}

	// Inicializar o OpenGL
	initOpenGL();

	// Carregar os shaders
	ShaderInfo shaders[] = {
	{ GL_VERTEX_SHADER, "Shaders/texture.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/texture.frag" },
	{ GL_NONE, NULL }
	};

	GLuint shaderProgram = LoadShaders(shaders);
	if (shaderProgram == 0) {
		std::cerr << "Erro ao carregar shaders" << std::endl;
		return -1;
	}

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
		renderBall(ball, mvp, shaderProgram);

		// Troca os buffers e verifica eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}