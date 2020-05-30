//
// Created by Aloysio Galvão Lopes on 2020-05-29.
//

#include "WaterOscillator.h"

WaterOscillator::WaterOscillator(vcl::vec2 position, float timestep, float charge = 1.0f, float amplitude = 1.0f,
                                 float initial_phase = 0.0f) : postion(position), timestep(timestep),
        charge(charge), amplitude(amplitude), phase(initial_phase) {}

vcl::vec3 WaterOscillator::step() {amplitude*sin(phase);
    phase += timestep;
    return {postion.x, postion.y, amplitude*sin(phase)};
}

vcl::vec3 WaterOscillator::getPosition() {
    return {postion.x, postion.y, amplitude*sin(phase)};
}
