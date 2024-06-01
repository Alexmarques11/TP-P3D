/*****************************************************************************
 * Camera.cpp
 *
 * Descri��o:
 * ----------
 * Este arquivo cont�m a implementa��o da classe Camera, que representa a c�mera virtual no jogo. A classe Camera � respons�vel por:
 * - Controlar a posi��o, orienta��o e zoom da c�mera.
 * - Responder a eventos do rato e scroll para manipular a c�mera.
 * - Calcular as matrizes de visualiza��o e proje��o para renderizar a cena.
 *
 * Fun��es principais:
 * - Camera(float zoom = 10.0f, const glm::vec3& rotationAngles = glm::vec3(0.0f), const glm::mat4& model = glm::mat4(1.0f), const glm::mat4& proj = glm::mat4(1.0f)): Construtor da classe Camera.
 * - mouseClickCallback(GLFWwindow* window, int button, int action, int mods): Callback para eventos de clique do rato.
 * - mouseMovementCallback(GLFWwindow* window, double xpos, double ypos): Callback para eventos de movimento do rato.
 * - scrollCallback(GLFWwindow* window, double xoffset, double yoffset): Callback para eventos de rolagem do rato (scroll).
 * - getViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up): Calcula a matriz de visualiza��o (view matrix).
 * - getMatrizZoom(): Calcula a matriz de zoom.
 * - setupCamera(const glm::vec3& position, const glm::vec3& target, float aspectRatio): Configura a c�mera.
 *
 * Vari�veis e constantes importantes:
 * - zoom: N�vel de zoom da c�mera.
 * - clickPos: Posi��o do clique do rato.
 * - prevClickPos: Posi��o anterior do clique do rato.
 * - rotationAngles: �ngulos de rota��o da c�mera (pitch, yaw, roll).
 * - model: Matriz de modelo da c�mera.
 * - proj: Matriz de proje��o da c�mera.
 * - view: Matriz de visualiza��o da c�mera.
 *
 ******************************************************************************/


#include "camera.h"


 /*****************************************************************************
 * Camera::Camera(float zoom = 10.0f, const glm::vec3& rotationAngles = glm::vec3(0.0f),
 *         const glm::mat4& model = glm::mat4(1.0f), const glm::mat4& proj = glm::mat4(1.0f))
 *
 * Descri��o:
 * ----------
 * Este � o construtor da classe `Camera`, respons�vel por inicializar uma nova
 * c�mera virtual com os par�metros fornecidos ou com valores padr�o.
 *
 * Par�metros:
 * -----------
 * - zoom (opcional): O n�vel de zoom inicial da c�mera (padr�o: 10.0f).
 * - rotationAngles (opcional): Os �ngulos de rota��o iniciais da c�mera em radianos (padr�o: glm::vec3(0.0f)).
 * - model (opcional): A matriz de modelo inicial da c�mera (padr�o: glm::mat4(1.0f)).
 * - proj (opcional): A matriz de proje��o inicial da c�mera (padr�o: glm::mat4(1.0f)).
 *
 * Retorno:
 * --------
 * - Nenhum (construtor).
 *
 * Observa��es:
 * -----------
 * - O construtor utiliza uma lista de inicializa��o para inicializar os membros da classe
 *  de forma mais eficiente.
 * - Os par�metros s�o opcionais, permite que o construtor seja chamado com diferentes
 *  configura��es iniciais da c�mera.
 * - Se nenhum valor for fornecido para um par�metro, o construtor usar� valores padr�o.
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
 * Descri��o:
 * ----------
 * Esta � a fun��o de callback que � chamada pela GLFW sempre que um evento de
 * clique do rato ocorre na janela especificada. A fun��o � respons�vel por
 * capturar a posi��o do clique e atualizar os �ngulos de rota��o da c�mera,
 * permite que o utilizador controle a rota��o da c�mera com o rato.
 *
 * Par�metros:
 * -----------
 * - window: Ponteiro para a janela GLFW onde o evento de clique ocorreu.
 * - button: O bot�o do rato que foi pressionado ou liberado (ex: GLFW_MOUSE_BUTTON_LEFT).
 * - action: A a��o do rato (GLFW_PRESS ou GLFW_RELEASE).
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
 * Descri��o:
 * ----------
 * Esta � a fun��o de callback chamada pela GLFW sempre que o rato � movido na
 * janela especificada. Ela � respons�vel por atualizar os �ngulos de rota��o da
 * c�mera com base no movimento do rato, permite que o utilizador controle a
 * rota��o da c�mera ao arrastar o rato.
 *
 * Par�metros:
 * -----------
 * - window: Ponteiro para a janela GLFW onde o evento de movimento do rato ocorreu.
 * - xpos: A nova posi��o horizontal (eixo X) do cursor do rato na janela.
 * - ypos: A nova posi��o vertical (eixo Y) do cursor do rato na janela.
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
 * Descri��o:
 * ----------
 * Esta fun��o de callback � chamada pela GLFW sempre que o utilizador rola o scroll
 * do rato na janela especificada. Ela � respons�vel por ajustar o n�vel de zoom
 * da c�mera com base na dire��o da rolagem.
 *
 * Par�metros:
 * -----------
 * - window: Ponteiro para a janela GLFW onde o evento de scroll ocorreu.
 * - xoffset: O deslocamento horizontal (eixo X) da rolagem (geralmente n�o utilizado para zoom).
 * - yoffset: O deslocamento vertical (eixo Y) da rolagem (usado para determinar a dire��o do zoom).
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
 * Descri��o:
 * ----------
 * Esta fun��o calcula a matriz de visualiza��o (View Matrix) da c�mera, que �
 * essencial para transformar as coordenadas dos objetos da cena em coordenadas
 * de visualiza��o, ou seja, em rela��o � posi��o e orienta��o da c�mera.
 *
 * Par�metros:
 * -----------
 * - position: A posi��o (x, y, z) da c�mera no espa�o 3D.
 * - target: O ponto (x, y, z) para o qual a c�mera est� a olhar.
 * - up: O vetor (x, y, z) "para cima" da c�mera, que define a orienta��o vertical da c�mera.
 *
 * Retorno:
 * --------
 * - glm::mat4: A matriz de visualiza��o (4x4) calculada.
 *
 * Observa��es:
 * -----------
 * - A matriz de visualiza��o � utilizada em conjunto com a matriz de modelo (Model Matrix) e a matriz
 *  de proje��o (Projection Matrix) para transformar as coordenadas dos v�rtices dos objetos da cena
 *  em coordenadas de tela.
 * - A fun��o `glm::lookAt` simplifica o c�lculo da matriz de visualiza��o, que seria mais complexo
 *  se feito manualmente.
 *
 ******************************************************************************/
glm::mat4 Camera::getViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) {
	return glm::lookAt(position, target, up);
}


/*****************************************************************************
 * glm::mat4 Camera::getMatrizZoom()
 *
 * Descri��o:
 * ----------
 * Esta fun��o calcula e retorna a matriz de zoom da c�mera. A matriz de zoom �
 * uma matriz de transforma��o que escala uniformemente um objeto em todas as
 * dire��es (x, y e z) com base no fator de zoom atual da c�mera.
 *
 * Par�metros:
 * -----------
 * - Nenhum.
 *
 * Retorno:
 * --------
 * - glm::mat4: A matriz de zoom (4x4) calculada.
 *
 * Observa��es:
 * -----------
 * - A matriz de zoom � utilizada em conjunto com a matriz de visualiza��o e a matriz
 *  de modelo para aplicar o zoom corretamente aos objetos da cena.
 * - A fun��o `glm::scale` simplifica o c�lculo da matriz de zoom, que seria mais complexo
 *  se feito manualmente.
 *
 ******************************************************************************/
glm::mat4 Camera::getMatrizZoom() {
	return glm::scale(glm::mat4(1.0f), glm::vec3(zoom));
}


/*****************************************************************************
 * void Camera::setupCamera(const glm::vec3& position, const glm::vec3& target, float aspectRatio)
 *
 * Descri��o:
 * ----------
 * Esta fun��o configura os par�metros da c�mera, como posi��o, orienta��o e proje��o,
 * para preparar a renderiza��o da cena a partir do ponto de vista da c�mera.
 *
 * Par�metros:
 * -----------
 * - position: A posi��o (x, y, z) da c�mera no espa�o 3D.
 * - target: O ponto (x, y, z) para o qual a c�mera est� a olhar.
 * - aspectRatio: A propor��o da largura pela altura da tela.
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