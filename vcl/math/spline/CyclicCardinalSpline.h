//
// Created by Aloysio Galvão Lopes on 2020-06-01.
//

#ifndef PGM_CYCLICCARDINASPLINE_H
#define PGM_CYCLICCARDINASPLINE_H

#include "vcl/math/vec/vec.hpp"
#include "CardinalSpline.h"

namespace vcl {
class CyclicCardinalSpline : public CardinalSpline {
public:
    CyclicCardinalSpline(float duration);

    void addKeyFrame(vcl::vec3 position, float time) override;

    float getDuration() override;

protected:
    std::vector<vcl::vec4> adaptedKeyFrames;
    float duration;

    int indexAtValue(float time) override ;

    vcl::vec3 interpolation(float time) override;
};
}


#endif //PGM_CYCLICCARDINASPLINE_H
