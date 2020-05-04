//
// Created by igor on 25/04/2020.
//

#include "Box.h"

Box::Box(Shaders &shaders, vcl::vec3 base) : Object(false) {
    auto mesh = vcl::mesh_primitive_parallelepiped({base.x - 0.5f, base.y - 0.5f, 0});
    boundingSphere = BoundingSphere(mesh);
    box = vcl::mesh_drawable(mesh);
    box.shader = shaders["mesh"];
}

void Box::draw(vcl::camera_scene &camera) {
    if (boundingSphere.isInCameraFrustum(camera)) {
        box.uniform.light = light;
        vcl::draw(box, camera);
    }
}