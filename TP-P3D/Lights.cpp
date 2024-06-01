/*****************************************************************************
 * Lights.cpp
 *
 * Descri��o:
 * ----------
 * Este arquivo cont�m a implementa��o da classe Lights, que gere as diferentes fontes de luz no jogo. A classe Lights � respons�vel por:
 * - Controlar o estado (ligado/desligado) de cada tipo de luz: ambiente, direcional, luz pontual e spot.
 * - Alternar o estado das luzes atrav�s da fun��o ToggleLight, que � chamada quando o utilizador pressiona as teclas correspondentes.
 *
 * Fun��es principais:
 * - Lights(): Construtor da classe Lights, que inicializa os estados das luzes.
 * - ToggleLight(int key): Alterna o estado de uma luz espec�fica com base na tecla pressionada.
 *
 * Vari�veis e constantes importantes:
 * - isAmbientLightEnabled: Indica se a luz ambiente est� ligada (true) ou desligada (false).
 * - isDirectionalLightEnabled: Indica se a luz direcional est� ligada (true) ou desligada (false).
 * - isPointLightEnabled: Indica se a luz pontual est� ligada (true) ou desligada (false).
 * - isSpotLightEnabled: Indica se a luz spot est� ligada (true) ou desligada (false).
 *
 ******************************************************************************/


#include "Lights.h"
#include <iostream>


 /*****************************************************************************
 * Lights::Lights()
 *
 * Descri��o:
 * ----------
 * Este � o construtor da classe `Lights`, respons�vel por inicializar uma nova
 * inst�ncia da classe e definir os valores iniciais das vari�veis membro que
 * controlam o estado (ligado/desligado) de cada tipo de luz.
 *
 * Par�metros:
 * -----------
 * - Nenhum.
 *
 * Retorno:
 * --------
 * - Nenhum (construtor).
 *
 * Observa��es:
 * -----------
 * - O construtor utiliza uma lista de inicializa��o para definir os valores iniciais das
 *  vari�veis membro de forma mais eficiente do que faria no corpo do construtor.
 * - Os valores iniciais das luzes podem ser alterados posteriormente durante a execu��o
 *  do programa ao utilizar a fun��o `ToggleLight`.
 *
 ******************************************************************************/
Lights::Lights()
	: isAmbientLightEnabled(true),
	isDirectionalLightEnabled(false),
	isPointLightEnabled(false),
	isSpotLightEnabled(false) {
	pointLights[0].position = glm::vec3(0.0f, 2.0f, 0.0f); // Posi��o da primeira luz pontual
	pointLights[1].position = glm::vec3(0.0f, 2.0f, 0.0f); // Posi��o da segunda luz pontual (pode ser diferente)
}


/*****************************************************************************
 * void Lights::ToggleLight(int key)
 *
 * Descri��o:
 * ----------
 * Esta fun��o membro da classe `Lights` � respons�vel por alternar o estado
 * (ligado/desligado) de um tipo de luz espec�fico com base em um valor de tecla
 * fornecido como entrada. A fun��o utiliza uma estrutura `switch` para determinar
 * qual luz deve ser alternada e, em seguida, inverte o estado da luz correspondente.
 *
 * Par�metros:
 * -----------
 * - key: Um valor inteiro que representa a tecla pressionada pelo utilizador. Cada valor
 *  corresponde a um tipo espec�fico de luz (1 para ambiente, 2 para direcional, 3 para luz pontual e 4 para spot).
 *
 * Retorno:
 * --------
 * - Nenhum (void).
 *
 ******************************************************************************/
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
