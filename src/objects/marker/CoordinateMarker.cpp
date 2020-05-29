//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#include "CoordinateMarker.h"

CoordinateMarker::CoordinateMarker(Shaders &shaders) : Object(false){
    marker = vcl::mesh_drawable(vcl::mesh_primitive_frame());
    marker.shader = shaders["mesh"];
}

void CoordinateMarker::drawMesh(vcl::camera_scene &camera, float) {
    vcl::draw(marker, camera);
}
