//
// Created by igor on 25/04/2020.
//

#include "Box.h"

Box::Box(Shaders &shaders) : Object(false) {
    auto mesh = vcl::mesh_primitive_parallelepiped({-0.5, -0.5, 0});
    box = vcl::mesh_drawable(mesh);
    box.shader = shaders["mesh"];
}

void Box::draw(const vcl::camera_scene &camera) {
    box.uniform.light = *light;
    vcl::draw(box, camera);
}