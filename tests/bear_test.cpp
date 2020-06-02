//
// Created by igor on 13/05/2020.
//

#include "scene/Scene.h"
#include "objects/bear/Bear.h"
#include "objects/terrain/FlatSurface.h"
#include "vcl/vcl.hpp"
#include "objects/debug/TrajectoryDrawer.h"

int main() {
    auto scene = Scene::getInstance("Bear Test");

    std::shared_ptr<BaseTerrain> terrain = std::make_shared<FlatSurface>(scene.getShaders());
    std::shared_ptr<Object> terrainObj = std::dynamic_pointer_cast<Object>(terrain);
    scene.addObject(terrainObj);

    // Cyclic spline
    // Adding positions
    std::vector<vcl::vec3> keyframes;
    keyframes.emplace_back(50, -100, 5);
    keyframes.emplace_back(50, 50, 5);
    keyframes.emplace_back(0, 50, 5);
    keyframes.emplace_back(0, 0, 5);

    vcl::CyclicCardinalSpline spline((float) keyframes.size());

    // Drawing trajectory
    //Adding positions to spline
    for (int i = 0; i < (int) keyframes.size(); ++i)
        spline.addKeyFrame(keyframes[i], (float) i);

    // Trajectory
    const float dt = 0.01;
    float t = 0;
    float tf = (float) keyframes.size();
    std::vector<vcl::vec3 > traj;
    while (t <= tf){
        traj.push_back(spline.position(t));
        t+= dt;
    }

    auto drawer = std::static_pointer_cast<Object>(std::make_shared<TrajectoryDrawer>(scene.getShaders(), traj));
    scene.addObject(drawer);

    // Bear animation part
    std::shared_ptr<Object> bear;
    std::shared_ptr<vcl::vec3 > bearPos = std::make_shared<vcl::vec3>(0, 0, 0);

    // Adding bear companion
    auto companion = std::static_pointer_cast<Object>(std::make_shared<BearCompanion>(
            scene.getShaders(), spline, 6.0f, bearPos));
    scene.addObject(companion);

    std::shared_ptr<BearCompanion> bearCompanionPtr = std::static_pointer_cast<BearCompanion>(companion);

    bear = std::static_pointer_cast<Object>(
            std::make_shared<Bear>(scene.getShaders(), terrain, bearCompanionPtr, bearPos));
    scene.addObject(bear);

    scene.display();
    return 0;
}
