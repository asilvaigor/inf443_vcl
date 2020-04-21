//
// Created by igor on 20/04/2020.
//

#pragma once

#include <random>

namespace vcl {

class rand_generator {
public:
    rand_generator(bool deterministic = true);

    float rand(float mi = -1.0f, float ma = 1.0f);

    std::default_random_engine getState();

    void setState(std::default_random_engine s);

private:
    std::default_random_engine state;
    std::uniform_real_distribution<float> distribution;
};

}
