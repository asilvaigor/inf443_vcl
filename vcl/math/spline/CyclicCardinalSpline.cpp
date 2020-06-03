//
// Created by Aloysio Galvão Lopes on 2020-06-01.
//

#include "CyclicCardinalSpline.h"

vcl::CyclicCardinalSpline::CyclicCardinalSpline(float duration) : duration(duration) {

}

void vcl::CyclicCardinalSpline::addKeyFrame(vcl::vec3 position, float time) {
    CardinalSpline::addKeyFrame(position, time);

    if (keyframes.size() >= 2){
        std::vector<vcl::vec4> adapted;
        adapted.emplace_back(keyframes[keyframes.size()-1].x,
                keyframes[keyframes.size()-1].y,
                keyframes[keyframes.size()-1].z,
                keyframes[0].w-(duration-keyframes[keyframes.size()-1].w));

        for (auto key: keyframes)
            adapted.emplace_back(key);

        adapted.emplace_back(keyframes[0].x, keyframes[0].y, keyframes[0].z, duration);
        adapted.emplace_back(keyframes[1].x, keyframes[1].y, keyframes[1].z, duration+keyframes[1].w-keyframes[0].w);

        adaptedKeyFrames = std::move(adapted);
    }
}

vcl::vec3 vcl::CyclicCardinalSpline::interpolation(float time) {
    // Interpolating position
    if (time < 0){
        time = fmod(-time, duration);
        time = duration-time;
    }
    time = fmod(time, duration);

    int idx = indexAtValue(time)+1;

    // Preparation of data for the interpolation
    const float t0 = adaptedKeyFrames[idx-1].w; // = t_{i-1}
    const float t1 = adaptedKeyFrames[idx  ].w; // = t_i
    const float t2 = adaptedKeyFrames[idx+1].w; // = t_{i+1}
    const float t3 = adaptedKeyFrames[idx+2].w; // = t_{i+2}

    const vec3 p0 = {adaptedKeyFrames[idx-1].x, adaptedKeyFrames[idx-1].y, adaptedKeyFrames[idx-1].z}; // = p_{i-1}
    const vec3 p1 = {adaptedKeyFrames[idx  ].x, adaptedKeyFrames[idx  ].y, adaptedKeyFrames[idx  ].z}; // = p_i
    const vec3 p2 = {adaptedKeyFrames[idx+1].x, adaptedKeyFrames[idx+1].y, adaptedKeyFrames[idx+1].z}; // = p_{i+1}
    const vec3 p3 = {adaptedKeyFrames[idx+2].x, adaptedKeyFrames[idx+2].y, adaptedKeyFrames[idx+2].z}; // = p_{i+2}

    interpolationFunction(time, t0, t1, t2, t3, p0, p1, p2, p3);

    return interpol;
}

int vcl::CyclicCardinalSpline::indexAtValue(float time) {
    if (time >= maxT)
        return keyframes.size()-1;

    return CardinalSpline::indexAtValue(time);
}

float vcl::CyclicCardinalSpline::getDuration() {
    return duration;
}