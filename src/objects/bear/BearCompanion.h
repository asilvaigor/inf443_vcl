//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_BEARCOMPANION_H
#define PGM_BEARCOMPANION_H

#include <vcl/math/spline/CyclicCardinalSpline.h>
#include "vcl.hpp"
#include "objects/Object.h"

class BearCompanion : public Object{
public:
    BearCompanion(Shaders& shaders, vcl::CyclicCardinalSpline& trajectory, float initialS,
            std::shared_ptr<vcl::vec3> bearPosition);

    void drawMesh(vcl::camera_scene &camera) override;

    void update(float time) override;

    vcl::vec2 getFieldAt(vcl::vec2 pos);

private:
    float s;
    float ds = 0.01;
    vcl::CyclicCardinalSpline& trajectory;
    vcl::mesh_drawable mesh;
    vcl::mesh_drawable chargeMesh;
    std::shared_ptr<vcl::vec3> bearPosition;

    vcl::vec3 dp;
    float currentTime;

    // Chargers variables
    vcl::vec3 pc1, pc2;

    // TODO make this global
    const float T = 1.0/60;
    const float frictionFactor = 0.01f;
    const float distanceDipole = 5.0f;
    const float dipoleOffset = 8.0f;
    const float charge = 15.0f;

    void positionUpdate(float time);

    void updateChargesPositions();
};


#endif //PGM_BEARCOMPANION_H
