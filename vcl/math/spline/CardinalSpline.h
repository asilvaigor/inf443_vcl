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

    virtual void addKeyFrame(vcl::vec3 position, float time);

    vcl::vec3 position(float time);

protected:
    std::vector<vcl::vec4> keyframes;
    bool initializedLimits;
    float minT, maxT;
    vcl::vec3 interpol;

    // Cardinal spline constant Catmull-Rom
    const float K = 0.5;

    virtual int indexAtValue(float time);

    virtual vcl::vec3 interpolation(float time);

    void interpolationFunction(float time, float t0, float t1, float t2, float t3,
                               const vec3 &p0, const vec3 &p1, const vec3 &p2, const vec3 &p3);
};
}


#endif //PGM_CARDINALSPLINE_H
