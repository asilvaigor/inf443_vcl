//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#include <src/objects/marker/CoordinateMarker.h>
#include <src/objects/debug/DebugObject.h>
#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/boid/Boid.h"

int main() {
    std::string windowTitle = "Boid Test";
    auto scene = Scene::getInstance(windowTitle);
    vcl::camera_scene &camera = scene.getGui()->getCamera();

    // Camera settings
    camera.translation = {-50, -50, -40};

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
    scene.getGui()->setShowVertices(true);

    // Adding boid to the scene
    auto boid = std::static_pointer_cast<Object>(std::make_shared<Boid>
            (scene.getShaders(), 10, 0, 100, 0, 100, 0, 100));

    scene.addObject(boid);

    // Adding marker to origin
    auto marker = std::static_pointer_cast<Object>(std::make_shared<CoordinateMarker>(scene.getShaders()));
    scene.addObject(marker);

    // Visualizing the birds limits
    auto debugObject = std::static_pointer_cast<Object>(std::make_shared<DebugObject>(scene.getShaders()));
    auto debugObjectPtr = std::static_pointer_cast<DebugObject>(debugObject);

    vcl::vec3 p0 = {0, 0, 0};
    vcl::mesh_drawable boundingBox =
            mesh_primitive_parallelepiped(p0,vcl::vec3(100, 0, 0)-p0, vcl::vec3(0, 100, 0)-p0, vcl::vec3(0, 0, 100)-p0);

    debugObjectPtr->addMesh(boundingBox);
    scene.addObject(debugObject);

    scene.display();
    return 0;
}
