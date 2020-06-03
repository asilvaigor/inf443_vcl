//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_COMPANIONFOLLOWER_H
#define PGM_COMPANIONFOLLOWER_H

#include "objects/Object.h"
#include "CompanionObject.h"

class CompanionFollower : public Object {
public:
    CompanionFollower(Shaders& shaders, std::shared_ptr<CompanionObject>& companion, bool debug=true);

    void drawMesh(vcl::camera_scene &camera) override;

    void update(float time) override;

private:
    // Meshes for drawing object
    vcl::mesh_drawable sphereMesh;
    vcl::mesh_drawable axisMarkerMesh;

    // Companion
    std::shared_ptr<CompanionObject>& companion;

    // State variables
    vcl::vec3 dp;

    // In debug mode
    bool debug;
};


#endif //PGM_COMPANIONFOLLOWER_H
