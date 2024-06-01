#ifndef BALL_H
#define BALL_H

#include <GL/glew.h> 
#include <string>
#include <vector>  
#include <glm/glm.hpp>
#include "Camera.h"
#include "Lights.h"

class Ball {

private:

	glm::vec3 ambientColor; // Cor ambiente da bola (iluminação ambiente)
	glm::vec3 diffuseColor; // Cor difusa da bola (iluminação difusa)
	glm::vec3 specularColor; // Cor especular da bola (reflexo da luz)
	float shininess;     // Brilho da bola (intensidade do reflexo)

	static const float BALL_RADIUS; // Raio constante de todas as bolas
	const float SPEED = 0.1f;     // Velocidade da bola

	Camera* cameraPtr; // Ponteiro para a câmera
	Lights* lightsPtr; // Ponteiro para as luzes

	GLuint VAO;      // Vertex Array Object (armazena configurações de vértices)
	GLuint VBO;      // Vertex Buffer Object (armazena dados dos vértices)
	GLuint ShaderProgram;  // Programa de shader (combina shaders de vértice e fragmento)
	GLuint textureIndex;  // Índice da textura da bola

	void LoadMTL(char* mtl_model_filepath); // Carrega o material da bola (arquivo .mtl)
	void LoadTexture(const char* textureFileName); // Carrega a textura da bola

	// Função para verificar colisão com outras bolas
	bool IsColliding(const std::vector<Ball>& balls);

public:

	// Dados do modelo 3D (.obj)
	std::vector<glm::vec3> vertices; // Coordenadas dos vértices
	std::vector<glm::vec2> uvs;    // Coordenadas de textura (UV)
	std::vector<glm::vec3> normals;  // Normais dos vértices

	glm::vec3 position;  // Posição atual da bola
	glm::vec3 orientation; // Orientação da bola
	bool isMoving;    // Indica se a bola está em movimento

	// Construtor da bola
	Ball(const glm::vec3& initialPosition, GLuint textureIndex, GLuint shaderProgram, Camera* camera, Lights* lights, bool isMoving = false, glm::vec3 orientation = glm::vec3(0, 0, 0));

	// Funções da bola
	void Load(const std::string obj_model_filepath); // Carrega o modelo 3D da bola
	void Install();                // Configura os buffers e atributos da bola
	void Render(glm::vec3 position, glm::vec3 orientation); // Renderiza a bola
	void Update(float deltaTime, const std::vector<Ball>& balls); // Atualiza a posição e estado da bola

	// Retorna as posições iniciais de todas as bolas
	static std::vector<glm::vec3> GetBallInitialPositions();
};


#endif 
