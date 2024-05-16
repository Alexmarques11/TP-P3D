#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;		// View * Model

uniform samplerCube cubeMap;

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
	vec3 ambient;	// Componente de luz ambiente global
};

uniform AmbientLight ambientLight; // Fonte de luz ambiente global

// Estrutura de uma fonte de luz direcional
struct DirectionalLight	{
	vec3 direction;		// Dire  o da luz, espa o do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
};

uniform DirectionalLight directionalLight; // Fonte de luz direcional

// Estrutura de uma fonte de luz pontual
struct PointLight	{
	vec3 position;		// Posi  o do ponto de luz, espa o do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenua  o constante
	float linear;		// Coeficiente de atenua  o linear
	float quadratic;	// Coeficiente de atenua  o quadr tica
};

uniform PointLight pointLight[2]; // Duas fontes de luz pontual

// Estrutura de uma fonte de luz c nica
struct SpotLight {
	vec3 position;		// Posi  o do foco de luz, espa o do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenua  o constante
	float linear;		// Coeficiente de atenua  o linear
	float quadratic;	// Coeficiente de atenua  o quadr tica

	float spotCutoff, spotExponent;
	vec3 spotDirection;
};

uniform SpotLight spotLight; // Fonte de luz c nica

struct Material{
	vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

uniform Material material;
vec3 diffuseColor;

in vec3 vPositionEyeSpace;
in vec3 vNormalEyeSpace;
in vec3 textureVector;

layout (location = 0) out vec4 fColor; // Cor final do fragmento

vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient);
vec4 calcPointLight(PointLight light, out vec4 ambient);
vec4 calcSpotLight(SpotLight light, out vec4 ambient);

void main()
{
	// Cor do Material
	// Se a textura n o for nula, ent o a cor do material   a cor da textura.
	//diffuseColor = texture(cubeMap, textureVector).rgb;
	// Sen o
	diffuseColor = material.diffuse;

	// C lculo da componente emissiva do material.
	vec4 emissive = vec4(material.emissive, 1.0);

	// Luz Ambiente Global
	vec4 ambient;

	// C lculo do efeito da ilumina  o no fragmento.
	vec4 light[4];
	vec4 ambientTmp;
	// Contribui  o da fonte de luz ambiente
	ambient = calcAmbientLight(ambientLight);
	// Contribui  o da fonte de luz direcional
	light[0] = calcDirectionalLight(directionalLight, ambientTmp);
	ambient += ambientTmp;
	// Contribui  o de cada fonte de luz Pontual
	for(int i=0; i<2; i++) {
		light[i+1] = calcPointLight(pointLight[i], ambientTmp);
		ambient += ambientTmp;
	}
	// Contribui  o da fonte de luz c nica
	light[3] = calcSpotLight(spotLight, ambientTmp);
	ambient += ambientTmp;

	// C lculo da cor final do fragmento.
	// Com CubeMap
	//fColor = emissive + (ambient/4) + light[0] + light[1] + light[2] + light[3];
	// Com cor de fragmento
	fColor = emissive + (ambient/4) + light[0] + light[1] + light[2] + light[3];
}

vec4 calcAmbientLight(AmbientLight light) {
	// C lculo da contribui  o da fonte de luz ambiente global, para a cor do objeto.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient) {
	// C lculo da reflex o da componente da luz ambiente.
	ambient = vec4(material.ambient * light.ambient, 1.0);

	// C lculo da reflex o da componente da luz difusa.
	vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace); // Dire  o inversa   da dire  o luz.
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(diffuseColor * light.diffuse, 1.0) * NdotL;
	
	// C lculo da reflex o da componente da luz especular.
	// Como os c lculos est o a ser realizados nas coordenadas do olho, ent o a c mara est  na posi  o (0,0,0).
	// Resulta ent o um vetor V entre os pontos (0,0,0) e vPositionEyeSpace:
	//		V = (0,0,0) - vPositionEyeSpace = (0-vPositionEyeSpace.x, 0-vPositionEyeSpace.y, 0-vPositionEyeSpace.z)
	// Que pode ser simplificado como:
	//		- vPositionEyeSpace
	vec3 V = normalize(-vPositionEyeSpace);
	//vec4 H = normalize(L + V);	// Modelo Blinn-Phong
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	// Modelo Blinn-Phong
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	// C lculo da contribui  o da fonte de luz direcional para a cor final do fragmento.
	return (diffuse + specular);
}

vec4 calcPointLight(PointLight light, out vec4 ambient) {
	// C lculo da reflex o da componente da luz ambiente.
	ambient = vec4(material.ambient * light.ambient, 1.0);

	// C lculo da reflex o da componente da luz difusa.
	//vec3 lightPositionEyeSpace = mat3(View) * light.position;
	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(diffuseColor * light.diffuse, 1.0) * NdotL;

	// C lculo da reflex o da componente da luz especular.
	// Como os c lculos est o a ser realizados nas coordenadas do olho, ent o a c mara est  na posi  o (0,0,0).
	// Resulta ent o um vetor V entre os pontos (0,0,0) e vPositionEyeSpace:
	//		V = (0,0,0) - vPositionEyeSpace = (0-vPositionEyeSpace.x, 0-vPositionEyeSpace.y, 0-vPositionEyeSpace.z)
	// Que pode ser simplificado como:
	//		- vPositionEyeSpace
	vec3 V = normalize(-vPositionEyeSpace);
	//vec4 H = normalize(L + V);	// Modelo Blinn-Phong
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	// Modelo Blinn-Phong
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
	
	// attenuation
	float dist = length(mat3(View) * light.position - vPositionEyeSpace);	// C lculo da dist ncia entre o ponto de luz e o v rtice
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// C lculo da contribui  o da fonte de luz pontual para a cor final do fragmento.
	return (attenuation * (diffuse + specular));
}

vec4 calcSpotLight(SpotLight light, out vec4 ambient) 
{
	// Colocar aqui todo o c digo necess rio para calcular a contribui  o da fonte de luz c nica para a cor final do fragmento.
	return vec4(0.0);
}