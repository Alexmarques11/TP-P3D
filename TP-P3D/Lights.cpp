/*****************************************************************************
 * Lights.cpp
 *
 * Descrição:
 * ----------
 * Este arquivo contém a implementação da classe Lights, que gerencia as diferentes fontes de luz no jogo. A classe Lights é responsável por:
 * - Controlar o estado (ligado/desligado) de cada tipo de luz: ambiente, direcional, pontual e spot.
 * - Alternar o estado das luzes através da função ToggleLight, que é chamada quando o utilizador pressiona as teclas correspondentes.
 *
 * Funções principais:
 * - Lights(): Construtor da classe Lights, que inicializa os estados das luzes.
 * - ToggleLight(int key): Alterna o estado de uma luz específica com base na tecla pressionada.
 *
 * Variáveis e constantes importantes:
 * - isAmbientLightEnabled: Indica se a luz ambiente está ligada (true) ou desligada (false).
 * - isDirectionalLightEnabled: Indica se a luz direcional está ligada (true) ou desligada (false).
 * - isPointLightEnabled: Indica se a luz pontual está ligada (true) ou desligada (false).
 * - isSpotLightEnabled: Indica se a luz spot está ligada (true) ou desligada (false).
 *
 ******************************************************************************/


#include "Lights.h"
#include <iostream>

Lights::Lights()
	: isAmbientLightEnabled(true),
	isDirectionalLightEnabled(false),
	isPointLightEnabled(false),
	isSpotLightEnabled(false) {}


void Lights::ToggleLight(int key) {
	switch (key) {
	case 1:
		isAmbientLightEnabled = !isAmbientLightEnabled;
		std::cout << "Ambient light toggled. Now ";
		std::cout << (isAmbientLightEnabled ? "enabled" : "disabled") << std::endl;
		break;
	case 2:
		isDirectionalLightEnabled = !isDirectionalLightEnabled;
		std::cout << "Directional light toggled. Now ";
		std::cout << (isDirectionalLightEnabled ? "enabled" : "disabled") << std::endl;
		break;
	case 3:
		isPointLightEnabled = !isPointLightEnabled;
		std::cout << "Point light toggled. Now ";
		std::cout << (isPointLightEnabled ? "enabled" : "disabled") << std::endl;
		break;
	case 4:
		isSpotLightEnabled = !isSpotLightEnabled;
		std::cout << "Spot light toggled. Now ";
		std::cout << (isSpotLightEnabled ? "enabled" : "disabled") << std::endl;
		break;
	default:
		break;
	}
}
