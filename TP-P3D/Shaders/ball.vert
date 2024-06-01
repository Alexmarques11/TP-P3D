#version 440 core

layout(location = 0) in vec3 aPosition; // Posição do vértice
layout(location = 1) in vec3 aNormal;  // Normal do vértice
layout(location = 2) in vec2 aTexCoord; // Coordenada de textura do vértice

out vec3 vPositionEyeSpace; // Posição do vértice no espaço da câmera
out vec3 vNormalEyeSpace;  // Normal do vértice no espaço da câmera
out vec2 textureCoord;    // Coordenada de textura do vértice

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
  // Calcula a posição do vértice no espaço da câmera
  vec4 positionEyeSpace = View * Model * vec4(aPosition, 1.0);
  vPositionEyeSpace = positionEyeSpace.xyz;

  // Calcula a normal do vértice no espaço da câmera
  vNormalEyeSpace = normalize(mat3(transpose(inverse(View * Model))) * aNormal);

  // Passa a coordenada de textura para o fragment shader
  textureCoord = aTexCoord;

  // Calcula a posição do vértice no espaço de projeção
  gl_Position = Projection * positionEyeSpace;
}
