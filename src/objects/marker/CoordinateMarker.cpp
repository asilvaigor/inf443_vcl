//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#include "CoordinateMarker.h"

CoordinateMarker::CoordinateMarker(Shaders &shaders, vcl::vec3 position) : Object(false), position(position){
    marker = vcl::mesh_drawable(vcl::mesh_primitive_frame());
    marker.shader = shaders["mesh"];
}

void CoordinateMarker::drawMesh(vcl::camera_scene &camera) {
    vcl::draw(marker, camera);
}

void CoordinateMarker::setPosition(vcl::vec3 position) {
    this->position = position;
}
