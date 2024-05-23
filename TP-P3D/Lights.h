#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>

class Lights {
public:

    bool isAmbientLightEnabled;
    bool isDirectionalLightEnabled;
    bool isPointLightEnabled;
    bool isSpotLightEnabled;

    Lights();

    void ToggleLight(int key);
};

#endif // LIGHTS_H