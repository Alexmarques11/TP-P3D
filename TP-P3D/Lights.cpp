/*****************************************************************************
 * Lights.cpp
 *
 * Descrição:
 * ----------
 * Este arquivo contém a implementação da classe Lights, que gere as diferentes fontes de luz no jogo. A classe Lights é responsável por:
 * - Controlar o estado (ligado/desligado) de cada tipo de luz: ambiente, direcional, luz pontual e spot.
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


 /*****************************************************************************
 * Lights::Lights()
 *
 * Descrição:
 * ----------
 * Este é o construtor da classe `Lights`, responsável por inicializar uma nova
 * instância da classe e definir os valores iniciais das variáveis membro que
 * controlam o estado (ligado/desligado) de cada tipo de luz.
 *
 * Parâmetros:
 * -----------
 * - Nenhum.
 *
 * Retorno:
 * --------
 * - Nenhum (construtor).
 *
 * Observações:
 * -----------
 * - O construtor utiliza uma lista de inicialização para definir os valores iniciais das
 *  variáveis membro de forma mais eficiente do que faria no corpo do construtor.
 * - Os valores iniciais das luzes podem ser alterados posteriormente durante a execução
 *  do programa ao utilizar a função `ToggleLight`.
 *
 ******************************************************************************/
Lights::Lights()
	: isAmbientLightEnabled(true),
	isDirectionalLightEnabled(false),
	isPointLightEnabled(false),
	isSpotLightEnabled(false) {
	pointLights[0].position = glm::vec3(0.0f, 2.0f, 0.0f); // Posição da primeira luz pontual
	pointLights[1].position = glm::vec3(0.0f, 2.0f, 0.0f); // Posição da segunda luz pontual (pode ser diferente)
}


/*****************************************************************************
 * void Lights::ToggleLight(int key)
 *
 * Descrição:
 * ----------
 * Esta função membro da classe `Lights` é responsável por alternar o estado
 * (ligado/desligado) de um tipo de luz específico com base em um valor de tecla
 * fornecido como entrada. A função utiliza uma estrutura `switch` para determinar
 * qual luz deve ser alternada e, em seguida, inverte o estado da luz correspondente.
 *
 * Parâmetros:
 * -----------
 * - key: Um valor inteiro que representa a tecla pressionada pelo utilizador. Cada valor
 *  corresponde a um tipo específico de luz (1 para ambiente, 2 para direcional, 3 para luz pontual e 4 para spot).
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
