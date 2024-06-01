#version 440 core

out vec4 FragColor; // Saída da cor do fragmento

in vec3 vs_normal;  // Normal interpolada do vértice
in vec3 vs_position; // Posição interpolada do vértice

// Uniforms para habilitar/desabilitar tipos de luz
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
  vec3 position; // Posição do luz pontual, espaço do mundo
  vec3 ambient; // Componente de luz ambiente
  vec3 diffuse; // Componente de luz difusa
  vec3 specular; // Componente de luz especular
  float constant; // Coeficiente de atenuação constante
  float linear; // Coeficiente de atenuação linear
  float quadratic; // Coeficiente de atenuação quadrática
};
uniform PointLight pointLight; // Apenas uma fonte de luz pontual

// Estrutura de uma fonte de luz cônica
struct SpotLight {
  vec3 position; // Posição do foco de luz, espaço do mundo
  vec3 ambient; // Componente de luz ambiente
  vec3 diffuse; // Componente de luz difusa
  vec3 specular; // Componente de luz especular
  float constant; // Coeficiente de atenuação constante
  float linear; // Coeficiente de atenuação linear
  float quadratic; // Coeficiente de atenuação quadrática
  float cutoff; // Ângulo de corte do foco de luz em radianos
  float exponent; // Expoente do foco de luz
  vec3 direction; // Direção do foco de luz
};
uniform SpotLight spotLight; // Fonte de luz cônica

// Estrutura do material
struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
uniform Material material;

// Uniform para a matriz de visualização
uniform mat4 view;

// Função para calcular a contribuição da luz ambiente
vec4 calcAmbientLight(AmbientLight light) {
  return vec4(mesaColor * light.ambient, 1.0);
}

// Função para calcular a contribuição da luz direcional
vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambientOut) {
  ambientOut = vec4(light.ambient, 1.0) * vec4(mesaColor, 1.0); // Cálculo da luz ambiente

  vec3 L = normalize(-light.direction); // Direção inversa à da luz
  float NdotL = max(dot(vs_normal, L), 0.0); // Produto escalar entre a normal e a direção da luz
  vec4 diffuse = vec4(light.diffuse, 1.0) * NdotL * vec4(mesaColor, 1.0); // Cálculo da luz difusa

  vec3 V = normalize(-vs_position); // Vetor de visualização apontando para a câmera
  vec3 R = reflect(-L, vs_normal); // Vetor de reflexão
  float RdotV = max(dot(R, V), 0.0); // Produto escalar entre a reflexão e o vetor de visualização
  vec4 specular = vec4(light.specular, 1.0) * pow(RdotV, material.shininess) * vec4(mesaColor, 1.0); // Cálculo da luz especular

  return (diffuse + specular);
}

// Função para calcular a contribuição da luz pontual
vec4 calcPointLight(PointLight light, out vec4 ambientOut) {
  ambientOut = vec4(light.ambient, 1.0) * vec4(mesaColor, 1.0);

  vec3 lightPositionEyeSpace = (view * vec4(light.position, 1.0)).xyz;
  vec3 L = normalize(lightPositionEyeSpace - vs_position);
  vec3 N = normalize(vs_normal);
  float NdotL = max(dot(N, L), 0.0);
  vec4 diffuse = vec4(light.diffuse, 1.0) * NdotL * vec4(mesaColor, 1.0);

  vec3 V = normalize(-vs_position);
  vec3 R = reflect(-L, N);
  float RdotV = max(dot(R, V), 0.0);
  vec4 specular = vec4(light.specular, 1.0) * pow(RdotV, material.shininess) * vec4(mesaColor, 1.0);

  float dist = length(lightPositionEyeSpace - vs_position);
  float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

  return attenuation * (diffuse + specular);
}

// Função para calcular a contribuição da luz cônica
vec4 calcSpotLight(SpotLight light, out vec4 ambientOut) {
  ambientOut = vec4(light.ambient, 1.0) * vec4(mesaColor, 1.0);

  vec3 lightPositionEyeSpace = (view * vec4(light.position, 1.0)).xyz;
  vec3 L = normalize(lightPositionEyeSpace - vs_position);

  float spotEffect = dot(normalize(light.direction), -L);

  if (spotEffect > cos(light.cutoff)) {
    vec3 N = normalize(vs_normal);
    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * NdotL * vec4(mesaColor, 1.0);

    vec3 V = normalize(-vs_position);
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = vec4(light.specular, 1.0) * pow(RdotV, material.shininess) * vec4(mesaColor, 1.0);

    float dist = length(lightPositionEyeSpace - vs_position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    return attenuation * (diffuse + specular);
  } else {
    return vec4(0.0);
  }
}

void main() {
  vec4 ambient = vec4(0.0);
  vec4 light[3] = {vec4(0.0), vec4(0.0), vec4(0.0)};
  vec4 ambientTmp;

  if (ambientLightEnabled) {
    ambient += calcAmbientLight(ambientLight);
  }

  if (directionalLightEnabled) {
    light[0] = calcDirectionalLight(directionalLight, ambientTmp);
    ambient += ambientTmp;
  }

  if (pointLightEnabled) {
    light[1] = calcPointLight(pointLight, ambientTmp);
    ambient += ambientTmp;
  }

  if (spotLightEnabled) {
    light[2] = calcSpotLight(spotLight, ambientTmp);
    ambient += ambientTmp;
  }

  FragColor = ambient + light[0] + light[1] + light[2];
}
