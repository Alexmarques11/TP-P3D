#version 440 core

layout (location = 0) in vec3 position;  // Posi��o do v�rtice
layout (location = 1) in vec3 normal;   // Normal do v�rtice
layout (location = 2) in vec2 texCoord;  // Coordenada de textura do v�rtice

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 vs_normal;    // Normal para o fragment shader
out vec3 vs_position;   // Posi��o para o fragment shader
out vec2 textureCoord;   // Coordenada de textura para o fragment shader

void main()
{
  gl_Position = proj * view * model * vec4(position, 1.0);
  vs_normal = mat3(transpose(inverse(model))) * normal;
  vs_position = vec3(view * model * vec4(position, 1.0));
  textureCoord = texCoord;
}
