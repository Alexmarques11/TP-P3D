#version 440 core

in vec3 vPositionEyeSpace;
in vec3 vNormalEyeSpace;
in vec2 textureCoord;  // Coordenada de textura do v�rtice

uniform mat4 Model;
uniform mat4 View;
uniform sampler2D TexSampler;  // Sampler de textura

struct AmbientLight {
    vec3 ambient;  // Componente de luz ambiente global
};

struct DirectionalLight {
    vec3 direction;  // Dire��o da luz, espa�o do mundo
    vec3 ambient;    // Componente de luz ambiente
    vec3 diffuse;    // Componente de luz difusa
    vec3 specular;   // Componente de luz especular
};

struct PointLight {
    vec3 position;   // Posi��o do ponto de luz, espa�o do mundo
    vec3 ambient;    // Componente de luz ambiente
    vec3 diffuse;    // Componente de luz difusa
    vec3 specular;   // Componente de luz especular
    float constant;  // Coeficiente de atenua��o constante
    float linear;    // Coeficiente de atenua��o linear
    float quadratic; // Coeficiente de atenua��o quadr�tica
};

struct SpotLight {
    vec3 position;   // Posi��o do foco de luz, espa�o do mundo
    vec3 ambient;    // Componente de luz ambiente
    vec3 diffuse;    // Componente de luz difusa
    vec3 specular;   // Componente de luz especular
    float constant;  // Coeficiente de atenua��o constante
    float linear;    // Coeficiente de atenua��o linear
    float quadratic; // Coeficiente de atenua��o quadr�tica
    float spotCutoff, spotExponent;
    vec3 spotDirection;
};

struct Material {
    vec3 emissive;
    vec3 ambient;   // Ka
    vec3 specular;  // Ke
    float shininess;
};

uniform AmbientLight ambientLight; // Fonte de luz ambiente global
uniform DirectionalLight directionalLight; // Fonte de luz direcional
uniform PointLight pointLight[2]; // Duas fontes de luz pontual
uniform SpotLight spotLight; // Fonte de luz c�nica
uniform Material material;

uniform bool ambientLightEnabled;
uniform bool directionalLightEnabled;
uniform bool pointLightEnabled[2]; // Duas fontes de luz pontual
uniform bool spotLightEnabled;

layout (location = 0) out vec4 fColor; // Cor final do fragmento

vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient);
vec4 calcPointLight(PointLight light, out vec4 ambient);
vec4 calcSpotLight(SpotLight light, out vec4 ambient);

vec3 diffuseColor;

void main() {
    // Cor do Material
    diffuseColor = texture(TexSampler, textureCoord).rgb;

    // C�lculo da componente emissiva do material.
    vec4 emissive = vec4(material.emissive, 1.0);

    // Luz Ambiente Global
    vec4 ambient = vec4(0.0);

    // C�lculo do efeito da ilumina��o no fragmento.
    vec4 light[4];
    vec4 ambientTmp;

    // Contribui��o da fonte de luz ambiente
    if (ambientLightEnabled) {
        ambient += calcAmbientLight(ambientLight);
    }

    // Contribui��o da fonte de luz direcional
    if (directionalLightEnabled) {
        light[0] = calcDirectionalLight(directionalLight, ambientTmp);
        ambient += ambientTmp;
    } else {
        light[0] = vec4(0.0);
    }

    // Contribui��o de cada fonte de luz Pontual
    for (int i = 0; i < 2; ++i) {
        if (pointLightEnabled[i]) {
            light[i+1] = calcPointLight(pointLight[i], ambientTmp);
            ambient += ambientTmp;
        } else {
            light[i+1] = vec4(0.0);
        }
    }

    // Contribui��o da fonte de luz c�nica
    if (spotLightEnabled) {
        light[3] = calcSpotLight(spotLight, ambientTmp);
        ambient += ambientTmp;
    } else {
        light[3] = vec4(0.0);
    }

    // C�lculo da cor final do fragmento.
    fColor = emissive + (ambient / 2.0) + light[0] + light[1] + light[2] + light[3];
}

vec4 calcAmbientLight(AmbientLight light) {
    // C�lculo da contribui��o da fonte de luz ambiente global, para a cor do objeto.
    return vec4(diffuseColor * light.ambient, 1.0);
}

vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient) {
    // C�lculo da reflex�o da componente da luz ambiente.
    ambient = vec4(material.ambient * light.ambient, 1.0);

    // C�lculo da reflex�o da componente da luz difusa.
    vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
    vec3 L = normalize(-lightDirectionEyeSpace); // Dire��o inversa � da dire��o luz.
    vec3 N = normalize(vNormalEyeSpace);
    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(diffuseColor * light.diffuse, 1.0) * NdotL;

    // C�lculo da reflex�o da componente da luz especular.
    vec3 V = normalize(-vPositionEyeSpace);
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

    // C�lculo da contribui��o da fonte de luz direcional para a cor final do fragmento.
    return (diffuse + specular);
}

vec4 calcPointLight(PointLight light, out vec4 ambient) {
    // C�lculo da reflex�o da componente da luz ambiente.
    ambient = vec4(material.ambient * light.ambient, 1.0);

    // C�lculo da reflex�o da componente da luz difusa.
    vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
    vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
    vec3 N = normalize(vNormalEyeSpace);
    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(diffuseColor * light.diffuse, 1.0) * NdotL;

    // C�lculo da reflex�o da componente da luz especular.
    vec3 V = normalize(-vPositionEyeSpace);
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

    // Atenua��o
    float dist = length(mat3(View) * light.position - vPositionEyeSpace); // C�lculo da dist�ncia entre o ponto de luz e o v�rtice
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    // C�lculo da contribui��o da fonte de luz pontual para a cor final do fragmento.
    return (attenuation * (diffuse + specular));
}

vec4 calcSpotLight(SpotLight light, out vec4 ambient) {
    // C�lculo da reflex�o da componente da luz ambiente.
    ambient = vec4(light.ambient, 1.0);

    // C�lculo da reflex�o da componente da luz difusa.
    vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
    vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
    vec3 N = normalize(vNormalEyeSpace);
    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * NdotL;

    // C�lculo da reflex�o da componente da luz especular.
    vec3 V = normalize(-vPositionEyeSpace);
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = vec4(light.specular, 1.0) * pow(RdotV, 32.0); // Valor fixo para o expoente especular

    // Atenua��o
    float dist = length(mat3(View) * light.position - vPositionEyeSpace); // C�lculo da dist�ncia entre o ponto de luz e o v�rtice
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    // C�lculo do �ngulo entre o vetor dire��o do foco e o vetor L
    float spotEffect = dot(normalize(light.spotDirection), -L);

    // Verifica se est� dentro do cone do foco de luz
    if (spotEffect > cos(light.spotCutoff)) {
        spotEffect = pow(spotEffect, light.spotExponent);
    } else {
        spotEffect = 0.0;
    }

    // C�lculo da contribui��o da fonte de luz c�nica para a cor final do fragmento.
    return attenuation * spotEffect * (diffuse + specular);
}
