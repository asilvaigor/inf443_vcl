//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_DIPOLECOMPANION_H
#define PGM_DIPOLECOMPANION_H

#include "CompanionObject.h"

class DipoleCompanion : public CompanionObject {
public:
    DipoleCompanion(Shaders &shaders, bool debug=true);

    void drawMesh(vcl::camera_scene &camera) override;

    vcl::vec2 getFieldAt(vcl::vec2 pos);

    vcl::vec3 getFieldAt(vcl::vec3 pos);
protected:
    // Mesh
    vcl::mesh_drawable chargeMesh;

    // Charges variables
    vcl::vec3 pc1, pc2;

    // TODO make this global
    const float T = 1.0/60;
    const float frictionFactor = 0.01f;
    const float distanceDipole = 5.0f;
    const float dipoleOffset = 8.0f;
    const float charge = 15.0f;

    void updateChargesPositions();
};


#endif //PGM_DIPOLECOMPANION_H
