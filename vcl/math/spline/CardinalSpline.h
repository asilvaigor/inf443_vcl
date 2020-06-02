//
// Created by Aloysio Galvão Lopes on 2020-06-01.
//

#ifndef PGM_CARDINALSPLINE_H
#define PGM_CARDINALSPLINE_H

#include "vcl.hpp"

namespace vcl{
class CardinalSpline {
public:
    CardinalSpline();

    void addKeyFrame(vcl::vec3 position, float time);

    vcl::vec3 position(float time);

    int indexAtValue(float time);

protected:
    std::vector<vcl::vec4> keyframes;
    bool initializedLimits;
    float minT, maxT;

    // Cardinal spline constant Catmull-Rom
    const float K = 0.5;

    vcl::vec3 interpolation(float time);
};
}


#endif //PGM_CARDINALSPLINE_H
