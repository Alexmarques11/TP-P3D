#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;		// View * Model
uniform mat4 Projection;
uniform mat3 NormalMatrix;

layout (location = 0) in vec3 vPosition;			// Coordenadas locais do vértice
layout(location = 1) in vec3 vNormal;			// Normal do vértice
layout(location = 2) in vec2 uv; // Add input variable for UVs


out vec3 vPositionEyeSpace;
out vec3 vNormalEyeSpace;
out vec2 textureVector;

void main()
{ 
	// Posição do vértice em coordenadas do olho.
	vPositionEyeSpace = (ModelView * vec4(vPosition, 1.0)).xyz;

	// Transformar a normal do vértice.
	vNormalEyeSpace = normalize(NormalMatrix * vNormal);

	// Coordenada de textura para o CubeMap
	textureVector = uv;

	// Posição final do vértice (em coordenadas de clip)
	gl_Position = Projection * ModelView * vec4(vPosition, 1.0f);
}