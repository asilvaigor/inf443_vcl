//
// Created by Aloysio Galvão Lopes on 2020-06-01.
//

#include <assert.h>
#include "CardinalSpline.h"

vcl::CardinalSpline::CardinalSpline() {
    initializedLimits = false;
}

void vcl::CardinalSpline::addKeyFrame(vcl::vec3 position, float time) {
    keyframes.emplace_back(position, time);

    if (!initializedLimits){
        minT = time;
        maxT = time;
        initializedLimits = true;
    } else {
        minT = std::min(time, minT);
        maxT = std::max(time, maxT);
    }

    std::sort(keyframes.begin(), keyframes.end(), [&](vcl::vec4& a, vcl::vec4 b){
        return a.w < b.w;
    });
}

vcl::vec3 vcl::CardinalSpline::position(float time) {

    return interpolation(time);
}

int vcl::CardinalSpline::indexAtValue(float time)
{
    assert(time >= minT && time <= maxT);
    int k=0;
    while(keyframes[k+1].w < time)
        ++k;

    return k;
}

vcl::vec3 vcl::CardinalSpline::interpolation(float time) {
    int idx = indexAtValue(time);

    // Preparation of data for the interpolation
    float t0 = keyframes[idx ].w; // = t_{i}
    if (idx > 0)
        t0 = keyframes[idx-1].w; // = t_{i-1}
    const float t1 = keyframes[idx  ].w; // = t_i
    const float t2 = keyframes[idx+1].w; // = t_{i+1}
    float t3 = keyframes[idx+1].w; // = t_{i+1}
    if (idx != (int)keyframes.size()-1)
        t3 = keyframes[idx+2].w; // = t_{i+2}

    vec3 p0 = {keyframes[idx].x, keyframes[idx].y, keyframes[idx].z}; // = p_{i}
    if (idx > 0)
        p0 = {keyframes[idx-1].x, keyframes[idx-1].y, keyframes[idx-1].z}; // = p_{i-1}
    const vec3 p1 = {keyframes[idx  ].x, keyframes[idx  ].y, keyframes[idx  ].z}; // = p_i
    const vec3 p2 = {keyframes[idx+1].x, keyframes[idx+1].y, keyframes[idx+1].z}; // = p_{i+1}
    vec3 p3 = {keyframes[idx+1].x, keyframes[idx+1].y, keyframes[idx+1].z}; // = p_{i+1}
    if (idx != (int)keyframes.size()-1)
        p3 = {keyframes[idx+2].x, keyframes[idx+2].y, keyframes[idx+2].z}; // = p_{i+2}

    interpolationFunction(time, t0, t1, t2, t3, p0, p1, p2, p3);

    return interpol;
}

void vcl::CardinalSpline::interpolationFunction(float time, float t0, float t1, float t2, float t3, const vcl::vec3 &p0,
                                           const vcl::vec3 &p1, const vcl::vec3 &p2, const vcl::vec3 &p3) {
    float s = (time-t1)/(t2-t1);

    vec3 d1 = 2*K*((p2-p0)/(t2-t0));
    vec3 d2 = 2*K*((p3-p1)/(t3-t1));

    float s3 = s*s*s;
    float s2 = s*s;

    // Current point
    interpol = (2*s3-3*s2+1)*p1+(s3-2*s2+s)*d1+(-2*s3+3*s2)*p2+(s3-s2)*d2;
}

float vcl::CardinalSpline::getDuration() {
    return maxT;
}
