/*****************************************************************************
 * Table.cpp
 *
 * Descri��o:
 * ----------
 * Este arquivo cont�m a implementa��o da classe Table, que representa a mesa de bilhar no jogo. A classe Table � respons�vel por:
 * - Carregar e definir os v�rtices e �ndices que comp�em a geometria da mesa.
 * - Configurar os buffers de v�rtices (VBO) e de elementos (EBO) para armazenar os dados da mesa.
 * - Renderizar a mesa no ecr� ao utilizar um programa de shader espec�fico.
 * - Configurar a ilumina��o da mesa, que inclui a luz ambiente, direcional, luz pontual e spot.
 *
 * Fun��es principais:
 * - Table(GLuint tableProgram, Camera* camera, Lights* lights): Construtor da classe Table.
 * - ~Table(): Destrutor da classe Table, que libera os recursos alocados.
 * - Load(): Carrega os dados da mesa (v�rtices, �ndices) e configura os buffers.
 * - Render(): Renderiza a mesa no ecr�, ao aplicar as transforma��es de c�mera e configura��es de luz.
 *
 * Vari�veis e constantes importantes:
 * - VAO, VBO, EBO: Identificadores dos objetos de vertex array, vertex buffer e element buffer, respectivamente.
 * - tableProgram: Identificador do programa de shader usado para renderizar a mesa.
 * - cameraPtr: Ponteiro para o objeto da c�mera.
 * - lightsPtr: Ponteiro para o objeto das luzes.
 * - vertices: Array que armazena as coordenadas dos v�rtices da mesa.
 * - indices: Array que armazena os �ndices dos v�rtices para formar os tri�ngulos da mesa.
 *
 ******************************************************************************/

#include "Table.h"
#include "LoadShaders.h"

 /*****************************************************************************
 * Table::Table(GLuint tableProgram, Camera* camera, Lights* lights)
 *
 * Descri��o:
 * ----------
 * Este � o construtor da classe `Table`, respons�vel por inicializar uma nova
 * inst�ncia da mesa de bilhar. Ele recebe como par�metros o programa de shader
 * a ser utilizado para renderizar a mesa, um apontador para a c�mera e um apontador
 * para as luzes do jogo.
 *
 * Par�metros:
 * -----------
 * - tableProgram: O identificador do programa de shader a ser utilizado para renderizar a mesa.
 * - camera: Ponteiro para o objeto da c�mera do jogo.
 * - lights: Ponteiro para o objeto das luzes do jogo.
 *
 * Retorno:
 * --------
 * - Nenhum (construtor).
 *
 * Observa��es:
 * -----------
 * - O construtor utiliza uma lista de inicializa��o para inicializar os membros da classe
 *  de forma mais eficiente do que faria no corpo do construtor.
 * - A fun��o `Load` � chamada imediatamente ap�s a inicializa��o dos membros para garantir
 *  que os dados da mesa estejam prontos para a renderiza��o.
 *
 ******************************************************************************/
Table::Table(GLuint tableProgram, Camera* camera, Lights* lights) : tableProgram(tableProgram), cameraPtr(camera), lightsPtr(lights) {
	Load();
}


/*****************************************************************************
 * Table::~Table()
 *
 * Descri��o:
 * ----------
 * Este � o destrutor da classe `Table`, respons�vel por liberar os recursos
 * alocados durante a cria��o e utiliza��o da mesa de bilhar, como os buffers
 * OpenGL (Vertex Array Object - VAO, Vertex Buffer Object - VBO e Element Buffer Object - EBO).
 *
 * Par�metros:
 * -----------
 * - Nenhum.
 *
 * Retorno:
 * --------
 * - Nenhum (destrutor).
 *
 * Observa��es:
 * -----------
 * - O destrutor � chamado automaticamente quando um objeto da classe `Table` � destru�do.
 * - A libera��o dos recursos OpenGL � importante para evitar vazamentos de mem�ria
 *  e garantir que os recursos da GPU sejam utilizados de forma eficiente.
 *
 ******************************************************************************/
Table::~Table() {
	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}


/*****************************************************************************
 * void Table::Load()
 *
 * Descri��o:
 * ----------
 * Esta fun��o membro da classe `Table` � respons�vel por carregar os dados da geometria da mesa de bilhar
 * e configurar os buffers OpenGL necess�rios para a renderiza��o. Ela define os v�rtices e �ndices que
 * comp�em a mesa, cria e vincula os objetos Vertex Array Object (VAO), Vertex Buffer Object (VBO) e
 * Element Buffer Object (EBO), e envia os dados para a placa gr�fica (GPU).
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
void Table::Load() {

	GLfloat vertices[] = {
		// Frente       //Normal
		-0.9f, -0.05f, 0.45f,  0.0f, 0.0f, 1.0f,
		0.9f, -0.05f, 0.45f,  0.0f, 0.0f, 1.0f,
		0.9f, 0.05f, 0.45f,   0.0f, 0.0f, 1.0f,
		-0.9f, 0.05f, 0.45f,  0.0f, 0.0f, 1.0f,

		// Tr�s
		-0.9f, -0.05f, -0.45f,  0.0f, 0.0f, -1.0f,
		-0.9f, 0.05f, -0.45f,  0.0f, 0.0f, -1.0f,
		0.9f, 0.05f, -0.45f,   0.0f, 0.0f, -1.0f,
		0.9f, -0.05f, -0.45f,  0.0f, 0.0f, -1.0f,

		// Direita
		0.9f, -0.05f, 0.45f,   1.0f, 0.0f, 0.0f,
		0.9f, -0.05f, -0.45f,  1.0f, 0.0f, 0.0f,
		0.9f, 0.05f, -0.45f,   1.0f, 0.0f, 0.0f,
		0.9f, 0.05f, 0.45f,   1.0f, 0.0f, 0.0f,

		// Esquerda
		-0.9f, -0.05f, 0.45f,  -1.0f, 0.0f, 0.0f,
		-0.9f, 0.05f, 0.45f,   -1.0f, 0.0f, 0.0f,
		-0.9f, 0.05f, -0.45f,  -1.0f, 0.0f, 0.0f,
		-0.9f, -0.05f, -0.45f,  -1.0f, 0.0f, 0.0f,

		// Cima											  
		-0.9f, 0.05f, 0.45f,   0.0f, 1.0f, 0.0f,
		0.9f, 0.05f, 0.45f,   0.0f, 1.0f, 0.0f,
		0.9f, 0.05f, -0.45f,   0.0f, 1.0f, 0.0f,
		-0.9f, 0.05f, -0.45f,  0.0f, 1.0f, 0.0f,

		// Baixo
		-0.9f, -0.05f, 0.45f,  	0.0f, -1.0f, 0.0f,
		-0.9f, -0.05f, -0.45f, 	0.0f, -1.0f, 0.0f,
		0.9f, -0.05f, -0.45f,  	0.0f, -1.0f, 0.0f,
		0.9f, -0.05f, 0.45f ,  	0.0f, -1.0f, 0.0f,
	};

	GLuint indices[] = {
		// Face frontal
		0, 1, 2,
		2, 3, 0,

		// Face traseira
		4, 5, 6,
		6, 7, 4,

		// Face direita
		8, 9, 10,
		10, 11, 8,

		// Face esquerda
		12, 13, 14,
		14, 15, 12,

		// Face superior
		16, 17, 18,
		18, 19, 16,

		// Face inferior
		20, 21, 22,
		22, 23, 20
	};

	// Gera um �nico objeto de VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Gera um �nico buffer de v�rtices (VBO) e um �nico buffer de elementos (EBO)
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Define o VAO atual, vincula-o
	glBindVertexArray(VAO);

	// Vincula o VBO como um buffer do tipo GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Armazena os v�rtices no VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Tipo de buffer, tamanho, dados, uso dos dados

	// Vincula o EBO como um buffer do tipo GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Configura o VAO para o OpenGL saber como ler o VBO
	// Especifica o layout do atributo de v�rtice 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	// Especifica o layout do atributo de v�rtice 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Habilita o atributo de v�rtice 0
	glEnableVertexAttribArray(0);
	// Habilita o atributo de v�rtice 1
	glEnableVertexAttribArray(1);

	// N�o obrigat�rio, mas para garantir que n�o alteramos o VAO e o VBO acidentalmente
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Isso deve ser feito ap�s o VAO, porque o EBO est� armazenado no VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

/*****************************************************************************
 * void Table::Render()
 *
 * Descri��o:
 * ----------
 * Esta fun��o membro da classe `Table` � respons�vel por renderizar a mesa de
 * bilhar na cena. Ela utiliza o Vertex Array Object (VAO) previamente carregado,
 * configura as matrizes de transforma��o (modelo, visualiza��o e proje��o) e
 * os par�metros de ilumina��o do shader, e finalmente desenha a mesa no ecr�.
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
void Table::Render()
{
	glBindVertexArray(VAO);

	glUseProgram(tableProgram);

	int modelLoc = glGetUniformLocation(tableProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cameraPtr->model));
	int viewLoc = glGetUniformLocation(tableProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cameraPtr->view * cameraPtr->getMatrizZoom()));
	int projLoc = glGetUniformLocation(tableProgram, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cameraPtr->proj));

	glUniform1i(glGetUniformLocation(tableProgram, "ambientLightEnabled"), lightsPtr->isAmbientLightEnabled);
	glUniform1i(glGetUniformLocation(tableProgram, "directionalLightEnabled"), lightsPtr->isDirectionalLightEnabled);
	glUniform1i(glGetUniformLocation(tableProgram, "pointLightEnabled"), lightsPtr->isPointLightEnabled);
	glUniform1i(glGetUniformLocation(tableProgram, "spotLightEnabled"), lightsPtr->isSpotLightEnabled);

	glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "material.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
	glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "material.diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "material.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glProgramUniform1f(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "material.shininess"), 32.0f);

	if (lightsPtr->isAmbientLightEnabled) {
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
	}

	if (lightsPtr->isDirectionalLightEnabled) {
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0, -0.5, 0.0)));
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
	}

	if (lightsPtr->isPointLightEnabled) {
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "pointLight.position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "pointLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "pointLight.diffuse"), 1, glm::value_ptr(glm::vec3(2.0, 2.0, 2.0)));
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "pointLight.specular"), 1, glm::value_ptr(glm::vec3(2.0, 2.0, 2.0)));
		glProgramUniform1f(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "pointLight.constant"), 1.0f);
		glProgramUniform1f(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "pointLight.linear"), 0.06f);
		glProgramUniform1f(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "pointLight.quadratic"), 0.02f);
	}

	glm::vec3 spotLightPosition(0.0f, 2.0f, 0.0f);    // Posi��o da luz (acima da mesa)
	glm::vec3 spotLightDirection(0.0f, -1.0f, 0.0f);  // Dire��o da luz (para baixo)
	if (lightsPtr->isSpotLightEnabled) {


		// Define a posi��o da luz spot
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.position"), 1, glm::value_ptr(spotLightPosition));

		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.direction"), 1, glm::value_ptr(glm::vec3(1.0, -0.5, 0.0)));
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glProgramUniform1f(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.constant"), 1.0f);
		glProgramUniform1f(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.linear"), 0.09f);
		glProgramUniform1f(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.quadratic"), 0.032f);
		glProgramUniform1f(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.spotCutoff"), glm::cos(glm::radians(25.0f)));
		glProgramUniform1f(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.spotExponent"), 3.0f);

		// Define a dire��o da luz spot
		glProgramUniform3fv(tableProgram, glGetProgramResourceLocation(tableProgram, GL_UNIFORM, "spotLight.spotDirection"), 1, glm::value_ptr(spotLightDirection));
	}

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
}

