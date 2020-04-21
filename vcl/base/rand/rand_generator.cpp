//
// Created by igor on 20/04/2020.
//

#include <chrono>
#include "rand_generator.h"

namespace vcl {

rand_generator::rand_generator(bool deterministic) : distribution(0, 1) {
    unsigned seed;
    if (deterministic)
        seed = 0;
    else seed = std::chrono::system_clock::now().time_since_epoch().count();
    state.seed(seed);
}

float rand_generator::rand(float mi, float ma) {
    return distribution(state) * (ma - mi) + mi;
}

std::default_random_engine rand_generator::getState() {
    return state;
}

void rand_generator::setState(std::default_random_engine s) {
    state = s;
}

}
