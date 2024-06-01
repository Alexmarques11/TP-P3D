/*****************************************************************************
 * Ball.cpp
 *
 * Descri��o:
 * ----------
 * Este arquivo cont�m a implementa��o da classe Ball, que representa uma bola de bilhar no jogo. A classe Ball � respons�vel por:
 * - Carregar o modelo 3D da bola a partir de um arquivo .obj e .mtl.
 * - Carregar a textura da bola.
 * - Configurar os buffers e atributos da bola (VAO, VBO).
 * - Renderizar a bola na cena.
 * - Atualizar a posi��o e estado da bola (movimento, colis�es).
 * - Verificar colis�es com outras bolas e com as paredes da mesa.
 *
 * Fun��es principais:
 * - Ball(const glm::vec3& initialPosition, GLuint textureIndex, GLuint shaderProgram, Camera* camera, Lights* lights, bool isMoving = false, glm::vec3 orientation = glm::vec3(0, 0, 0)): Construtor da classe Ball.
 * - Load(const std::string obj_model_filepath): Carrega o modelo 3D da bola.
 * - LoadMTL(char* mtl_model_filepath): Carrega o material da bola.
 * - LoadTexture(const char* textureFileName): Carrega a textura da bola.
 * - Install(): Configura os buffers e atributos da bola.
 * - Render(glm::vec3 position, glm::vec3 orientation): Renderiza a bola.
 * - Update(float deltaTime, const std::vector<Ball>& balls): Atualiza a posi��o e estado da bola.
 * - IsColliding(const std::vector<Ball>& balls): Verifica colis�es com outras bolas.
 * - GetBallInitialPositions(): Retorna as posi��es iniciais de todas as bolas.
 *
 * Vari�veis e constantes importantes:
 * - BALL_RADIUS: Raio da bola.
 * - SPEED: Velocidade de movimento da bola.
 * - position: Posi��o atual da bola.
 * - orientation: Orienta��o da bola.
 * - isMoving: Indica se a bola est� em movimento.
 * - vertices, uvs, normals: Vetores que armazenam os dados do modelo 3D da bola.
 * - VAO, VBO, ShaderProgram: Vari�veis para configura��o e renderiza��o da bola.
 * - cameraPtr, lightsPtr: Apontadores para a c�mera e as luzes do jogo.
 *
 ******************************************************************************/


#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "stb_image.h"
#include "Ball.h"
#include "LoadShaders.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define _CRT_SECURE_NO_WARNINGS
#define GLEW_STATIC
#define GLFW_USE_DWM_SWAP_INTERVAL

const float Ball::BALL_RADIUS = 0.035f;

/*****************************************************************************
 * Ball::Ball(const glm::vec3& initialPosition, GLuint textureIndex, GLuint shaderProgram,
 * Camera* camera, Lights* lights, bool isMoving = false, glm::vec3 orientation = glm::vec3(0, 0, 0))
 *
 * Descri��o:
 * ----------
 * Este � o construtor da classe Ball, respons�vel por inicializar uma nova bola de bilhar no jogo.
 * Ele recebe como par�metros a posi��o inicial da bola, o �ndice da textura que ser� aplicada,
 * o programa de shader a ser utilizado, um apontador para a c�mera e um apontador para as luzes do jogo.
 * Al�m disso, tamb�m recebe par�metros opcionais para indicar se a bola est� em movimento (`isMoving`)
 * e a sua orienta��o (`orientation`), com valores padr�o para estes.
 *
 * Retorno:
 * --------
 * - Nenhum (construtor).
 *
 ******************************************************************************/
Ball::Ball(const glm::vec3& initialPosition, GLuint textureIndex, GLuint shaderProgram, Camera* camera, Lights* lights, bool isMoving, glm::vec3 orientation)
	: position(initialPosition), textureIndex(textureIndex), ShaderProgram(shaderProgram), cameraPtr(camera), lightsPtr(lights), isMoving(isMoving), orientation(orientation) {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}


/*****************************************************************************
 * void Ball::Load(const std::string obj_model_filepath)
 *
 * Descri��o:
 * ----------
 * Carrega o modelo 3D da bola a partir de um arquivo Wavefront OBJ (`obj_model_filepath`).
 * O arquivo OBJ � um formato de arquivo de texto que descreve a geometria de um objeto 3D,
 * inclui v�rtices, coordenadas de textura e normais.
 *
 * Par�metros:
 * -----------
 * - obj_model_filepath: Caminho para o arquivo OBJ que cont�m o modelo da bola.
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
void Ball::Load(const std::string obj_model_filepath) {
	FILE* file;
	errno_t err;
	err = fopen_s(&file, obj_model_filepath.c_str(), "r");
	if (file == NULL) {
		throw("Impossible to open the file !\n");
		return;
	}

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	while (1) {
		char lineHeader[128];
		int res = fscanf_s(file, "%s", lineHeader, (unsigned int)_countof(lineHeader));
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "mtllib") == 0) {
			char materialsFilename[128];

			fscanf_s(file, "%s\n", materialsFilename, (unsigned int)_countof(materialsFilename));
			LoadMTL(materialsFilename);
		}

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}

		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}

		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}

		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				throw("Failed to read face information\n");
				return;
			}

			for (int i = 0; i < 3; i++)
			{
				vertices.push_back(temp_vertices.at(vertexIndex[i] - 1));
				uvs.push_back(temp_uvs.at(uvIndex[i] - 1));
				normals.push_back(temp_normals.at(normalIndex[i] - 1));
			}

		};
	}
	fclose(file);
	return;

}


/*****************************************************************************
 * void Ball::Install()
 *
 * Descri��o:
 * ----------
 * Configura os buffers (Vertex Buffer Objects - VBOs) e atributos de v�rtice
 * para a renderiza��o da bola. Esta fun��o � essencial para enviar os dados
 * do modelo da bola (v�rtices, normais, coordenadas de textura) para a placa
 * gr�fica (GPU) de forma que ela possa process�-los e renderizar a bola no ecr�.
 *
 * Par�metros:
 * -----------
 * - Nenhum.
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
void Ball::Install() {

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float scale = 0.040f;
	for (size_t i = 0; i < vertices.size(); i++) {
		vertices[i] *= scale;
	}

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint normalsVBO;
	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	GLuint uvsVBO;
	glGenBuffers(1, &uvsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvsVBO);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	GLint textura = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "textSampler");
	glProgramUniform1i(ShaderProgram, textura, 0);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "OpenGL Error: " << error << std::endl;
	}

}


/*****************************************************************************
 * void Ball::Render(glm::vec3 position, glm::vec3 orientation)
 *
 * Descri��o:
 * ----------
 * Esta fun��o renderiza a bola na cena 3D, ao utilizar as informa��es de posi��o
 * e orienta��o fornecidas. Ela aplica as transforma��es de modelo, visualiza��o e
 * proje��o da c�mera, configura as propriedades de ilumina��o e material da bola
 * e, por fim, desenha os tri�ngulos que comp�em o modelo da bola no ecr�.
 *
 * Par�metros:
 * -----------
 * - position: A posi��o (x, y, z) da bola no mundo.
 * - orientation: A orienta��o (x, y, z) da bola em radianos.
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
void Ball::Render(glm::vec3 position, glm::vec3 orientation) {
	glBindVertexArray(VAO);

	glm::mat4 Model = cameraPtr->model;
	Model = glm::translate(Model, position);
	Model = glm::rotate(Model, glm::radians(orientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	Model = glm::rotate(Model, glm::radians(orientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	Model = glm::rotate(Model, glm::radians(orientation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	GLint viewId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "View");
	glProgramUniformMatrix4fv(ShaderProgram, viewId, 1, GL_FALSE, glm::value_ptr(cameraPtr->view * cameraPtr->getMatrizZoom()));

	GLint projectionId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "Projection");
	glProgramUniformMatrix4fv(ShaderProgram, projectionId, 1, GL_FALSE, glm::value_ptr(cameraPtr->proj));

	GLint modelId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "Model");
	glProgramUniformMatrix4fv(ShaderProgram, modelId, 1, GL_FALSE, glm::value_ptr(Model));

	glm::mat4 modelView = cameraPtr->view * cameraPtr->getMatrizZoom() * Model;
	GLint modelViewId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "ModelView");
	glProgramUniformMatrix4fv(ShaderProgram, modelViewId, 1, GL_FALSE, glm::value_ptr(modelView));

	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelView));
	GLint normalMatrixId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "NormalMatrix");
	glProgramUniformMatrix3fv(ShaderProgram, normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glUniform1i(glGetUniformLocation(ShaderProgram, "ambientLightEnabled"), lightsPtr->isAmbientLightEnabled);
	glUniform1i(glGetUniformLocation(ShaderProgram, "directionalLightEnabled"), lightsPtr->isDirectionalLightEnabled);
	glUniform1i(glGetUniformLocation(ShaderProgram, "pointLightEnabled[0]"), lightsPtr->isPointLightEnabled);
	glUniform1i(glGetUniformLocation(ShaderProgram, "spotLightEnabled"), lightsPtr->isSpotLightEnabled);

	if (lightsPtr->isAmbientLightEnabled) {
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
	}

	if (lightsPtr->isDirectionalLightEnabled) {
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0, -0.5, 0.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	}

	if (lightsPtr->isPointLightEnabled) {
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].constant"), 1.0f);
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].linear"), 0.09f);
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].quadratic"), 0.032f);
	}

	if (lightsPtr->isSpotLightEnabled) {
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.constant"), 1.0f);
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.linear"), 0.09f); // Ajuste de atenua��o
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.quadratic"), 0.032f); // Ajuste de atenua��o
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.spotCutoff"), glm::cos(glm::radians(12.5f)));
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.spotExponent"), 2.0f);
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.spotDirection"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.2f)));
	}

	glUniform3fv(glGetUniformLocation(ShaderProgram, "material.ambient"), 1, glm::value_ptr(ambientColor));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "material.diffuse"), 1, glm::value_ptr(diffuseColor));
	glUniform3fv(glGetUniformLocation(ShaderProgram, "material.specular"), 1, glm::value_ptr(specularColor));
	glUniform1f(glGetUniformLocation(ShaderProgram, "material.shininess"), shininess);

	glBindTexture(GL_TEXTURE_2D, textureIndex);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}


/*****************************************************************************
 * void Ball::Update(float deltaTime, const std::vector<Ball>& balls)
 *
 * Descri��o:
 * ----------
 * Esta fun��o atualiza o estado da bola a cada quadro da simula��o. Ela verifica
 * se a bola est� em movimento e, em caso afirmativo, atualiza sua posi��o, rota��o
 * e verifica se houve colis�o com outras bolas.
 *
 * Par�metros:
 * -----------
 * - deltaTime: Tempo decorrido desde o �ltimo quadro da simula��o.
 * - balls: Refer�ncia constante para o vetor de bolas, usado para verificar colis�es.
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
void Ball::Update(float deltaTime, const std::vector<Ball>& balls) {

	if (isMoving) {

		if (IsColliding(balls)) {
			isMoving = false;
		}

		position.x += SPEED * deltaTime;

		float distance = SPEED * deltaTime;

		orientation.z -= glm::degrees(distance / BALL_RADIUS);
	}
}


/*****************************************************************************
 * void Ball::LoadMTL(char* mtl_model_filepath)
 *
 * Descri��o:
 * ----------
 * Carrega as propriedades do material da bola a partir de um arquivo MTL (Material Template Library).
 * O arquivo MTL define as caracter�sticas visuais do material, como cor ambiente, cor difusa,
 * cor especular, brilho e textura.
 *
 * Par�metros:
 * -----------
 * - mtl_model_filepath: Caminho para o arquivo MTL que cont�m as propriedades do material da bola.
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
void Ball::LoadMTL(char* mtl_model_filepath) {
	char lineHeader[128];

	FILE* mtlFile;
	errno_t err;
	err = fopen_s(&mtlFile, mtl_model_filepath, "r");
	if (mtlFile == NULL) {
		printf("Impossible to open the file !\n");
		return;
	}

	while (true) {
		int res = fscanf_s(mtlFile, "%s", lineHeader, sizeof(lineHeader));
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop

		if (strcmp(lineHeader, "Ka") == 0) {
			fscanf_s(mtlFile, "%f %f %f", &ambientColor.r, &ambientColor.g, &ambientColor.b);
		}
		else if (strcmp(lineHeader, "Kd") == 0) {
			fscanf_s(mtlFile, "%f %f %f", &diffuseColor.r, &diffuseColor.g, &diffuseColor.b);
		}
		else if (strcmp(lineHeader, "Ks") == 0) {
			fscanf_s(mtlFile, "%f %f %f", &specularColor.r, &specularColor.g, &specularColor.b);
		}
		else if (strcmp(lineHeader, "Ns") == 0) {
			fscanf_s(mtlFile, "%f", &shininess);
		}
		else if (strcmp(lineHeader, "map_Kd") == 0) {
			char textureFilename[128];
			fscanf_s(mtlFile, "%s", textureFilename, sizeof(textureFilename));
			LoadTexture(textureFilename);
		}
	}

	fclose(mtlFile);
}


/*****************************************************************************
 * void Ball::LoadTexture(const char* textureFileName)
 *
 * Descri��o:
 * ----------
 * Carrega uma textura 2D a partir de um arquivo de imagem e configura os par�metros
 * da textura para uso na renderiza��o da bola. A textura � carregada na mem�ria da
 * GPU para ser utilizada no processo de renderiza��o.
 *
 * Par�metros:
 * -----------
 * - textureFileName: O nome do arquivo de imagem que cont�m a textura a ser carregada.
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
void Ball::LoadTexture(const char* textureFileName) {

	glGenTextures(1, &textureIndex);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, textureIndex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nChannels;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* imageData = stbi_load(textureFileName, &width, &height, &nChannels, 0);
	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(imageData);
	}
	else {
		std::cout << "Error loading texture!" << std::endl;
	}
}


/*****************************************************************************
 * bool Ball::IsColliding(const std::vector<Ball>& balls)
 *
 * Descri��o:
 * ----------
 * Esta fun��o membro da classe `Ball` verifica se a bola est� a colidir com outras
 * bolas na mesa ou com as paredes da mesa. Ela retorna `true` se houver colis�o
 * e `false` caso contr�rio.
 *
 * Par�metros:
 * -----------
 * - balls: Refer�ncia constante para o vetor de bolas na mesa.
 *
 * Retorno:
 * --------
 * - bool: `true` se a bola estiver a colidir com outra bola ou com as paredes da mesa,
 *   `false` caso contr�rio.
 *
 ******************************************************************************/
bool Ball::IsColliding(const std::vector<Ball>& balls) {
	for (size_t i = 0; i < balls.size(); i++) {
		if (&balls[i] != this) {
			float distance = glm::distance(position, balls[i].position);
			float minDistance = 2 * BALL_RADIUS;
			if (distance <= minDistance) {
				return true;
			}
		}
	}

	if (position.x + BALL_RADIUS > 0.9f || position.x - BALL_RADIUS < -0.9f ||
		position.z + BALL_RADIUS > 0.45f || position.z - BALL_RADIUS < -0.45f) {
		return true;
	}

	return false;
}


/*****************************************************************************
 * std::vector<glm::vec3> Ball::GetBallInitialPositions()
 *
 * Descri��o:
 * ----------
 * Esta fun��o est�tica retorna um vetor (std::vector) que cont�m as posi��es
 * iniciais de todas as bolas de bilhar no jogo. Cada posi��o � representada por
 * um vetor glm::vec3, que cont�m as coordenadas x, y e z da posi��o da bola no
 * espa�o 3D.
 *
 * Par�metros:
 * -----------
 * - Nenhum.
 *
 * Retorno:
 * --------
 * - std::vector<glm::vec3>: Um vetor que cont�m as posi��es iniciais de todas as bolas.
 *
 ******************************************************************************/
std::vector<glm::vec3> Ball::GetBallInitialPositions() {
	std::vector<glm::vec3> ballPositions = {
		glm::vec3(-0.5f, 0.1f, 0.2f),
		glm::vec3(-0.3f, 0.1f, 0.3f),
		glm::vec3(-0.1f, 0.1f, -0.2f),
		glm::vec3(0.0f, 0.1f, 0.4f),
		glm::vec3(-0.6f, 0.1f, -0.4f),
		glm::vec3(-0.4f, 0.1f, -0.3f),
		glm::vec3(-0.2f, 0.1f, 0.2f),
		glm::vec3(0.3f, 0.1f, -0.4f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.5f, 0.1f, -0.1f),
		glm::vec3(0.2f, 0.1f, 0.3f),
		glm::vec3(0.4f, 0.1f, -0.2f),
		glm::vec3(0.6f, 0.1f, 0.1f),
		glm::vec3(0.7f, 0.1f, -0.3f),
		glm::vec3(0.8f, 0.1f, 0.4f)
	};

	return ballPositions;
}