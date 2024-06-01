#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>

class Lights {
public:
	bool isAmbientLightEnabled;  // Indica se a luz ambiente está ativa
	bool isDirectionalLightEnabled; // Indica se a luz direcional está ativa
	bool isPointLightEnabled;   // Indica se a luz pontual está ativa
	bool isSpotLightEnabled;    // Indica se a luz spot está ativa

	Lights(); // Construtor da classe Lights

	void ToggleLight(int key); // Alterna o estado de uma luz com base na tecla pressionada

	struct PointLight {
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;
	};

	PointLight pointLights[2];
};

#endif // LIGHTS_H
