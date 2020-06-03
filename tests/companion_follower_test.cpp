//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include <vcl/math/spline/CyclicCardinalSpline.h>
#include <src/objects/debug/TrajectoryDrawer.h>
#include <src/objects/debug/DebugObject.h>
#include <src/objects/companion/CyclicCompanion.h>
#include <src/objects/companion/CompanionFollower.h>
#include <src/objects/companion/OneWayCompanion.h>
#include "scene/Scene.h"
#include "shaders/Shaders.h"

int main() {
    auto scene = Scene::getInstance("Companion Follower Test");
    vcl::camera_scene &camera = scene.getGui()->getCamera();

    // Camera settings
    camera.set_translation({-50, -50, -10});

    float phi = M_PI_2 - M_PI_4 / 2;
    float theta = M_PI_2 + M_PI_4;

    const vcl::mat3 Rx = {1, 0, 0,
                          0, std::cos(phi), -std::sin(phi),
                          0, std::sin(phi), std::cos(phi)};

    const vcl::mat3 Rz = {std::cos(theta), -std::sin(theta), 0,
                          std::sin(theta), std::cos(theta), 0,
                          0, 0, 1};


    camera.set_orientation(Rz*Rx);
    camera.set_scale(50);
    scene.getGui()->setShowVertices(true);

    // Cyclic spline
    // Adding positions
    std::vector<vcl::vec3> keyframes;
    keyframes.emplace_back(50, 0, 5);
    keyframes.emplace_back(50, 50, 5);
    keyframes.emplace_back(0, 50, -10);
    keyframes.emplace_back(0, 0, 5);

    auto spline = std::make_shared<vcl::CardinalSpline>();
    auto spline1 = std::make_shared<vcl::CyclicCardinalSpline>((float) keyframes.size());

    // Drawing trajectory
    //Adding positions to spline
    for (int i = 0; i < (int) keyframes.size(); ++i) {
        spline->addKeyFrame(keyframes[i], (float) i);
        spline1->addKeyFrame(keyframes[i], (float) i);
    }

    // Trajectory
    const float dt = 0.01;
    float t = 0;
    float tf = 3;
    auto traj = std::make_shared<std::vector<vcl::vec3>>();
    while (t <= tf) {
        traj->push_back(spline->position(t));
        t += dt;
    }

    auto drawer = std::static_pointer_cast<Object>(std::make_shared<TrajectoryDrawer>(scene.getShaders(), traj));
    scene.addObject(drawer);

    // Adding companion
    auto companion1 = std::static_pointer_cast<Object>(std::make_shared<CyclicCompanion>(
            scene.getShaders(), spline1, 0.0f));
    std::shared_ptr<DipoleCompanion> companionPtr1 = std::static_pointer_cast<DipoleCompanion>(companion1);
    scene.addObject(companion1);

    // Adding companion
    auto companion = std::static_pointer_cast<Object>(std::make_shared<OneWayCompanion>(
            scene.getShaders(), spline, 0.0f));
    std::shared_ptr<DipoleCompanion> companionPtr = std::static_pointer_cast<DipoleCompanion>(companion);
    scene.addObject(companion);

    // Adding companion follower to the scene
    auto companions = std::make_shared<std::vector<std::shared_ptr<DipoleCompanion>>>();
    companions->emplace_back(companionPtr);
    companions->emplace_back(companionPtr1);

    auto transitionTimes = std::make_shared<std::vector<float>>();
    transitionTimes->emplace_back(20);

    auto follower = std::static_pointer_cast<Object>(std::make_shared<CompanionFollower>(
            scene.getShaders(), companions, transitionTimes));
    scene.addObject(follower);

    scene.display();
    return 0;
}
