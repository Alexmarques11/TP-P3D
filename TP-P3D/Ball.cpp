#define _CRT_SECURE_NO_WARNINGS
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

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW/glfw3.h>

const float Ball::BALL_RADIUS = 0.035f;

// Construtor que recebe a posição inicial, textureIndex e shaderProgram
Ball::Ball(const glm::vec3& initialPosition, GLuint textureIndex, GLuint shaderProgram, Camera* camera, Lights* lights, bool isMoving, glm::vec3 orientation)
	: position(initialPosition), textureIndex(textureIndex), ShaderProgram(shaderProgram), cameraPtr(camera), lightsPtr(lights), isMoving(isMoving), orientation(orientation) {

	// Gerar o VAO e VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

//Funcao para ler os ficheiros obj
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

//Funcao para enviar os dados
void Ball::Install() {

	// Vincular o VAO e VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Aplicar uma escala para diminuir o tamanho
	float scale = 0.040f;
	for (size_t i = 0; i < vertices.size(); i++) {
		vertices[i] *= scale;
	}

	//Guardar os dados no VBO
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	//Configurar os dados
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Criar um VBO para as normais
	GLuint normalsVBO;
	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	// Configurar os dados
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	//Criar um VBO para as uvs
	GLuint uvsVBO;
	glGenBuffers(1, &uvsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvsVBO);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

	// Configurar os dados
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	//Criar um VBO para as texturas
	GLint textura = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "textSampler");
	glProgramUniform1i(ShaderProgram, textura, 0);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "OpenGL Error: " << error << std::endl;
	}

}

//Funcao para desenhar uma bola
void Ball::Render(glm::vec3 position, glm::vec3 orientation) {

	//Vincula o VAO
	glBindVertexArray(VAO);

	glm::mat4 Model = cameraPtr->model;

	//Posicao da Bola
	Model = glm::translate(Model, position);

	//Orientação da bola
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
	glProgramUniformMatrix4fv(ShaderProgram, normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glUniform1i(glGetUniformLocation(ShaderProgram, "ambientLightEnabled"), lightsPtr->isAmbientLightEnabled);
	glUniform1i(glGetUniformLocation(ShaderProgram, "directionalLightEnabled"), lightsPtr->isDirectionalLightEnabled);
	glUniform1i(glGetUniformLocation(ShaderProgram, "pointLightEnabled[0]"), lightsPtr->isPointLightEnabled);
	glUniform1i(glGetUniformLocation(ShaderProgram, "spotLightEnabled"), lightsPtr->isSpotLightEnabled);

	// Define os parâmetros da luz ambiente
	if (lightsPtr->isAmbientLightEnabled) {
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
	}

	// Define os parâmetros da luz direcional
	if (lightsPtr->isDirectionalLightEnabled) {
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	}

	// Define os parâmetros da luz pontual #1
	if (lightsPtr->isPointLightEnabled) {
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].constant"), 1.0f);
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].linear"), 0.06f);
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "pointLight[0].quadratic"), 0.02f);
	}

	// Define os parâmetros da luz spot
	if (lightsPtr->isSpotLightEnabled) {
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.constant"), 1.0f);
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.linear"), 0.06f);
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.quadratic"), 0.02f);
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.spotCutoff"), glm::cos(glm::radians(12.5f)));
		glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.spotExponent"), 2.0f);
		glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "spotLight.spotDirection"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.2f)));
	}

	// Define as propriedades dos materiais
	glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "material.emissive"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
	glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "material.ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
	glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(ShaderProgram, glGetUniformLocation(ShaderProgram, "material.specular"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
	glProgramUniform1f(ShaderProgram, glGetUniformLocation(ShaderProgram, "material.shininess"), 32.0f);

	glBindTexture(GL_TEXTURE_2D, textureIndex);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}


void Ball::Update(float deltaTime, const std::vector<Ball>& balls) {

	if (isMoving) {

		if (IsColliding(balls)) {
			// Se houver colisão, pare o movimento
			isMoving = false;
		}

		position.x += SPEED * deltaTime;

		// Calcular a distância percorrida
		float distance = SPEED * deltaTime;

		// Atualizar a rotação da bola com base na distância percorrida
		orientation.z -= glm::degrees(distance / BALL_RADIUS);
	}
}


//Funcao para ler um ficheiro mtl
void Ball::LoadMTL(char* mtl_model_filepath) {

	char lineHeader[128];

	FILE* mtlFile;
	errno_t err;
	err = fopen_s(&mtlFile, mtl_model_filepath, "r");
	if (mtlFile == NULL) {
		printf("Impossible to open the file !\n");
		return;
	}

	while (lineHeader[0] != EOF) {

		int res = fscanf_s(mtlFile, "%s", lineHeader, sizeof(lineHeader));
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop

		if (strcmp(lineHeader, "Ka") == 0) {
			glm::vec3 ambientColor;
			fscanf_s(mtlFile, "%f %f %f", &ambientColor.r, &ambientColor.g, &ambientColor.b);
		}
		else if (strcmp(lineHeader, "Kd") == 0) {
			glm::vec3 diffuseColor;
			fscanf_s(mtlFile, "%f %f %f", &diffuseColor.r, &diffuseColor.g, &diffuseColor.b);
		}
		else if (strcmp(lineHeader, "Ks") == 0) {
			glm::vec3 specularColor;
			fscanf_s(mtlFile, "%f %f %f", &specularColor.r, &specularColor.g, &specularColor.b);
		}
		else if (strcmp(lineHeader, "Ns") == 0) {
			float shininess;
			fscanf_s(mtlFile, "%f", &shininess);
		}
		else if (strcmp(lineHeader, "map_Kd") == 0) {
			char textureFilename[128];
			fscanf_s(mtlFile, "%s", textureFilename, sizeof(textureFilename));
			LoadTexture(textureFilename);
		}
	}

}

//Funcao para carregar uma textura
void Ball::LoadTexture(const char* textureFileName) {

	// Gera um nome de textura
	glGenTextures(1, &textureIndex);

	// Ativa a Unidade de Textura #0
	// A Unidade de Textura 0 já está ativa por defeito.
	// Só uma Unidade de Textura está ativa a cada momento.
	glActiveTexture(GL_TEXTURE0);

	// Vincula esse nome de textura ao target GL_TEXTURE_2D da Unidade de Textura ativa.
	glBindTexture(GL_TEXTURE_2D, textureIndex);

	// Define os parâmetros de filtragem (wrapping e ajuste de tamanho)
	// para a textura que está vinculada ao target GL_TEXTURE_2D da Unidade de Textura ativa.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Leitura/descompressão do ficheiro com imagem de textura
	int width, height, nChannels;
	// Ativa a inversão vertical da imagem, aquando da sua leitura para memória.
	stbi_set_flip_vertically_on_load(true);
	// Leitura da imagem para memória do CPU
	unsigned char* imageData = stbi_load(textureFileName, &width, &height, &nChannels, 0);
	if (imageData) {
		// Carrega os dados da imagem para o Objeto de Textura vinculado ao target GL_TEXTURE_2D da Unidade de Textura ativa.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);

		// Gera o Mipmap para essa textura
		glGenerateMipmap(GL_TEXTURE_2D);

		// Liberta a imagem da memória do CPU
		stbi_image_free(imageData);
	}
	else {
		std::cout << "Error loading texture!" << std::endl;
	}
}

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

	// Verificar colisão com as paredes
	if (position.x + BALL_RADIUS > 0.9f || position.x - BALL_RADIUS < -0.9f ||
		position.z + BALL_RADIUS > 0.45f || position.z - BALL_RADIUS < -0.45f) {
		return true;
	}

	return false;
}


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