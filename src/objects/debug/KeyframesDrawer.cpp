//
// Created by igor on 02/06/2020.
//

#include "KeyframesDrawer.h"

KeyframesDrawer::KeyframesDrawer(Shaders &shaders, std::vector<vcl::vec3> &keyframes) :
        Object(false), keyframes(keyframes) {
    for (int i = 0; i < (int) keyframes.size(); i++) {
        vcl::mesh m = vcl::mesh_primitive_sphere(1.0f, keyframes[i]);
        drawables.push_back(m);
        drawables[i].uniform.color = {1.0f, 0.0f, 0.0f};
        drawables[i].shader = shaders["mesh"];
    }
}

void KeyframesDrawer::drawMesh(vcl::camera_scene &camera) {
    for (int i = 0; i < (int) keyframes.size(); i++) {
        drawables[i].uniform.lights = lights;
        vcl::draw(drawables[i], camera);
    }
}
