#include <utility>

//
// Created by Aloysio Galvão Lopes on 2020-05-29.
//

#include "WaterOscillator.h"

WaterOscillator::WaterOscillator(vcl::vec2 position, float timestep, float charge, float amplitude,
                                 float initial_phase, bool debug) : postion(position), timestep(timestep),
        charge(charge), amplitude(amplitude), phase(initial_phase), debug(debug) {
    if (debug){
        meshDrawable = vcl::mesh_primitive_sphere(1);
        meshDrawable.uniform.color = {1, 0, 0};
        meshDrawable.uniform.transform.translation = {position.x, position.y, amplitude*sin(phase)};
    }
}

vcl::vec3 WaterOscillator::step() {amplitude*sin(phase);
    phase += timestep;

    if (debug)
        meshDrawable.uniform.transform.translation = {postion.x, postion.y, amplitude*sin(phase)};
    return {postion.x, postion.y, amplitude*sin(phase)};
}

vcl::vec3 WaterOscillator::getPosition() {
    return {postion.x, postion.y, amplitude*sin(phase)};
}

bool WaterOscillator::getDebugState() {
    return debug;
}

void WaterOscillator::setShaders(Shaders &shaders) {
    // Must be on debug mode
    assert(debug);

    meshDrawable.shader = shaders["mesh"];
}

void WaterOscillator::setLight(std::shared_ptr<vcl::light_source> light) {
    // Must be on debug mode
    assert(debug);
    meshDrawable.uniform.light = std::move(light);
}

vcl::mesh_drawable &WaterOscillator::getMesh() {
    return meshDrawable;
}

float WaterOscillator::getCharge() {
    return charge;
}
