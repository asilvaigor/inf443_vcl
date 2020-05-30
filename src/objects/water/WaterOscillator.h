//
// Created by Aloysio Galvão Lopes on 2020-05-29.
//

#ifndef PGM_WATEROSCILLATOR_H
#define PGM_WATEROSCILLATOR_H

#include <src/shaders/Shaders.h>
#include "vcl.hpp"

class WaterOscillator {
public:
    WaterOscillator(vcl::vec2 position, float timestep, float charge = 1.0f, float amplitude = 1.0f,
                    float initial_phase = 0.0f, bool debug = false);

    vcl::vec3 step();

    vcl::vec3 getPosition();

    void setShaders(Shaders& shaders);

    void setLight(std::shared_ptr<vcl::light_source> light);

    float getCharge();

    bool getDebugState();

    vcl::mesh_drawable& getMesh();

private:
    vcl::vec2 postion;
    vcl::mesh_drawable meshDrawable;
    float timestep;
    float charge;
    float amplitude;
    float phase;
    bool debug;
};


#endif //PGM_WATEROSCILLATOR_H
