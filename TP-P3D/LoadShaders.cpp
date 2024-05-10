#pragma once

#include <iostream>
#include <fstream>

#define GLEW_STATIC
#include <GL\glew.h>

#include "LoadShaders.h"

static const GLchar* ReadShader(const char* filename) {
	// Abre o ficheiro 'filename' em bin�rio, e coloca-se na �ltima posi��o do ficheiro.
	std::ifstream ficheiro(filename, std::ifstream::ate | std::ifstream::binary);
	// Se o ficheiro foi aberto.
	if (ficheiro.is_open()) {
		// Leitura da pr�xima posi��o de leitura.
		std::streampos tamanhoDoFicheiroEmBytes = ficheiro.tellg();
		// Reposiciona a leitura do ficheiro no seu in�cio.
		ficheiro.seekg(0, std::ios::beg);

		// Aloca��o de espa�o de mem�ria para dados do ficheiro.
		GLchar* source = new GLchar[int(tamanhoDoFicheiroEmBytes) + 1];
		// Leitura do ficheiro para o array 'source'.
		ficheiro.read(source, tamanhoDoFicheiroEmBytes);
		// Fecha a string.
		source[tamanhoDoFicheiroEmBytes] = 0;

		// Fecha o ficheiro.
		ficheiro.close();

		// Retorna o endere�o da string alocada.
		return const_cast<const GLchar*>(source);
	}
	else {
		std::cerr << "Erro ao abrir o ficheiro '" << filename << "'" << std::endl;
	}

	return nullptr;
}

