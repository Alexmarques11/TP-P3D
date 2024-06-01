#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>

class Lights {
public:
	bool isAmbientLightEnabled;    // Indica se a luz ambiente está ativa
	bool isDirectionalLightEnabled; // Indica se a luz direcional está ativa
	bool isPointLightEnabled;      // Indica se a luz pontual está ativa
	bool isSpotLightEnabled;       // Indica se a luz spot está ativa

	Lights();  // Construtor da classe Lights

	void ToggleLight(int key); // Alterna o estado de uma luz com base na tecla pressionada
};

#endif // LIGHTS_H
