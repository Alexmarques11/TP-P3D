#version 440 core

out vec4 FragColor; // Saída da cor do fragmento

uniform bool ambientLightEnabled;
uniform bool directionalLightEnabled;
uniform bool pointLightEnabled; // Apenas uma fonte de luz pontual
uniform bool spotLightEnabled;

// Cor fixa da mesa
const vec3 mesaColor = vec3(0.0, 0.4, 0.0); // Cor verde

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

uniform PointLight pointLight; // Apenas uma fonte de luz pontual

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

// Função para calcular a contribuição da luz ambiente
vec4 calcAmbientLight(AmbientLight light) {
    return vec4(mesaColor * light.ambient, 1.0);
}

// Função para calcular a contribuição da luz direcional
vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambientOut) {
    ambientOut = vec4(light.ambient, 1.0) * vec4(mesaColor, 1.0);

    vec3 L = normalize(-light.direction); // Direção inversa à da direção luz
    float NdotL = max(dot(vec3(0, 1, 0), L), 0.0); // Assumindo que a normal da mesa é (0, 1, 0)
    vec4 diffuse = vec4(light.diffuse, 1.0) * NdotL * vec4(mesaColor, 1.0);

    vec3 V = normalize(vec3(0, 0, 1)); // Vetor de visualização apontando para a câmera
    vec3 R = reflect(-L, vec3(0, 1, 0)); // Vetor de reflexão
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = vec4(light.specular, 1.0) * pow(RdotV, 32.0); // Valor fixo para o expoente especular

    return (diffuse + specular);
}

// Função para calcular a contribuição da luz pontual
vec4 calcPointLight(PointLight light, out vec4 ambientOut) {
    ambientOut = vec4(light.ambient, 1.0) * vec4(mesaColor, 1.0);

    vec3 lightDir = normalize(light.position - vec3(0, 0, 0)); // Vetor direção da luz
    float distance = length(light.position - vec3(0, 0, 0)); // Distância entre a posição da luz e a posição do fragmento

    // Vetor normal da superfície (assumindo que a normal da mesa é sempre apontando para cima)
    vec3 normal = vec3(0, 1, 0);

    // Vetor de visualização apontando para a câmera
    vec3 viewDir = normalize(vec3(0, 0, 1));

    // Vetor de reflexão
    vec3 reflectDir = reflect(-lightDir, normal);

    // Cálculo da luz difusa
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * mesaColor);

    // Cálculo da luz especular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = light.specular * (spec * mesaColor);

    // Cálculo da atenuação
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Atenuação da luz
    diffuse *= attenuation;
    specular *= attenuation;

    return vec4(diffuse + specular, 1.0);
}


// Função para calcular a contribuição da luz cónica
vec4 calcSpotLight(SpotLight light, out vec4 ambientOut) {
    // A implementação desta função é semelhante à função de luz pontual,
    // mas com a adição do cálculo da atenuação do ângulo do cone de luz
    // em relação à direção da luz.
    // Você pode implementar essa função seguindo um padrão semelhante
    // ao da função calcPointLight, considerando a direção do foco de luz
    // e o cálculo da atenuação baseada no ângulo de corte do foco de luz.
    // Se precisar de ajuda adicional com essa função, estou aqui para ajudar!
    return vec4(0.0);
}

void main() {
    FragColor = vec4(0.0f, 0.4f, 0.0f, 1.0f); // Cor inicial da mesa

    vec4 ambient;
    vec4 light[3];
    vec4 ambientTmp;

    ambient = vec4(0.0);
    light[0] = vec4(0.0);
    light[1] = vec4(0.0);
    light[2] = vec4(0.0);

    // Se a luz ambiente
    if (ambientLightEnabled) {
        ambient += calcAmbientLight(ambientLight);
    }

    // Adiciona a contribuição da luz direcional, se estiver ativada
    if (directionalLightEnabled) {
        light[0] = calcDirectionalLight(directionalLight, ambientTmp);
        ambient += ambientTmp;
    }

    // Adiciona a contribuição da luz pontual, se estiver ativada
    if (pointLightEnabled) {
        light[1] = calcPointLight(pointLight, ambientTmp);
        ambient += ambientTmp;
    }

    // Adiciona a contribuição da luz cônica, se estiver ativada
    if (spotLightEnabled) {
        light[2] = calcSpotLight(spotLight, ambientTmp);
        ambient += ambientTmp;
    }

    FragColor = ambient + light[0] + light[1] + light[2];
}