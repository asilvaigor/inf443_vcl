//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_BEARCOMPANION_H
#define PGM_BEARCOMPANION_H

#include <vcl/math/spline/CyclicCardinalSpline.h>
#include "vcl.hpp"
#include "objects/Object.h"
#include "Bear.h"

class BearCompanion : public Object{
public:
    BearCompanion(Shaders& shaders, vcl::CyclicCardinalSpline& trajectory, float initialS, std::shared_ptr<Bear>& bear);

    void drawMesh(vcl::camera_scene &camera, float time) override;

private:
    float s;
    float ds = 0.01;
    vcl::CyclicCardinalSpline& trajectory;
    vcl::mesh_drawable mesh;
    std::shared_ptr<Bear>& bear;

    vcl::vec3 dp;
    vcl::vec3 p;
    float currentTime;

    // TODO make this global
    const float T = 1.0/60;
    const float frictionFactor = 0.1f;

    void positionUpdate(float time);
};


#endif //PGM_BEARCOMPANION_H
