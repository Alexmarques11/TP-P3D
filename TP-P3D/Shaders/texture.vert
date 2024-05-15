#version 440 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoords; // Adicionando entrada para coordenadas de textura

layout(location = 0) uniform mat4 MVP;

out vec2 TexCoords; // Coordenadas de textura passadas para o fragment shader

void main()
{ 
    gl_Position = MVP * vec4(vPosition, 1.0f);
    TexCoords = vTexCoords; // Passando coordenadas de textura
}
