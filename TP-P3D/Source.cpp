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

float currentBallRotation = 0.0f;

GLuint VAO, VBO, EBO;

std::vector<glm::vec3> ballPositions = Ball::GetBallInitialPositions();
std::vector<Ball> balls;

Camera* cameraPtr = new Camera();
Lights* lightsPtr = new Lights();

/*****************************************************************************
 * void handleKeypress(GLFWwindow* window, int key, int scancode, int action, int mods)
 *
 * Descrição:
 * ----------
 * Esta é a função de callback chamada pela GLFW sempre que uma tecla é pressionada ou liberada.
 * Ela lida com eventos específicos de teclas, como iniciar o movimento da bola 9 e alternar as luzes.
 *
 * Parâmetros:
 * -----------
 * - window: Ponteiro para a janela da GLFW onde o evento de tecla ocorreu.
 * - key: Código da tecla pressionada ou liberada (ex: GLFW_KEY_SPACE).
 * - scancode: Código de varredura da tecla (geralmente não utilizado).
 * - action: A ação realizada na tecla (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT).
 * - mods: Bits de modificador que indica se teclas como Shift, Ctrl ou Alt estavam pressionadas.
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
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

/*****************************************************************************
 * int main(void)
 *
 * Descrição:
 * ----------
 * Este é o ponto de entrada principal do programa de simulação de mesa de bilhar.
 * O programa utiliza as bibliotecas GLFW, GLEW e GLM para criar uma janela, gerenciar
 * o contexto OpenGL, carregar e usar shaders, e realizar operações matemáticas 3D.
 *
 * O programa cria uma mesa de bilhar e 16 bolas. A câmera pode ser movida ao clicar
 * e ao arrastar com o botão esquerdo do rato, e o zoom pode ser ajustado com o scroll
 * do rato. A barra de espaço inicia o movimento da bola 9, e as teclas 1, 2, 3 e 4
 * alternam a luz ambiente, direcional, luz pontual e spot, respectivamente.
 *
 * Fluxo do Programa:
 * 1. Inicialização:
 *  - Inicializa GLFW e GLEW.
 *  - Cria a janela do jogo.
 *  - Define o contexto OpenGL.
 *  - Habilita o teste de profundidade.
 *  - Registra callbacks para eventos de teclado, rato e scroll.
 *  - Configura a posição e o alvo da câmera.
 *  - Carrega os shaders para as bolas e a mesa.
 *  - Cria os objetos da mesa e das bolas.
 * 2. Loop Principal:
 *  - Enquanto a janela não for fechada:
 *   - Limpa o buffer de cor e profundidade.
 *   - Atualiza a matriz de modelo da câmera com base na rotação.
 *   - Renderiza as bolas e a mesa.
 *   - Troca os buffers da janela para mostrar o quadro renderizado.
 *   - Processa eventos de entrada.
 * 3. Finalização:
 *  - Libera os recursos do OpenGL.
 *  - Destrói a janela do jogo.
 *  - Encerra a GLFW.
 *
 ******************************************************************************/

int main(void) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "PoolTable", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);

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

	glm::vec3 cameraPosition(0.0f, 10.0f, 20.0f);
	glm::vec3 cameraTarget(0.0f);
	float aspectRatio = 800.0f / 800.0f;
	cameraPtr->setupCamera(cameraPosition, cameraTarget, aspectRatio);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "Shaders/ball.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/ball.frag" },
		{ GL_NONE, NULL }
	};

	GLuint shaderProgram = LoadShaders(shaders);
	if (!shaderProgram)
		exit(EXIT_FAILURE);

	glUseProgram(shaderProgram);

	ShaderInfo tableshaders[] = {
		{ GL_VERTEX_SHADER, "Shaders/table.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/table.frag" },
		{ GL_NONE, NULL }
	};

	GLuint tableProgram = LoadShaders(tableshaders);

	Table table(tableProgram, cameraPtr, lightsPtr);

	for (int i = 0; i < ballPositions.size(); ++i) {

		Ball ball(ballPositions[i], i + 1, shaderProgram, cameraPtr, lightsPtr);
		ball.Load("Ball" + std::to_string(i + 1) + ".obj");
		ball.Install();
		balls.push_back(ball);
	}

	float lastFrameTime = 0.0f;
	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cameraPtr->model = glm::rotate(cameraPtr->model, glm::radians(cameraPtr->rotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 matrizZoom = cameraPtr->getMatrizZoom();

		glBindVertexArray(VAO);

		glUseProgram(shaderProgram);

		float currentFrameTime = glfwGetTime();
		float deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		for (size_t i = 0; i < balls.size(); ++i) {
			balls[i].Update(deltaTime, balls);
			balls[i].Render(balls[i].position, balls[i].orientation);
			balls[i].Update(deltaTime, balls);

			balls[i].Render(balls[i].position, balls[i].orientation);
		}

		table.Render();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}
