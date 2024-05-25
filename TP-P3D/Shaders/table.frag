#version 440 core

out vec4 FragColor; // outputs vec4

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj; // View * Model

uniform bool ambientLightEnabled;
uniform bool directionalLightEnabled;
uniform bool pointLightEnabled[2]; // Duas fontes de luz pontual
uniform bool spotLightEnabled;

in vec3 vs_normal;
in vec3 vs_position;

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
    vec3 ambient; // Componente de luz ambiente global
};

uniform AmbientLight ambientLight; // Fonte de luz ambiente global

// Estrutura de uma fonte de luz direcional
struct DirectionalLight {
    vec3 direction; // Direção da luz, espaço do mundo
    vec3 ambient; // Componente de luz ambiente
    vec3 diffuse; // Componente de luz difusa
    vec3 specular; // Componente de luz especular
};

uniform DirectionalLight directionalLight; // Fonte de luz direcional

// Estrutura de uma fonte de luz pontual
struct PointLight {
    vec3 position; // Posição do ponto de luz, espaço do mundo
    vec3 ambient; // Componente de luz ambiente
    vec3 diffuse; // Componente de luz difusa
    vec3 specular; // Componente de luz especular
    float constant; // Coeficiente de atenuação constante
    float linear; // Coeficiente de atenuação linear
    float quadratic; // Coeficiente de atenuação quadrática
};

uniform PointLight pointLight[2]; // Duas fontes de luz pontual

// Estrutura de uma fonte de luz cónica
struct SpotLight {
    vec3 position; // Posição do foco de luz, espaço do mundo
    vec3 ambient; // Componente de luz ambiente
    vec3 diffuse; // Componente de luz difusa
    vec3 specular; // Componente de luz especular
    float constant; // Coeficiente de atenuação constante
    float linear; // Coeficiente de atenuação linear
    float quadratic; // Coeficiente de atenuação quadrática
    float spotCutoff; // Ângulo de corte do foco de luz
    float spotExponent; // Expoente do foco de luz
    vec3 spotDirection; // Direção do foco de luz
};

uniform SpotLight spotLight; // Fonte de luz cónica

vec4 calcAmbientLight(AmbientLight light) {
    // Cálculo da contribuição da fonte de luz ambiente global, para a cor do objeto.
    return vec4(light.ambient, 1.0);
}

vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambientOut) {
    // Cálculo da reflexão da componente da luz ambiente.
    ambientOut = vec4(light.ambient, 1.0);

    // Cálculo da reflexão da componente da luz difusa.
    vec3 lightDirectionEyeSpace = (view * vec4(light.direction, 0.0)).xyz;
    vec3 L = normalize(-lightDirectionEyeSpace); // Direção inversa à da direção luz.
    vec3 N = normalize(vs_normal);
    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * NdotL;

    // Cálculo da reflexão da componente da luz especular.
    vec3 V = normalize(-vs_position);
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = vec4(light.specular, 1.0) * pow(RdotV, 32.0); // Valor fixo para o expoente especular

    // Cálculo da contribuição da fonte de luz direcional para a cor final do fragmento.
    return (diffuse + specular);
}

vec4 calcPointLight(PointLight light, out vec4 ambientOut) {
    // Cálculo da reflexão da componente da luz ambiente.
    ambientOut = vec4(light.ambient, 1.0);

    // Cálculo da reflexão da componente da luz difusa.
    vec3 lightPositionEyeSpace = (view * vec4(light.position, 1.0)).xyz;
    vec3 L = normalize(lightPositionEyeSpace - vs_position);
    vec3 N = normalize(vs_normal);
    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * NdotL;

    // Cálculo da reflexão da componente da luz especular.
    vec3 V = normalize(-vs_position);
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = vec4(light.specular, 1.0) * pow(RdotV, 32.0); // Valor fixo para o expoente especular

    // Atenuação
    float dist = length(mat3(view) * light.position - vs_position); // Cálculo da distância entre o ponto de luz e o vértice
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    // Cálculo da contribuição da fonte de luz pontual para a cor final do fragmento.
    return (attenuation * (diffuse + specular));
}

vec4 calcSpotLight(SpotLight light, out vec4 ambientOut) {
    // Cálculo da reflexão da componente da luz ambiente.
    ambientOut = vec4(light.ambient, 1.0);

    // Cálculo da reflexão da componente da luz difusa.
    vec3 lightPositionEyeSpace = (view * vec4(light.position, 1.0)).xyz;
    vec3 L = normalize(lightPositionEyeSpace - vs_position);
    vec3 N = normalize(vs_normal);
    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * NdotL;

    // Cálculo da reflexão da componente da luz especular.
    vec3 V = normalize(-vs_position);
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = vec4(light.specular, 1.0) * pow(RdotV, 32.0); // Valor fixo para o expoente especular

    // Atenuação
    float dist = length(mat3(view) * light.position - vs_position); // Cálculo da distância entre o ponto de luz e o vértice
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    // Cálculo do ângulo entre o vetor direção do foco e o vetor L
    float spotEffect = dot(normalize(light.spotDirection), -L);

    // Verifica se está dentro do cone do foco de luz
    if (spotEffect > cos(light.spotCutoff)) {
        float spotFactor = pow(spotEffect, light.spotExponent);
        return (attenuation * (diffuse + specular)) * spotFactor;
    } else {
        return vec4(0, 0, 0, 1.0);
    }
}

void main() {
    FragColor = vec4(0.0f, 0.4f, 0.0f, 1.0f);

    vec4 ambient;
    vec4 light[4];
    vec4 ambientTmp;

    ambient = vec4(0.0);
    light[0] = vec4(0.0);
    light[1] = vec4(0.0);
    light[2] = vec4(0.0);
    light[3] = vec4(0.0);

    // Se a luz ambiente estiver ativada, adiciona sua contribuição
    if (ambientLightEnabled) {
        ambient += calcAmbientLight(ambientLight);
    }

    // Adiciona a contribuição da luz direcional, se estiver ativada
    if (directionalLightEnabled) {
        light[0] = calcDirectionalLight(directionalLight, ambientTmp);
        ambient += ambientTmp;
    }

    // Adiciona a contribuição das luzes pontuais, se estiverem ativadas
    for (int i = 0; i < 2; ++i) {
        if (pointLightEnabled[i]) {
            light[i + 1] = calcPointLight(pointLight[i], ambientTmp);
            ambient += ambientTmp;
        }
    }

    // Adiciona a contribuição da luz cônica, se estiver ativada
    if (spotLightEnabled) {
        light[3] = calcSpotLight(spotLight, ambientTmp);
        ambient += ambientTmp;
    }

    FragColor += (ambient / 4.0) + light[0] + light[1] + light[2] + light[3];
}
