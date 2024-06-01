/*****************************************************************************
 * Camera.cpp
 *
 * Descrição:
 * ----------
 * Este arquivo contém a implementação da classe Camera, que representa a câmera virtual no jogo. A classe Camera é responsável por:
 * - Controlar a posição, orientação e zoom da câmera.
 * - Responder a eventos do rato e scroll para manipular a câmera.
 * - Calcular as matrizes de visualização e projeção para renderizar a cena.
 *
 * Funções principais:
 * - Camera(float zoom = 10.0f, const glm::vec3& rotationAngles = glm::vec3(0.0f), const glm::mat4& model = glm::mat4(1.0f), const glm::mat4& proj = glm::mat4(1.0f)): Construtor da classe Camera.
 * - mouseClickCallback(GLFWwindow* window, int button, int action, int mods): Callback para eventos de clique do rato.
 * - mouseMovementCallback(GLFWwindow* window, double xpos, double ypos): Callback para eventos de movimento do rato.
 * - scrollCallback(GLFWwindow* window, double xoffset, double yoffset): Callback para eventos de rolagem do rato (scroll).
 * - getViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up): Calcula a matriz de visualização (view matrix).
 * - getMatrizZoom(): Calcula a matriz de zoom.
 * - setupCamera(const glm::vec3& position, const glm::vec3& target, float aspectRatio): Configura a câmera.
 *
 * Variáveis e constantes importantes:
 * - zoom: Nível de zoom da câmera.
 * - clickPos: Posição do clique do rato.
 * - prevClickPos: Posição anterior do clique do rato.
 * - rotationAngles: Ângulos de rotação da câmera (pitch, yaw, roll).
 * - model: Matriz de modelo da câmera.
 * - proj: Matriz de projeção da câmera.
 * - view: Matriz de visualização da câmera.
 *
 ******************************************************************************/


#include "camera.h"


 /*****************************************************************************
 * Camera::Camera(float zoom = 10.0f, const glm::vec3& rotationAngles = glm::vec3(0.0f),
 *         const glm::mat4& model = glm::mat4(1.0f), const glm::mat4& proj = glm::mat4(1.0f))
 *
 * Descrição:
 * ----------
 * Este é o construtor da classe `Camera`, responsável por inicializar uma nova
 * câmera virtual com os parâmetros fornecidos ou com valores padrão.
 *
 * Parâmetros:
 * -----------
 * - zoom (opcional): O nível de zoom inicial da câmera (padrão: 10.0f).
 * - rotationAngles (opcional): Os ângulos de rotação iniciais da câmera em radianos (padrão: glm::vec3(0.0f)).
 * - model (opcional): A matriz de modelo inicial da câmera (padrão: glm::mat4(1.0f)).
 * - proj (opcional): A matriz de projeção inicial da câmera (padrão: glm::mat4(1.0f)).
 *
 * Retorno:
 * --------
 * - Nenhum (construtor).
 *
 * Observações:
 * -----------
 * - O construtor utiliza uma lista de inicialização para inicializar os membros da classe
 *  de forma mais eficiente.
 * - Os parâmetros são opcionais, permite que o construtor seja chamado com diferentes
 *  configurações iniciais da câmera.
 * - Se nenhum valor for fornecido para um parâmetro, o construtor usará valores padrão.
 *
 ******************************************************************************/
Camera::Camera(float zoom, const glm::vec3& rotationAngles, const glm::mat4& model, const glm::mat4& proj) :
	zoom(zoom), rotationAngles(rotationAngles), model(model), proj(proj) {
	clickPos = glm::vec2(0.0f);
	prevClickPos = glm::vec2(0.0f);
	view = glm::mat4(1.0f);
}


/*****************************************************************************
 * void Camera::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
 *
 * Descrição:
 * ----------
 * Esta é a função de callback que é chamada pela GLFW sempre que um evento de
 * clique do rato ocorre na janela especificada. A função é responsável por
 * capturar a posição do clique e atualizar os ângulos de rotação da câmera,
 * permite que o utilizador controle a rotação da câmera com o rato.
 *
 * Parâmetros:
 * -----------
 * - window: Ponteiro para a janela GLFW onde o evento de clique ocorreu.
 * - button: O botão do rato que foi pressionado ou liberado (ex: GLFW_MOUSE_BUTTON_LEFT).
 * - action: A ação do rato (GLFW_PRESS ou GLFW_RELEASE).
 * - mods: Bits de modificador que indica se teclas como Shift, Ctrl ou Alt estavam pressionadas.
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
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


/*****************************************************************************
 * void Camera::mouseMovementCallback(GLFWwindow* window, double xpos, double ypos)
 *
 * Descrição:
 * ----------
 * Esta é a função de callback chamada pela GLFW sempre que o rato é movido na
 * janela especificada. Ela é responsável por atualizar os ângulos de rotação da
 * câmera com base no movimento do rato, permite que o utilizador controle a
 * rotação da câmera ao arrastar o rato.
 *
 * Parâmetros:
 * -----------
 * - window: Ponteiro para a janela GLFW onde o evento de movimento do rato ocorreu.
 * - xpos: A nova posição horizontal (eixo X) do cursor do rato na janela.
 * - ypos: A nova posição vertical (eixo Y) do cursor do rato na janela.
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
void Camera::mouseMovementCallback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		prevClickPos = clickPos;
		clickPos = glm::vec2(xpos, ypos);
		glm::vec2 clickDelta = clickPos - prevClickPos;
		const float sensitivity = 0.004f;
		rotationAngles.y += clickDelta.x * sensitivity;
	}
}


/*****************************************************************************
 * void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
 *
 * Descrição:
 * ----------
 * Esta função de callback é chamada pela GLFW sempre que o utilizador rola o scroll
 * do rato na janela especificada. Ela é responsável por ajustar o nível de zoom
 * da câmera com base na direção da rolagem.
 *
 * Parâmetros:
 * -----------
 * - window: Ponteiro para a janela GLFW onde o evento de scroll ocorreu.
 * - xoffset: O deslocamento horizontal (eixo X) da rolagem (geralmente não utilizado para zoom).
 * - yoffset: O deslocamento vertical (eixo Y) da rolagem (usado para determinar a direção do zoom).
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset == 1) {
		zoom += fabs(zoom) * 0.1f;
	}
	else if (yoffset == -1) {
		zoom -= fabs(zoom) * 0.1f;
	}
}


/*****************************************************************************
 * glm::mat4 Camera::getViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
 *
 * Descrição:
 * ----------
 * Esta função calcula a matriz de visualização (View Matrix) da câmera, que é
 * essencial para transformar as coordenadas dos objetos da cena em coordenadas
 * de visualização, ou seja, em relação à posição e orientação da câmera.
 *
 * Parâmetros:
 * -----------
 * - position: A posição (x, y, z) da câmera no espaço 3D.
 * - target: O ponto (x, y, z) para o qual a câmera está a olhar.
 * - up: O vetor (x, y, z) "para cima" da câmera, que define a orientação vertical da câmera.
 *
 * Retorno:
 * --------
 * - glm::mat4: A matriz de visualização (4x4) calculada.
 *
 * Observações:
 * -----------
 * - A matriz de visualização é utilizada em conjunto com a matriz de modelo (Model Matrix) e a matriz
 *  de projeção (Projection Matrix) para transformar as coordenadas dos vértices dos objetos da cena
 *  em coordenadas de tela.
 * - A função `glm::lookAt` simplifica o cálculo da matriz de visualização, que seria mais complexo
 *  se feito manualmente.
 *
 ******************************************************************************/
glm::mat4 Camera::getViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) {
	return glm::lookAt(position, target, up);
}


/*****************************************************************************
 * glm::mat4 Camera::getMatrizZoom()
 *
 * Descrição:
 * ----------
 * Esta função calcula e retorna a matriz de zoom da câmera. A matriz de zoom é
 * uma matriz de transformação que escala uniformemente um objeto em todas as
 * direções (x, y e z) com base no fator de zoom atual da câmera.
 *
 * Parâmetros:
 * -----------
 * - Nenhum.
 *
 * Retorno:
 * --------
 * - glm::mat4: A matriz de zoom (4x4) calculada.
 *
 * Observações:
 * -----------
 * - A matriz de zoom é utilizada em conjunto com a matriz de visualização e a matriz
 *  de modelo para aplicar o zoom corretamente aos objetos da cena.
 * - A função `glm::scale` simplifica o cálculo da matriz de zoom, que seria mais complexo
 *  se feito manualmente.
 *
 ******************************************************************************/
glm::mat4 Camera::getMatrizZoom() {
	return glm::scale(glm::mat4(1.0f), glm::vec3(zoom));
}


/*****************************************************************************
 * void Camera::setupCamera(const glm::vec3& position, const glm::vec3& target, float aspectRatio)
 *
 * Descrição:
 * ----------
 * Esta função configura os parâmetros da câmera, como posição, orientação e projeção,
 * para preparar a renderização da cena a partir do ponto de vista da câmera.
 *
 * Parâmetros:
 * -----------
 * - position: A posição (x, y, z) da câmera no espaço 3D.
 * - target: O ponto (x, y, z) para o qual a câmera está a olhar.
 * - aspectRatio: A proporção da largura pela altura da tela.
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
void Camera::setupCamera(const glm::vec3& position, const glm::vec3& target, float aspectRatio) {
	glm::vec3 camFront = position - target;
	glm::vec3 camRight = glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 up = -glm::cross(camFront, camRight);

	proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	view = getViewMatrix(position, target, up);
}