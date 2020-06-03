//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#include <src/objects/marker/CoordinateMarker.h>
#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/water/Water.h"
#include "objects/water/WaterOscillator.h"
#include "objects/terrain/MountainTerrain.h"
#include "objects/water/WaterLimits.h"
#include <string>
#include <vector>
#include <src/objects/debug/DebugObject.h>
#include <src/objects/forest/Forest.h>

int main() {
    std::string windowTitle = "Scene Integration Test";
    auto scene = Scene::getInstance(windowTitle);
    vcl::camera_scene &camera = scene.getGui()->getCamera();

    // Camera settings
    camera.translation = {-50, -50, 0};

    float phi = M_PI_2-M_PI_4/2;
    float theta = M_PI_2+M_PI_4;

    const vcl::mat3 Rx = {1,0,0,
                          0,std::cos(phi),-std::sin(phi),
                          0,std::sin(phi),std::cos(phi)};

    const vcl::mat3 Rz = {std::cos(theta),-std::sin(theta),0,
                          std::sin(theta),std::cos(theta),0,
                          0,0,1};


    camera.orientation = Rz*Rx;
    camera.scale = 200;

    // Adding terrain
    WaterLimits waterLimits(-100, 100, -35, 135, -30, -10);
    auto terrain = std::make_shared<MountainTerrain>(scene.getShaders(), 280, 280, waterLimits);
    auto terrainPt = std::static_pointer_cast<Object>(terrain);

    scene.addObject(terrainPt);

    // Adding water to the scene
    std::vector<WaterOscillator> oscillators;
    oscillators.emplace_back(vcl::vec2(0.0f, 50.0f), 0.1f, 0.01, 1, 1, waterLimits.getWaterLevel(), false);
    oscillators.emplace_back(vcl::vec2(20.0f, 50.0f-40*sqrt(3)/6), 0.01f, 0.01, 10, 1, waterLimits.getWaterLevel(), false);
    oscillators.emplace_back(vcl::vec2(-20.0f, 50.0f-40*sqrt(3)/6), 0.01f, 0.01, 10, 1, waterLimits.getWaterLevel(), false);
    oscillators.emplace_back(vcl::vec2(0.0f, 50.0f+40*sqrt(3)/3), 0.01f, 0.01, 10, 1, waterLimits.getWaterLevel(), false);

    auto water = std::static_pointer_cast<Object>(std::make_shared<Water>(scene.getShaders(),
                                                                          waterLimits,oscillators));
    scene.addObject(water);

    // Adding forest
    auto terrainBase = std::static_pointer_cast<BaseTerrain>(terrain);
    auto forest = std::static_pointer_cast<Object>(std::make_shared<Forest>(scene.getShaders(), terrainBase));
    scene.addObject(forest);

    scene.display();
    return 0;
}

