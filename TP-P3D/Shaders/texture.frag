#version 440 core

layout(location = 0) out vec4 fColor; // Cor final do fragmento

in vec2 TexCoords; // Coordenadas de textura passadas do vertex shader

uniform sampler2D diffuseTexture; // sampler2D para textura 2D

void main()
{
    fColor = texture(diffuseTexture, TexCoords);
}
