#version 440 core

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform Material material;
uniform sampler2D diffuseTexture;

void main()
{
    vec3 ambient = material.ambient * texture(diffuseTexture, TexCoord).rgb;
    vec3 diffuse = material.diffuse * texture(diffuseTexture, TexCoord).rgb;
    vec3 specular = material.specular * texture(diffuseTexture, TexCoord).rgb;

    vec3 ambientResult = ambient;
    vec3 diffuseResult = diffuse * max(dot(normalize(Normal), normalize(-FragPos)), 0.0);
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-viewDir, normalize(Normal));
    float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularResult = specular * specularIntensity;

    FragColor = vec4(ambientResult + diffuseResult + specularResult, 1.0);
}
