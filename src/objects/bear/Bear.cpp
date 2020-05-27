//
// Created by igor on 15/05/2020.
//

#include "Bear.h"

Bear::Bear(Shaders &shaders, vcl::vec3 base) :
        Object(true), bear("../src/assets/models/bear.fbx", shaders["mesh"], base) {
    boundingSphere = BoundingSphere(bear.meshes); // TODO: check bounding sphere
    bear.set_animation("bear|run");
}

void Bear::drawMesh(vcl::camera_scene &camera, float time) {
    bear.set_light(lights[0]);
    bear.draw(camera, time);
}