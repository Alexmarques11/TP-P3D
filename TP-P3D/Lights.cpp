#include "Lights.h"
#include <iostream>

Lights::Lights()
    : isAmbientLightEnabled(false),
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
