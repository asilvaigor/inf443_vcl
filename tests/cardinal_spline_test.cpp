//
// Created by Aloysio Galvão Lopes on 2020-06-01.
//

#include <vcl/math/spline/CardinalSpline.h>
#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/marker/CoordinateMarker.h"
#include "objects/debug/DebugObject.h"
#include "objects/debug/TrajectoryDrawer.h"

int main() {
    auto scene = Scene::getInstance("Spline Test");
    vcl::camera_scene & camera = scene.getGui()->getCamera();

    // Camera settings
    camera.translation = {0, 0, 0};

    float phi = M_PI_2-M_PI_4/2;
    float theta = M_PI_2+M_PI_4;

    const vcl::mat3 Rx = {1,0,0,
                          0,std::cos(phi),-std::sin(phi),
                          0,std::sin(phi),std::cos(phi)};

    const vcl::mat3 Rz = {std::cos(theta),-std::sin(theta),0,
                          std::sin(theta),std::cos(theta),0,
                          0,0,1};


    camera.orientation = Rz*Rx;
    camera.scale = 30;
    scene.getGui()->setShowVertices(true);

    // Adding marker to origin
    auto marker = std::static_pointer_cast<Object>(std::make_shared<CoordinateMarker>(scene.getShaders()));
    scene.addObject(marker);

    // Adding positions
    vcl::CardinalSpline spline;
    std::vector<vcl::vec3> keyframes;
    keyframes.emplace_back(10, 0, 10);
    keyframes.emplace_back(10, 10, 0);
    keyframes.emplace_back(0, 10, 10);
    keyframes.emplace_back(0, 0, 0);

    //Adding positions to spline
    spline.addKeyFrame(keyframes[0], 0);
    spline.addKeyFrame(keyframes[1], 2);
    spline.addKeyFrame(keyframes[2], 1);
    spline.addKeyFrame(keyframes[3], 4);

    // Adding trajectory lines
    auto drawer = std::static_pointer_cast<Object>(std::make_shared<TrajectoryDrawer>(scene.getShaders(), keyframes));
//    scene.addObject(drawer);

    // Trajectory
    const float dt = 0.01;
    float t = 2;
    float tf = 4;
    std::vector<vcl::vec3 > traj;
    while (t < tf){
        traj.push_back(spline.position(t));
        t+= dt;
        std::cout << t << "\n";
    }

    auto drawer2 = std::static_pointer_cast<Object>(std::make_shared<TrajectoryDrawer>(scene.getShaders(), traj));
    scene.addObject(drawer2);

    // Printing positions
    auto debugObject = std::static_pointer_cast<Object>(std::make_shared<DebugObject>(scene.getShaders()));
    auto debugObjectPtr = std::static_pointer_cast<DebugObject>(debugObject);
    debugObjectPtr->addMesh(vcl::mesh_primitive_sphere(1.0, keyframes[0]));
    debugObjectPtr->addMesh(vcl::mesh_primitive_sphere(1.0, keyframes[1]));
    debugObjectPtr->addMesh(vcl::mesh_primitive_sphere(1.0, keyframes[2]));
    debugObjectPtr->addMesh(vcl::mesh_primitive_sphere(1.0, keyframes[3]));
    scene.addObject(debugObject);

    scene.display();
    return 0;
}

