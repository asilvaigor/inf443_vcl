//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_DIPOLECOMPANION_H
#define PGM_DIPOLECOMPANION_H

#include "CompanionObject.h"
#include "utils/Constants.h"

class DipoleCompanion : public CompanionObject {
public:
    DipoleCompanion(Shaders &shaders, bool debug=true);

    void drawMesh(vcl::camera_scene &camera) override;

    void update(float time) override;

    vcl::vec2 getFieldAt(vcl::vec2 pos);

    vcl::vec3 getFieldAt(vcl::vec3 pos);

    vcl::vec3 getNegativeChargePosition();

    float getRotationAngle();
protected:
    // Mesh
    vcl::mesh_drawable chargeMesh;

    // Charges variables
    vcl::vec3 pc1, pc2;
    float rotation;

    void updateChargesPositions();
};


#endif //PGM_DIPOLECOMPANION_H
