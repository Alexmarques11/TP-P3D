#include "Light.h"
#include <GLFW/glfw3.h>

bool luzAmbienteAtiva = true;
bool luzDirecionalAtiva = true;
bool luzPontualAtiva = true;
bool luzConicaAtiva = true;

AmbientLight ambientLight;

void HandleKeyboardInput(int key, int action)
{
    if (action == GLFW_PRESS) {
        switch (key)
        {
        case GLFW_KEY_1:
            luzAmbienteAtiva = !luzAmbienteAtiva;
            break;
        case GLFW_KEY_2:
            luzDirecionalAtiva = !luzDirecionalAtiva;
            break;
        case GLFW_KEY_3:
            luzPontualAtiva = !luzPontualAtiva;
            break;
        case GLFW_KEY_4:
            luzConicaAtiva = !luzConicaAtiva;
            break;
        }
    }
    UpdateLightSources();
}

void UpdateLightSources()
{
    
    if (luzAmbienteAtiva) {
        ambientLight.color = glm::vec3(1.0f, 1.0f, 1.0f); // Cor branca (RGB: 1, 1, 1)
        ambientLight.intensity = 0.5f; // Intensidade de 0.5
    }
    else {
        // Desativar a luz ambiente, se necessário
        // Exemplo:
        // luzAmbiente.setIntensity(0.0f);
    }

    // Repita para as outras fontes de luz (direcional, pontual, conica) se necessário
}

