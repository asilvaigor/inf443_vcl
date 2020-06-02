//
// Created by Aloysio Galvão Lopes on 2020-06-01.
//

#include <vcl/shape/segment/segment.hpp>
#include "TrajectoryDrawer.h"

TrajectoryDrawer::TrajectoryDrawer(Shaders& shaders, std::vector<vcl::vec3> &points) : Object(false),
    points(points), shaders(shaders) {
    segmentDrawer.init();
}

void TrajectoryDrawer::drawMesh(vcl::camera_scene &camera, float time) {
    for (int i = 0; i < (int)points.size()-1; ++i){
        segmentDrawer.uniform_parameter.p1 = points[i];
        segmentDrawer.uniform_parameter.p2 = points[i+1];

        segmentDrawer.draw(shaders["segment_immediate_mode"], camera);
    }
}

