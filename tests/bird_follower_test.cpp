//
// Created by Aloysio Galvão Lopes on 2020-05-31.
//

#include <src/objects/marker/CoordinateMarker.h>
#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/terrain/MountainTerrain.h"
#include <string>
#include <vector>
#include <src/objects/debug/DebugObject.h>
#include <src/objects/boid/Boid.h>

int main() {
    std::string windowTitle = "Lake Test";
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
    scene.getGui()->setShowVertices(true);

    WaterLimits waterLimits(-100, 100, -35, 135, -30, -10);
    auto terrain = std::static_pointer_cast<Object>(std::make_shared<MountainTerrain>(scene.getShaders(), 280, 280,
                                                                                      waterLimits));
    auto terrainPtr = std::static_pointer_cast<MountainTerrain>(terrain);
    scene.addObject(terrain);

    // Visualizing birds limits
    auto debugObject = std::static_pointer_cast<Object>(std::make_shared<DebugObject>(scene.getShaders()));
    auto debugObjectPtr = std::static_pointer_cast<DebugObject>(debugObject);

    vcl::vec3 p0 = {-140, -140, -20};
    vcl::mesh_drawable boundingBox =
            mesh_primitive_parallelepiped(p0,vcl::vec3(140, -140, -20)-p0, vcl::vec3(-140, 140, -20)-p0,
                    vcl::vec3(-140, -140, 100)-p0);

    debugObjectPtr->addMesh(boundingBox);
//    scene.addObject(debugObject);

    // Adding boid to the scene
    auto boid = std::make_shared<Boid>(scene.getShaders(), 20, -140, 140, -140, 140, -10, 100, terrainPtr);
    auto boidObj = std::static_pointer_cast<Object>(boid);

    scene.addObject(boidObj);

    scene.haveCameraFollow(boid->getBird(0));

    scene.display();
    return 0;
}
