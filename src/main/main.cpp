//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#include <src/objects/debug/KeyframesDrawer.h>
#include <src/objects/debug/TrajectoryDrawer.h>
#include "scene/Scene.h"
#include "objects/water/Water.h"
#include "objects/water/WaterOscillator.h"
#include "objects/terrain/MountainTerrain.h"
#include "objects/water/WaterLimits.h"
#include "objects/snow/Snow.h"
#include "objects/boid/Boid.h"
#include "objects/bear/Bear.h"
#include <string>
#include <vector>
#include <src/objects/forest/Forest.h>
#include <src/objects/companion/CompanionFollower.h>
#include <src/objects/companion/OneWayCompanion.h>
#include <src/objects/dome/Dome.h>
#include <src/objects/companion/ObjectFollowerCompanion.h>
#include <src/objects/bear/BearFilmingCompanion.h>

std::shared_ptr<BaseTerrain> addTerrain(Scene &scene) {
    // Adding terrain
    WaterLimits waterLimits(-100, 100, -35, 135, -30, -10);
    auto terrain = std::make_shared<MountainTerrain>(scene.getShaders(), 280, 280, waterLimits);
    auto terrainObj = std::static_pointer_cast<Object>(terrain);
    scene.addObject(terrainObj);

    // Adding water to the scene
    auto oscillators = std::make_shared<std::vector<WaterOscillator>>();
    oscillators->emplace_back(vcl::vec2(0.0f, 50.0f), 0.1f, 0.01, 1, 1, waterLimits.getWaterLevel(), false);
    oscillators->emplace_back(vcl::vec2(20.0f, 50.0f - 40 * sqrt(3) / 6), 0.01f, 0.01, 10, 1,
                             waterLimits.getWaterLevel(), false);
    oscillators->emplace_back(vcl::vec2(-20.0f, 50.0f - 40 * sqrt(3) / 6), 0.01f, 0.01, 10, 1,
                             waterLimits.getWaterLevel(), false);
    oscillators->emplace_back(vcl::vec2(0.0f, 50.0f + 40 * sqrt(3) / 3), 0.01f, 0.01, 10, 1, waterLimits.getWaterLevel(),
                             false);
    auto water = std::static_pointer_cast<Object>(std::make_shared<Water>(scene.getShaders(),
                                                                          waterLimits, oscillators));
    scene.addObject(water);

    return std::static_pointer_cast<BaseTerrain>(terrain);
}

std::shared_ptr<Bear> addBear(Scene &scene, std::shared_ptr<BaseTerrain> &terrain, std::shared_ptr<Forest> &forest) {
    // Cyclic spline
    // Adding positions
    std::vector<vcl::vec3> keyframes;
    keyframes.emplace_back(0, -85, 10);
    keyframes.emplace_back(70, -100, 10);
    keyframes.emplace_back(100, -70, 10);
    keyframes.emplace_back(100, -50, 10);

    auto spline = std::make_shared<vcl::CyclicCardinalSpline>((float) keyframes.size());

    // Drawing trajectory
    //Adding positions to spline
    for (int i = 0; i < (int) keyframes.size(); ++i)
        spline->addKeyFrame(keyframes[i], (float) i);

//    // Trajectory
//    const float dt = 0.01;
//    float t = 0;
//    float tf = (float) keyframes.size();
//    auto traj = std::make_shared<std::vector<vcl::vec3>>();
//    while (t <= tf) {
//        traj->push_back(spline->position(t));
//        t += dt;
//    }
//    auto drawer = std::static_pointer_cast<Object>(std::make_shared<TrajectoryDrawer>(scene.getShaders(), traj));
//    scene.addObject(drawer);

    // Bear animation part
    auto bearPos = std::make_shared<vcl::vec3>(0, -85, 0);

    // Adding bear companion
    auto companion = std::static_pointer_cast<Object>(std::make_shared<BearWalkingCompanion>(
            scene.getShaders(), spline, 0.0f, bearPos, false));
    scene.addObject(companion);

    std::shared_ptr<BearWalkingCompanion> bearCompanionPtr = std::static_pointer_cast<BearWalkingCompanion>(companion);

    // Creating bear
    auto bear = std::make_shared<Bear>(scene.getShaders(), terrain, bearCompanionPtr, bearPos, forest);
    auto bearObj = std::static_pointer_cast<Object>(bear);
    scene.addObject(bearObj);

    return bear;
}

void configureCamera(Scene &scene, std::shared_ptr<Object> &bird, std::shared_ptr<Object> &bearPtr) {
    scene.getGui()->getCamera().set_scale(5);
    std::shared_ptr<Bear> bear = std::static_pointer_cast<Bear>(bearPtr);

    // Spline
    // Adding positions
    std::vector<std::vector<vcl::vec3>> keyframes;
    keyframes.push_back({{135, 135, 2}, {100, 100, 10}, {0, 80, -5}, {-80, 80, 10}, {-90, 0, 10},
                         {-85, -85, 75}, {0, -85, 10}, {70, -100, 5}, {100, -70, 2}, {100, -50, 2}, {0, 0, 2}});
    keyframes.push_back({{60, 135, -2}, {50, 100, -8}, {50, 50, -8}, {30, 10, -8}, {0, 20, -8}, {-50, 20, -8},
                         {-50, 20, -8}});

//    auto keyDrawer = std::static_pointer_cast<Object>(std::make_shared<KeyframesDrawer>(scene.getShaders(), keyframes));
//    scene.addObject(keyDrawer);

    auto spline1 = std::make_shared<vcl::CardinalSpline>();
    for (int i = 0; i < (int) keyframes[0].size(); ++i)
        spline1->addKeyFrame(keyframes[0][i], (float) i);

    // Adding companion
    auto companion1 = std::static_pointer_cast<Object>(std::make_shared<OneWayCompanion>(
            scene.getShaders(), spline1, 0.0f, false, false));
    auto companion1Ptr = std::static_pointer_cast<DipoleCompanion>(companion1);
    scene.addObject(companion1);

    auto spline2 = std::make_shared<vcl::CardinalSpline>();
    for (int i = 0; i < (int) keyframes[1].size(); ++i)
        spline2->addKeyFrame(keyframes[1][i], (float) i);

    // Adding companion
    auto companion2 = std::static_pointer_cast<Object>(std::make_shared<OneWayCompanion>(
            scene.getShaders(), spline2, 0.0f, false, false));
    auto companion2Ptr = std::static_pointer_cast<DipoleCompanion>(companion2);
    scene.addObject(companion2);

    // Adding companion
    auto companion3 = std::static_pointer_cast<Object>
            (std::make_shared<ObjectFollowerCompanion>(scene.getShaders(), bird, false));
    std::shared_ptr<DipoleCompanion> companion3Ptr = std::static_pointer_cast<DipoleCompanion>(companion3);
    scene.addObject(companion3);

    // Adding companion
    auto companion4 = std::static_pointer_cast<Object>
            (std::make_shared<BearFilmingCompanion>(scene.getShaders(), bearPtr, false));
    std::shared_ptr<DipoleCompanion> companion4Ptr = std::static_pointer_cast<DipoleCompanion>(companion4);
    scene.addObject(companion4);

//    // Trajectory
//    const float dt = 0.01;
//    float t = 0;
//    float tf = (float) keyframes.size() - 1;
//    auto traj1 = std::make_shared<std::vector<vcl::vec3>>();
//    while (t <= tf) {
//        traj1->push_back(spline1->position(t));
//        t += dt;
//    }
//    auto drawer = std::static_pointer_cast<Object>(std::make_shared<TrajectoryDrawer>(scene.getShaders(), traj));
//    scene.addObject(drawer);

    // Adding companion follower to the scene
    auto companions = std::make_shared<std::vector<std::shared_ptr<DipoleCompanion>>>();

    companions->emplace_back(companion1Ptr);
    companions->emplace_back(companion2Ptr);
    companions->emplace_back(companion3Ptr);
    companions->emplace_back(companion4Ptr);

    auto transitionTimes = std::make_shared<std::vector<float>>();
    transitionTimes->emplace_back(150);
    transitionTimes->emplace_back(250);
    transitionTimes->emplace_back(330);

    auto follower = std::static_pointer_cast<Object>(std::make_shared<CompanionFollower>(
            scene.getShaders(), companions, transitionTimes, keyframes[0][0], false));
    scene.addObject(follower);

    scene.haveCameraFollow(follower);
}

int main() {
    std::string windowTitle = "Main";
    auto scene = Scene::getInstance(windowTitle);

    // Adding terrain
    auto terrain = addTerrain(scene);

    // Adding dome
    auto dome = std::static_pointer_cast<Object>(std::make_shared<Dome>(scene.getShaders()));
    scene.addObject(dome);

    // Adding forest
    auto forest = std::make_shared<Forest>(scene.getShaders(), terrain);
    auto forestObj = std::static_pointer_cast<Object>(forest);
    scene.addObject(forestObj);

    // Adding snow
    auto snow = std::static_pointer_cast<Object>(std::make_shared<Snow>(scene.getShaders()));
    scene.addObject(snow);

    // Adding bear
    std::shared_ptr<Object> bear = addBear(scene, terrain, forest);

    // Adding boid
    auto boid = std::make_shared<Boid>(scene.getShaders(), 20, -140, 140, -140, 140, -10, 100, terrain);
    auto boidObj = std::static_pointer_cast<Object>(boid);
    scene.addObject(boidObj);

    // Adding camera options
    configureCamera(scene, boid->getBird(0), bear);

    scene.display();
    return 0;
}
