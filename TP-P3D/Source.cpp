/*****************************************************************************
 * Source.cpp
 *
 * Descrição:
 * ----------
 * Este é o arquivo principal do projeto Pool Table. Ele é responsável por:
 * - Inicializar a biblioteca GLFW e criar a janela do jogo.
 * - Configurar o contexto OpenGL.
 * - Carregar e compilar os shaders para as bolas e a mesa.
 * - Configurar a cãmera e as luzes do jogo.
 * - Criar e carregar os objetos da mesa e das bolas.
 * - Executar o loop principal do jogo, onde as bolas são atualizadas e renderizadas, e a cãmera responde aos comandos do utilizador.
 *
 * Funções principais:
 * - handleKeypress(GLFWwindow* window, int key, int scancode, int action, int mods): Callback para eventos de teclado.
 * - main(): Função principal do programa.
 *
 * Variáveis e constantes importantes:
 * - window: Ponteiro para a janela do jogo.
 * - shaderProgram: Referência ao programa de shader das bolas.
 * - tableProgram: Referência ao programa de shader da mesa.
 * - ballPositions: Vetor com as posições iniciais das bolas.
 * - balls: Vetor que armazena os objetos das bolas.
 * - cameraPtr: Ponteiro para o objeto da câmera.
 * - lightsPtr: Ponteiro para o objeto das luzes.
 *
 ******************************************************************************/

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW/glfw3.h>

#include "Table.h"
#include "Ball.h"
#include "LoadShaders.h"
#include "Camera.h"
#include "Lights.h"

 // Variável para controlar a rotação da bola durante a animação
float currentBallRotation = 0.0f;

// Variáveis para criação de VAO, VBO, EBO
GLuint VAO, VBO, EBO;

// Posições das bolas de bilhar
std::vector<glm::vec3> ballPositions = Ball::GetBallInitialPositions();
std::vector<Ball> balls;

// Instancias
Camera* cameraPtr = new Camera();
Lights* lightsPtr = new Lights();

// Função callback para verificar se uma tecla foi pressionada
void handleKeypress(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action != GLFW_PRESS)
		return;

	switch (key) {
	case GLFW_KEY_SPACE:
		balls[8].isMoving = true;
		std::cout << "Ball 9 started rolling!" << std::endl;
		break;
	case GLFW_KEY_1:
		lightsPtr->ToggleLight(1);
		break;
	case GLFW_KEY_2:
		lightsPtr->ToggleLight(2);
		break;
	case GLFW_KEY_3:
		lightsPtr->ToggleLight(3);
		break;
	case GLFW_KEY_4:
		lightsPtr->ToggleLight(4);
		break;
	default:
		break;
	}
}


int main(void) {
	// Inicializar glfw para criar uma janela
	glfwInit();

	// Glfw não sabe que versão do OpenGL estamos a usar, temos de dizer isso através de hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // especificamos que vamos usar a versão do OpenGL 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // especificar que OpenGL profile queremos usar

	// Width, height, name of the window, fullscreen ou não
	GLFWwindow* window = glfwCreateWindow(800, 800, "PoolTable", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	// Usar a janela
	glfwMakeContextCurrent(window);

	// Inicia o gestor de extensões GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Habilita o teste de profundidade
	glEnable(GL_DEPTH_TEST);

	// Chamar as funções callback
	glfwSetKeyCallback(window, handleKeypress);
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		cameraPtr->mouseClickCallback(window, button, action, mods);
		});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		cameraPtr->mouseMovementCallback(window, xpos, ypos);
		});
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
		cameraPtr->scrollCallback(window, xoffset, yoffset);
		});

	// Configurar a câmera
	glm::vec3 cameraPosition(0.0f, 10.0f, 20.0f);
	glm::vec3 cameraTarget(0.0f);
	float aspectRatio = 800.0f / 800.0f;
	cameraPtr->setupCamera(cameraPosition, cameraTarget, aspectRatio);

	// Array de informações dos shaders para o programa das bolas
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "Shaders/ball.vert" }, // Shader de vértice
		{ GL_FRAGMENT_SHADER, "Shaders/ball.frag" }, // Shader de fragmento
		{ GL_NONE, NULL } // Marcação de fim do array
	};

	// Carrega os shaders e cria o programa das bolas
	GLuint shaderProgram = LoadShaders(shaders);
	if (!shaderProgram)
		exit(EXIT_FAILURE);

	// Usa o programa das bolas para a renderização
	glUseProgram(shaderProgram);

	// Array de informações dos shaders para o programa da mesa
	ShaderInfo tableshaders[] = {
		{ GL_VERTEX_SHADER, "Shaders/table.vert" }, // Shader de vértice da mesa
		{ GL_FRAGMENT_SHADER, "Shaders/table.frag" }, // Shader de fragmento da mesa
		{ GL_NONE, NULL } // Marcação de fim do array
	};

	// Carrega os shaders e cria o programa da mesa
	GLuint tableProgram = LoadShaders(tableshaders);

	// Chama a função para carregar os dados da mesa
	Table table(tableProgram, cameraPtr, lightsPtr);

	// Cria e carrega as bolas
	for (int i = 0; i < ballPositions.size(); ++i) {

		Ball ball(ballPositions[i], i + 1, shaderProgram, cameraPtr, lightsPtr);
		ball.Load("Ball" + std::to_string(i + 1) + ".obj");
		ball.Install();
		balls.push_back(ball);
	}

	float lastFrameTime = 0.0f;
	while (!glfwWindowShouldClose(window)) {

		// Limpar buffer de cor e profundidade
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Aplica a rotação ao modelo
		cameraPtr->model = glm::rotate(cameraPtr->model, glm::radians(cameraPtr->rotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));

		// Matriz ZOOM
		glm::mat4 matrizZoom = cameraPtr->getMatrizZoom();

		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);

		// Dizer que programa usar (usamos o programa das bolas)
		glUseProgram(shaderProgram);

		// get delta time
		float currentFrameTime = glfwGetTime();
		float deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		for (size_t i = 0; i < balls.size(); ++i) {
			balls[i].Update(deltaTime, balls);
			balls[i].Render(balls[i].position, balls[i].orientation);
		}

		// Desenhar a mesa
		table.Render();

		glfwSwapBuffers(window);

		// Dizer ao glfw para processar todos os eventos
		glfwPollEvents();
	}

	// Apaga o VAO, VBO, EBO e o programa shader
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	// Destruir a janela
	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}
