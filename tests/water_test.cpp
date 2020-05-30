//
// Created by Aloysio Galvão Lopes on 2020-05-29.
//

#include <src/objects/marker/CoordinateMarker.h>
#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/water/Water.h"
#include <string>

int main() {
    std::string windowTitle = "Water Test";
    auto scene = Scene::getInstance(windowTitle);
    vcl::camera_scene & camera = scene.getGui()->getCamera();

//    // Camera settings
//    camera.translation = {-50, -50, 0};
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


//    camera.orientation = Rz*Rx;
//    camera.scale = 200;
    auto water = std::static_pointer_cast<Object>(std::make_shared<Water>(scene.getShaders(), 280, 280));
    auto marker = std::static_pointer_cast<Object>(std::make_shared<CoordinateMarker>(scene.getShaders()));

    scene.addObject(marker);
    scene.addObject(water);

    scene.display();
    return 0;
}