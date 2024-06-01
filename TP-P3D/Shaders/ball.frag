#version 440 core

in vec3 vPositionEyeSpace;
in vec3 vNormalEyeSpace;
in vec2 textureCoord;

uniform mat4 Model;
uniform mat4 View;
uniform sampler2D TexSampler;

struct AmbientLight {
  vec3 ambient;
};

struct DirectionalLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
  float spotCutoff;
  float spotExponent;
  vec3 spotDirection;
};

struct Material {
  vec3 emissive;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform AmbientLight ambientLight;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight[2];
uniform SpotLight spotLight;
uniform Material material;

uniform bool ambientLightEnabled;
uniform bool directionalLightEnabled;
uniform bool pointLightEnabled[2];
uniform bool spotLightEnabled;

layout (location = 0) out vec4 fColor;

vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient);
vec4 calcPointLight(PointLight light, out vec4 ambient);
vec4 calcSpotLight(SpotLight light, vec3 viewDir, vec3 normal, vec3 fragPos, out vec4 ambientOut);


vec3 diffuseColor;

void main() {
  diffuseColor = texture(TexSampler, textureCoord).rgb;

  vec4 emissive = vec4(material.emissive, 1.0);
  vec4 ambient = vec4(0.0);

  vec4 light[4];
  vec4 ambientTmp;

  if (ambientLightEnabled) {
    ambient += calcAmbientLight(ambientLight);
  }

  if (directionalLightEnabled) {
    light[0] = calcDirectionalLight(directionalLight, ambientTmp);
    ambient += ambientTmp;
  } else {
    light[0] = vec4(0.0);
  }

  for (int i = 0; i < 2; ++i) {
    if (pointLightEnabled[i]) {
      light[i+1] = calcPointLight(pointLight[i], ambientTmp);
      ambient += ambientTmp;
    } else {
      light[i+1] = vec4(0.0);
    }
  }

  if (spotLightEnabled) {
  light[3] = calcSpotLight(spotLight, normalize(-vPositionEyeSpace), normalize(vNormalEyeSpace), vPositionEyeSpace, ambientTmp);
  ambient += ambientTmp;
} else {
  light[3] = vec4(0.0);
}


  fColor = emissive + (ambient / 2.0) + light[0] + light[1] + light[2] + light[3];
}

vec4 calcAmbientLight(AmbientLight light) {
  return vec4(diffuseColor * light.ambient, 1.0);
}

vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient) {
  ambient = vec4(material.ambient * light.ambient, 1.0);

  vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
  vec3 L = normalize(-lightDirectionEyeSpace);
  vec3 N = normalize(vNormalEyeSpace);
  float NdotL = max(dot(N, L), 0.0);
  vec4 diffuse = vec4(diffuseColor * light.diffuse, 1.0) * NdotL;

  vec3 V = normalize(-vPositionEyeSpace);
  vec3 R = reflect(-L, N);
  float RdotV = max(dot(R, V), 0.0);
  vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

  return (diffuse + specular);
}

vec4 calcPointLight(PointLight light, out vec4 ambientOut) {
  // Cálculo da reflexão da componente da luz ambiente.
  ambientOut = vec4(material.ambient * light.ambient, 1.0);

  // Cálculo do vetor de direção da luz.
  vec3 lightDirection = normalize(light.position - vPositionEyeSpace);

  // Cálculo da reflexão da componente da luz difusa.
  float NdotL = max(dot(vNormalEyeSpace, lightDirection), 0.0);
  vec4 diffuse = vec4(light.diffuse * NdotL, 1.0);

  // Cálculo da reflexão da componente da luz especular.
  vec3 viewDirection = normalize(-vPositionEyeSpace);
  vec3 reflectDirection = reflect(-lightDirection, vNormalEyeSpace);
  float RdotV = max(dot(reflectDirection, viewDirection), 0.0);
  vec4 specular = vec4(light.specular * pow(RdotV, material.shininess), 1.0);

  // Attenuation
  float distance = length(light.position - vPositionEyeSpace);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  // Cálculo da contribuição da fonte de luz luz pontual para a cor final do fragmento.
  vec4 lightColor = attenuation * (diffuse + specular);
  return lightColor + ambientOut; // Adicionamos a componente da luz ambiente.

  // Retornar a cor difusa da textura multiplicada pela luz difusa
  return texture(TexSampler, textureCoord) * lightColor;
}



vec4 calcSpotLight(SpotLight light, vec3 viewDir, vec3 normal, vec3 fragPos, out vec4 ambientOut) {
  // Cálculo da reflexão da componente da luz ambiente.
   ambientOut = vec4(material.ambient * light.ambient, 1.0);

  // Cálculo do vetor de direção da luz.
  vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
  vec3 L = normalize(lightPositionEyeSpace - fragPos);

  // Cálculo do ângulo entre o vetor de direção da luz (L) e o vetor de direção da spot light (D).
  float spotEffect = dot(normalize(light.spotDirection), -L);

  // Comparação com o ângulo de abertura da spot light.
  if (spotEffect > cos(light.spotCutoff)) {
    // Dentro do cone de luz da spot light.
    // Cálculo da reflexão da componente da luz difusa.
    float NdotL = max(dot(normal, L), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * NdotL * vec4(light.ambient, 1.0);

    // Cálculo da reflexão da componente da luz especular.
    vec3 V = normalize(viewDir);
    vec3 R = reflect(-L, normal);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = vec4(light.specular, 1.0) * pow(RdotV, material.shininess) * vec4(light.ambient, 1.0);

    // attenuation
    float dist = length(lightPositionEyeSpace - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    // Cálculo da contribuição da fonte de luz luz pontual para a cor final do fragmento.
    return attenuation * (diffuse + specular);
  } else {
    // Fora do cone de luz da spot light, retorna cor preta.
    return vec4(0.0);
  }
}

