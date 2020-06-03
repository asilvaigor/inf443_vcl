//
// Created by Aloysio Galvão Lopes on 2020-06-01.
//

#include <vcl/math/spline/CardinalSpline.h>
#include <vcl/math/spline/CyclicCardinalSpline.h>
#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/marker/CoordinateMarker.h"
#include "objects/debug/DebugObject.h"
#include "objects/debug/TrajectoryDrawer.h"

int main() {
    auto scene = Scene::getInstance("Spline Test");
    vcl::camera_scene & camera = scene.getGui()->getCamera();

    // Camera settings
    camera.set_translation({0, 0, 0});

    float phi = M_PI_2-M_PI_4/2;
    float theta = M_PI_2+M_PI_4;

    const vcl::mat3 Rx = {1,0,0,
                          0,std::cos(phi),-std::sin(phi),
                          0,std::sin(phi),std::cos(phi)};

    const vcl::mat3 Rz = {std::cos(theta),-std::sin(theta),0,
                          std::sin(theta),std::cos(theta),0,
                          0,0,1};


    camera.set_orientation(Rz*Rx);
    camera.set_scale(30);
    scene.getGui()->setShowVertices(true);

    // Adding marker to origin
    auto marker = std::static_pointer_cast<Object>(std::make_shared<CoordinateMarker>(scene.getShaders()));
    scene.addObject(marker);

    // Adding positions
    auto spline = std::make_shared<vcl::CyclicCardinalSpline>(5);
    auto keyframes = std::make_shared<std::vector<vcl::vec3>>();
    keyframes->emplace_back(10, 0, 10);
    keyframes->emplace_back(10, 0, 0);
    keyframes->emplace_back(0, 0, 10);
    keyframes->emplace_back(0, 0, 0);

    //Adding positions to spline
    spline->addKeyFrame((*keyframes)[0], 4);
    spline->addKeyFrame((*keyframes)[1], 2);
    spline->addKeyFrame((*keyframes)[2], 1);
    spline->addKeyFrame((*keyframes)[3], 0);

    // Adding trajectory lines
    auto drawer = std::static_pointer_cast<Object>(std::make_shared<TrajectoryDrawer>(scene.getShaders(), keyframes));
//    scene.addObject(drawer);

    // Trajectory
    const float dt = 0.01;
    float t = 0;
    float tf = 6;
    auto traj = std::make_shared<std::vector<vcl::vec3>>();
    while (t < tf){
        traj->push_back(spline->position(t));
        t+= dt;
    }

    auto drawer2 = std::static_pointer_cast<Object>(std::make_shared<TrajectoryDrawer>(scene.getShaders(), traj));
    scene.addObject(drawer2);

    // Printing positions
    auto debugObject = std::static_pointer_cast<Object>(std::make_shared<DebugObject>(scene.getShaders()));
    auto debugObjectPtr = std::static_pointer_cast<DebugObject>(debugObject);
    debugObjectPtr->addMesh(vcl::mesh_primitive_sphere(1.0, (*keyframes)[0]));
    debugObjectPtr->addMesh(vcl::mesh_primitive_sphere(1.0, (*keyframes)[1]));
    debugObjectPtr->addMesh(vcl::mesh_primitive_sphere(1.0, (*keyframes)[2]));
    debugObjectPtr->addMesh(vcl::mesh_primitive_sphere(1.0, (*keyframes)[3]));
    debugObjectPtr->addMesh(vcl::mesh_primitive_sphere(1.0, spline->position(7.5)));
    scene.addObject(debugObject);

    scene.display();
    return 0;
}

