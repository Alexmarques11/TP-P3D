#version 330 core //versao do glsl a usar
layout (location = 0) in vec3 aPos; //o layout ajuda o opengl com o veretx data que recebe , neste caso no layout 0 existe um vector para posicao
layout (location = 1) in vec3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 vs_normal;
out vec3 vs_position;

void main()
{
  vs_position = vec4(model * vec4(aPos, 1.0f)).xyz;
  vs_normal = normalize(mat3(model) * normal);
  gl_Position = proj * view * model * vec4(aPos, 1.0);

};