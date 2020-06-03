//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "CompanionFollower.h"

CompanionFollower::CompanionFollower(Shaders &shaders, std::vector<std::shared_ptr<ActivatableCompanion>> &companions,
                                     std::vector<float> &trasitionTimes,
                                     bool debug) :
    Object(true), companions(companions), trasitionTimes(trasitionTimes), currentCompanionIndex(0), debug(debug)
{
    assert(!companions.empty());
    assert(trasitionTimes.size() == companions.size()-1);

    // Setting um meshes if in debug mode
    if (debug){
        // TODO do not hardcode the sizes
        sphereMesh = vcl::mesh_primitive_sphere();
        sphereMesh.shader = shaders["mesh"];
        axisMarkerMesh = vcl::mesh_drawable(vcl::mesh_primitive_frame());
        axisMarkerMesh.shader = shaders["mesh"];
        axisMarkerMesh.uniform.transform.scaling = 2.0f;
    }
}

void CompanionFollower::drawMesh(vcl::camera_scene &camera) {
    // Print meshes if in debug mode
    if (debug){
        // Sphere
        sphereMesh.uniform.transform.translation = position;
        sphereMesh.uniform.lights = lights;

        vcl::draw(sphereMesh, camera);

        // Marker
        axisMarkerMesh.uniform.transform.translation = position;
        axisMarkerMesh.uniform.transform.rotation = orientation;
        axisMarkerMesh.uniform.lights = lights;

        vcl::draw(axisMarkerMesh, camera);
    }
}

void CompanionFollower::update(float time) {
    // Updating dp (speed)
    // TODO pass initial speed and initial angle
    updateDp();

    float dist = (position-companions[currentCompanionIndex]->getNegativeChargePosition()).norm();
    if (dist < quadraticSpeedThreshold){
        position+=dp*(dist*dist)/pow(quadraticSpeedThreshold, 2);
    }
    else position+=dp;

    // Activate the companion if necessary
    if (dist  < activationRadius && !companions[currentCompanionIndex]->getActivationState())
        companions[currentCompanionIndex]->setActivationState(true);


    // Making orientation matrix match speed vector and angle
    // Rotation using euler angles
    vcl::vec3 projDpXY = {dp.x, dp.y, 0};

    float alpha;
    if (vcl::cross(projDpXY, {0, 1, 0}).z > 0)
        alpha = -projDpXY.angle({0, 1, 0});
    else alpha = projDpXY.angle({0, 1, 0});
    vcl::mat3 aRotation = vcl::rotation_from_axis_angle_mat3({0, 0, 1}, alpha);

    vcl::vec3 bAxis = vcl::cross(dp, {0, 0, 1});
    float beta = -dp.angle({0, 0, 1})+M_PI_2;
    vcl::mat3 bRotation = vcl::rotation_from_axis_angle_mat3(bAxis, beta);

    // TODO make set angle
//    vcl::vec3 projDpo = {odp.x, odp.y, 1};
//    vcl::vec3 projNdp = {ndp.x, ndp.y, 1};
//    float gamma = projDpo.angle(projNdp)*turining;
//    if (gamma > M_PI_2)
//        gamma = M_PI_2;
//    else if (gamma < -M_PI_2)
//        gamma = -M_PI_2;
//    vcl::mat3 cRotation = vcl::rotation_from_axis_angle_mat3(dp, gamma);

    orientation = bRotation*aRotation;
}

void CompanionFollower::updateDp() {
    // Following field
    dp = companions[currentCompanionIndex]->getFieldAt(position).normalized()*maxSpeedFactor;
}

int CompanionFollower::getTransitionIndex(float time) {
//    int k = 0;
//    while (trasitionTimes[k] < time &&)
//        k++;
//    return k-1;
}




