//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include <src/objects/bear/BearFilmingCompanion.h>
#include "CompanionFollower.h"

CompanionFollower::CompanionFollower(Shaders &shaders,
        std::shared_ptr<std::vector<std::shared_ptr<DipoleCompanion>>> &companions,
        std::shared_ptr<std::vector<float>> &trasitionTimes, vcl::vec3 initialPosition, bool debug) :
    Object(true), companions(companions), trasitionTimes(trasitionTimes),
    currentCompanionIndex(0), debug(debug)
{
    assert(!companions->empty());
    assert(trasitionTimes->size() == companions->size()-1);
    position = initialPosition;

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
    float dist = (position - (*companions)[currentCompanionIndex]->getPosition()).norm();

    // Companion specific updates
    // Specific for filming mode
    auto* filmingCompanion = dynamic_cast<BearFilmingCompanion*>((*companions)[currentCompanionIndex].get());
    if (filmingCompanion != nullptr){
        if (dist < filmingThreshold){
            float speedAdd = pow(dist, 2);
            speedAdd = std::min(speedAdd, maxSpeedFactor);
            position += (filmingCompanion->getPosition()-position).normalized()*speedAdd;

            dp = dp.normalized();
            vcl::vec3 newOrientation = filmingCompanion->getOrientation()*vcl::vec3(0, 1, 0).normalized();

            vcl::vec3 diff = (newOrientation-dp);

            dp += diff*0.05;

            orientation = vcl::rotation_euler(dp, 0.0f);

            return;
        }
    }

    float distNedagive = (position - (*companions)[currentCompanionIndex]->getNegativeChargePosition()).norm();

    // Activate the companion if necessary
    auto* activableCompanion = dynamic_cast<ActivatableCompanion*>((*companions)[currentCompanionIndex].get());
    if (activableCompanion != nullptr){
        activableCompanion->setActivationState(
                distNedagive < activationRadius && !activableCompanion->getActivationState());
    }

    // General updates
    // Updating dp (speed)
    // TODO pass initial speed and initial angle
    currentCompanionIndex = getTransitionIndex(time);

    updateDp();

    if (distNedagive < quadraticSpeedThreshold){
        position+=dp*(distNedagive*distNedagive)/pow(quadraticSpeedThreshold, 2);
    }
    else position+=dp;

    orientation = vcl::rotation_euler(dp, (*companions)[currentCompanionIndex]->getRotationAngle());

}

void CompanionFollower::updateDp() {
    // Following field
    dp = (*companions)[currentCompanionIndex]->getFieldAt(position).normalized()*maxSpeedFactor;
}

int CompanionFollower::getTransitionIndex(float time) {
    int k = 0;
    while (k <= (int)trasitionTimes->size()-1 && (*trasitionTimes)[k] < time)
        k++;
    return k;
}




