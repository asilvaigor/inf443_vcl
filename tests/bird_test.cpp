//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#include <src/objects/marker/CoordinateMarker.h>
#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/boid/Bird.h"

int main() {
    std::string windowTitle = "Bird Test";
    auto scene = Scene::getInstance(windowTitle);
    vcl::camera_scene &camera = scene.getGui()->getCamera();

    auto bird = std::static_pointer_cast<Object>(std::make_shared<Bird>
            (scene.getShaders(), vcl::vec3(0, 0, 0), 1, vcl::vec3(-1,-1,0)));

    scene.addObject(bird);

    // Adding marker to origin
    auto marker = std::static_pointer_cast<Object>(std::make_shared<CoordinateMarker>(scene.getShaders()));

    scene.addObject(marker);

    scene.display();
    return 0;
}
