//
// Created by igor on 25/04/2020.
//

#include "Box.h"

Box::Box(Shaders &shaders, vcl::vec3 base) : Object(false) {
    auto mesh = vcl::mesh_primitive_parallelepiped({base.x - 0.5f, base.y - 0.5f, 0});
    boundingBox = BoundingBox(base.x - 0.5, base.x + 0.5, base.y - 0.5,
                              base.y + 0.5, base.z, base.z + 1.0);
    box = vcl::mesh_drawable(mesh);
    box.shader = shaders["mesh"];
}

void Box::draw(const vcl::camera_scene &camera) {
    box.uniform.light = *light;
    vcl::draw(box, camera);
}