//
// Created by igor on 15/05/2020.
//

#include "Bear.h"

Bear::Bear(Shaders &shaders, vcl::vec3 base) :
        Object(true), bear("../src/assets/models/bear.fbx", shaders["mesh_skinned"], base) {
    boundingSphere = BoundingSphere(bear.meshes);
    bear.set_animation("bear|run");
}

void Bear::drawMesh(vcl::camera_scene &camera, float time) {
    bear.set_light(light);
    bear.draw(camera, time);
}