//
// Created by Aloysio Galvão Lopes on 2020-05-27.
//

#include "lerp.h"

namespace vcl {

    double lerp(double a, double b, double t) {
        return a + t * (b - a);
    }

    float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }
}
