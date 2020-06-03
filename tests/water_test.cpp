//
// Created by Aloysio Galvão Lopes on 2020-05-29.
//

#include <src/objects/marker/CoordinateMarker.h>
#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/water/Water.h"
#include "objects/water/WaterOscillator.h"
#include <string>
#include <vector>

int main() {
    std::string windowTitle = "Water Test";
    auto scene = Scene::getInstance(windowTitle);
    vcl::camera_scene & camera = scene.getGui()->getCamera();

//    // Camera settings
//    camera.set_translation({-50, -50, 0});
//
//    float phi = M_PI_2-M_PI_4/2;
//    float theta = M_PI_2+M_PI_4;
//
//    const vcl::mat3 Rx = {1,0,0,
//                          0,std::cos(phi),-std::sin(phi),
//                          0,std::sin(phi),std::cos(phi)};
//
//    const vcl::mat3 Rz = {std::cos(theta),-std::sin(theta),0,
//                          std::sin(theta),std::cos(theta),0,
//                          0,0,1};


//    camera.set_orientation(Rz*Rx);
//    camera.set_scale(200);
    auto oscillators = std::make_shared<std::vector<WaterOscillator>>();
    oscillators->emplace_back(vcl::vec2(0.0f, 0.0f), 0.1f, 0.01, 1, 1, true);
    oscillators->emplace_back(vcl::vec2(20.0f, 20.0f), 0.01f, 0.01, 10, 1, true);
    oscillators->emplace_back(vcl::vec2(-30.0f, -30.0f), 0.01f, 0.01, 10, 1, true);

    WaterLimits waterLimits(10, 280, 10, 280, 0, 1);

    auto water = std::static_pointer_cast<Object>(std::make_shared<Water>(scene.getShaders(), waterLimits, oscillators));
    auto marker = std::static_pointer_cast<Object>(std::make_shared<CoordinateMarker>(scene.getShaders()));

    scene.addObject(marker);
    scene.addObject(water);

    scene.display();
    return 0;
}