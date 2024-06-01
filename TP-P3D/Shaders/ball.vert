#version 440 core

layout(location = 0) in vec3 aPosition; // Posi��o do v�rtice
layout(location = 1) in vec3 aNormal;  // Normal do v�rtice
layout(location = 2) in vec2 aTexCoord; // Coordenada de textura do v�rtice

out vec3 vPositionEyeSpace; // Posi��o do v�rtice no espa�o da c�mera
out vec3 vNormalEyeSpace;  // Normal do v�rtice no espa�o da c�mera
out vec2 textureCoord;    // Coordenada de textura do v�rtice

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
  // Calcula a posi��o do v�rtice no espa�o da c�mera
  vec4 positionEyeSpace = View * Model * vec4(aPosition, 1.0);
  vPositionEyeSpace = positionEyeSpace.xyz;

  // Calcula a normal do v�rtice no espa�o da c�mera
  vNormalEyeSpace = normalize(mat3(transpose(inverse(View * Model))) * aNormal);

  // Passa a coordenada de textura para o fragment shader
  textureCoord = aTexCoord;

  // Calcula a posi��o do v�rtice no espa�o de proje��o
  gl_Position = Projection * positionEyeSpace;
}
