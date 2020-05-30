//
// Created by Aloysio Galvão Lopes on 2020-05-29.
//

#ifndef PGM_WATEROSCILLATOR_H
#define PGM_WATEROSCILLATOR_H

#include "vcl.hpp"

class WaterOscillator {
public:
    WaterOscillator(vcl::vec2 position, float timestep, float charge, float amplitude,
                    float initial_phase);

    vcl::vec3 step();

    vcl::vec3 getPosition();
private:
    vcl::vec2 postion;
    float timestep;
    float charge;
    float amplitude;
    float phase;
};


#endif //PGM_WATEROSCILLATOR_H
