/*****************************************************************************
 * LoadShaders.cpp
 *
 * Descrição:
 * ----------
 * Este arquivo contém a implementação das funções `ReadShader` e `LoadShaders`, que são responsáveis por:
 * - Ler o código fonte dos shaders a partir de arquivos.
 * - Compilar os shaders.
 * - Criar e linkar um programa de shader, que combina os shaders de vértice e fragmento.
 * - Retornar um identificador para o programa de shader criado.
 *
 * Funções principais:
 * - ReadShader(const char* filename): Lê o código fonte de um shader a partir de um arquivo.
 * - LoadShaders(ShaderInfo* shaders): Carrega, compila e linka os shaders, cria um programa de shader.
 *
 * Estruturas de dados importantes:
 * - ShaderInfo: Estrutura que armazena informações sobre um shader (tipo, nome do arquivo e identificador do shader).
 *
 * Variáveis e constantes importantes:
 * - _DEBUG: Macro que ativa o modo de depuração, para exibir mensagens de erro detalhadas.
 *
 ******************************************************************************/


#pragma once

#include <iostream>
#include <fstream>

#define GLEW_STATIC
#include <GL\glew.h>
#include "LoadShaders.h"


 /*****************************************************************************
 * static const GLchar* ReadShader(const char* filename)
 *
 * Descrição:
 * ----------
 * Esta função estática lê o conteúdo de um arquivo de shader (código GLSL) e o
 * retorna como uma string (const GLchar*). Ela é utilizada pela função `LoadShaders`
 * para obter o código-fonte dos shaders a serem compilados e vinculados.
 *
 * Parâmetros:
 * -----------
 * - filename: O nome do arquivo de shader a ser lido (const char*).
 *
 * Retorno:
 * --------
 * - const GLchar*: Um apontador constante para a string que contém o conteúdo do arquivo
 *  de shader, ou `nullptr` se ocorrer um erro na leitura do arquivo.
 *
 ******************************************************************************/
static const GLchar* ReadShader(const char* filename) {
	std::ifstream ficheiro(filename, std::ifstream::ate | std::ifstream::binary);
	if (ficheiro.is_open()) {
		std::streampos tamanhoDoFicheiroEmBytes = ficheiro.tellg();
		ficheiro.seekg(0, std::ios::beg);

		GLchar* source = new GLchar[int(tamanhoDoFicheiroEmBytes) + 1];
		ficheiro.read(source, tamanhoDoFicheiroEmBytes);
		source[tamanhoDoFicheiroEmBytes] = 0;

		ficheiro.close();

		return const_cast<const GLchar*>(source);
	}
	else {
		std::cerr << "Erro ao abrir o ficheiro '" << filename << "'" << std::endl;
	}

	return nullptr;
}


/*****************************************************************************
 * GLuint LoadShaders(ShaderInfo* shaders)
 *
 * Descrição:
 * ----------
 * Esta função carrega, compila e vincula shaders GLSL (OpenGL Shading Language), cria
 * um programa de shader que pode ser usado para renderizar objetos na cena.
 * Ela recebe um array de estruturas `ShaderInfo`, onde cada estrutura descreve
 * um shader (tipo, nome do arquivo).
 *
 * Parâmetros:
 * -----------
 * - shaders: Um array de estruturas `ShaderInfo` que descrevem os shaders a serem carregados
 *  e compilados.
 *
 * Retorno:
 * --------
 * - GLuint: O identificador do programa de shader criado e vinculado com sucesso, ou 0 em caso de erro.
 *
 ******************************************************************************/
GLuint LoadShaders(ShaderInfo* shaders) {
	if (shaders == nullptr) return 0;

	GLuint program = glCreateProgram();

	for (GLint i = 0; shaders[i].type != GL_NONE; i++) {
		shaders[i].shader = glCreateShader(shaders[i].type);

		const GLchar* source = ReadShader(shaders[i].filename);
		if (source == NULL) {
			for (int j = 0; shaders[j].type != GL_NONE; j++) {
				if (shaders[j].shader != 0)
					glDeleteShader(shaders[j].shader);
				shaders[j].shader = 0;
			}

			return 0;
		}

		glShaderSource(shaders[i].shader, 1, &source, NULL);
		delete[] source;

		glCompileShader(shaders[i].shader);

		GLint compiled;
		glGetShaderiv(shaders[i].shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
#ifdef _DEBUG
			GLsizei len;
			glGetShaderiv(shaders[i].shader, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(shaders[i].shader, len, &len, log);
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete[] log;
#endif /* DEBUG */

			for (int j = 0; shaders[j].type != GL_NONE; j++) {
				if (shaders[j].shader != 0)
					glDeleteShader(shaders[j].shader);
				shaders[j].shader = 0;
			}

			return 0;
		}

		glAttachShader(program, shaders[i].shader);
	}

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
#ifdef _DEBUG
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		std::cerr << "Shader linking failed: " << log << std::endl;
		delete[] log;
#endif /* DEBUG */

		for (int j = 0; shaders[j].type != GL_NONE; j++) {
			if (shaders[j].shader != 0)
				glDeleteShader(shaders[j].shader);
			shaders[j].shader = 0;
		}

		return 0;
	}

	return program;
}
