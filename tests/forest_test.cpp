//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/terrain/MountainTerrain.h"
#include "objects/forest/Forest.h"

int main() {
    std::string windowTitle = "Forest Test";
    auto scene = Scene::getInstance(windowTitle);
    vcl::camera_scene & camera = scene.getGui()->getCamera();

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
    WaterLimits waterLimits(-100, 100, -35, 135, -30, -10);
    auto terrain = std::make_shared<MountainTerrain>(scene.getShaders(), 280, 280, waterLimits);
    auto terrainBase = std::static_pointer_cast<BaseTerrain>(terrain);
    auto terrainObj = std::static_pointer_cast<Object>(terrain);
    auto forest = std::static_pointer_cast<Object>(std::make_shared<Forest>(scene.getShaders(), terrainBase));

    scene.addObject(terrainObj);
    scene.addObject(forest);

    scene.display();
    return 0;
}
