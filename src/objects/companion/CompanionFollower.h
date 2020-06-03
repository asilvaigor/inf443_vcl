//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_COMPANIONFOLLOWER_H
#define PGM_COMPANIONFOLLOWER_H

#include "objects/Object.h"
#include "ActivatableCompanion.h"

class CompanionFollower : public Object {
public:
    CompanionFollower(Shaders &shaders, std::vector<std::shared_ptr<ActivatableCompanion>> &companions,
                      std::vector<float> &trasitionTimes, vcl::vec3 initialPosition = {0, 0, 0}, bool debug = true);

    void drawMesh(vcl::camera_scene &camera) override;

    void update(float time) override;

    void updateDp();

    int getTransitionIndex(float time);

private:
    // Meshes for drawing object
    vcl::mesh_drawable sphereMesh;
    vcl::mesh_drawable axisMarkerMesh;

    // Companion
    std::vector<std::shared_ptr<ActivatableCompanion>> &companions;
    std::vector<float> &trasitionTimes;

    // State variables
    vcl::vec3 dp;
    int currentCompanionIndex;

    // Constants
    const float quadraticSpeedThreshold = 10;
    const float maxSpeedFactor = 0.4;
    const float activationRadius = 10;

    // In debug mode
    bool debug;
};


#endif //PGM_COMPANIONFOLLOWER_H
