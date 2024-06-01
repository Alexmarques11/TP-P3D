#ifndef LOAD_SHADERS_H
#define LOAD_SHADERS_H

#include <GL\glew.h>

/*****************************************************************************
				 GLuint LoadShaders(ShaderInfo*);

Descrição:
----------
Recebe um array de estruturas ShaderInfo.
Cada estrutura contém:
- tipo de shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.)
- apontador para uma C-string, que contém o nome do ficheiro com código do shader
- valor que referencia o objeto shader criado

O array de estruturas deverá terminar com o valor GL_NONE no campo 'type'.
Exemplo:
ShaderInfo  shaders[] = {
  { GL_VERTEX_SHADER, "triangles.vert" },
  { GL_FRAGMENT_SHADER, "triangles.frag" },
  { GL_NONE, NULL }
};

Retorno:
--------
Em caso de sucesso, a função retorna o valor que referencia o objeto programa.
Em caso de erro, será retornado o valor zero (0).

*****************************************************************************/

// Descomentar para debug
#define _DEBUG

typedef struct {
	GLenum     type;
	const char* filename;
	GLuint     shader;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo*);

#endif // LOAD_SHADERS_H
