//
// Created by Aloysio Galvão Lopes on 2020-06-03.
//

#include <vcl/math/spline/CyclicCardinalSpline.h>
#include <src/objects/debug/TrajectoryDrawer.h>
#include <src/objects/debug/DebugObject.h>
#include <src/objects/forest/Forest.h>
#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/bear/Bear.h"
#include "objects/terrain/MountainTerrain.h"
#include "objects/terrain/FlatSurface.h"
#include "objects/bear/BearWalkingCompanion.h"

int main() {
    auto scene = Scene::getInstance("Bear Trajectory Test");
    vcl::camera_scene &camera = scene.getGui()->getCamera();

    // Camera settings
    camera.set_translation({-50, -50, 0});

    float phi = M_PI_2 - M_PI_4 / 2;
    float theta = M_PI_2 + M_PI_4;

    const vcl::mat3 Rx = {1, 0, 0,
                          0, std::cos(phi), -std::sin(phi),
                          0, std::sin(phi), std::cos(phi)};

    const vcl::mat3 Rz = {std::cos(theta), -std::sin(theta), 0,
                          std::sin(theta), std::cos(theta), 0,
                          0, 0, 1};


    camera.set_orientation(Rz * Rx);
    camera.set_scale(50);
    scene.getGui()->setShowVertices(true);

    // Terrain

    WaterLimits waterLimits(-100, 100, -35, 135, -30, -10);
    std::shared_ptr<BaseTerrain> terrain = std::make_shared<MountainTerrain>(scene.getShaders(), 280, 280, waterLimits);
    std::shared_ptr<Object> terrainObj = std::dynamic_pointer_cast<Object>(terrain);
    auto forest = std::static_pointer_cast<Object>(std::make_shared<Forest>(scene.getShaders(), terrain));
    std::shared_ptr<Forest> forestPtr = std::static_pointer_cast<Forest>(forest);

    scene.addObject(terrainObj);
    scene.addObject(forest);

    // Cyclic spline
    // Adding positions
    std::vector<vcl::vec3> keyframes;
    keyframes.emplace_back(50, -100, 5);
    keyframes.emplace_back(50, 50, 5);
    keyframes.emplace_back(0, 50, 5);
    keyframes.emplace_back(0, 0, 5);

    auto spline = std::make_shared<vcl::CyclicCardinalSpline>((float) keyframes.size());

    // Drawing trajectory
    //Adding positions to spline
    for (int i = 0; i < (int) keyframes.size(); ++i)
        spline->addKeyFrame(keyframes[i], (float) i);

    // Trajectory
    const float dt = 0.01;
    float t = 0;
    float tf = (float) keyframes.size();
    auto traj = std::make_shared<std::vector<vcl::vec3>>();
    while (t <= tf) {
        traj->push_back(spline->position(t));
        t += dt;
    }

    auto drawer = std::static_pointer_cast<Object>(std::make_shared<TrajectoryDrawer>(scene.getShaders(), traj));
    scene.addObject(drawer);

    // Bear animation part
    std::shared_ptr<Object> bear;
    std::shared_ptr<vcl::vec3> bearPos = std::make_shared<vcl::vec3>(0, 0, 0);

    // Adding bear companion
    auto companion = std::static_pointer_cast<Object>(std::make_shared<BearWalkingCompanion>(
            scene.getShaders(), spline, 6.0f, bearPos));
    scene.addObject(companion);

    std::shared_ptr<BearWalkingCompanion> bearCompanionPtr = std::static_pointer_cast<BearWalkingCompanion>(companion);

    // Creating bear
    bear = std::make_shared<Bear>(scene.getShaders(), terrain, bearCompanionPtr, bearPos, forestPtr);
    auto bearObj = std::static_pointer_cast<Object>(bear);
    scene.addObject(bearObj);

    scene.display();
    return 0;
}