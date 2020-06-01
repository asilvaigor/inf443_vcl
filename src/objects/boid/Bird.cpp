//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#include "utils/Texture.h"
#include "Bird.h"

std::vector<GLuint> Bird::textures;

Bird::Bird(Shaders &shaders, vcl::vec3 pos, float scale, vcl::vec3 speed) : Object(true), p(pos), dp(speed),
    bird("../src/assets/models/bird.fbx", shaders["mesh"]) {
    if (textures.empty()) {
        Texture feathers("bird");
        textures.push_back(feathers.getId());
    }
    bird.set_textures(textures);
    bird.set_animation("bird|fly");
}

void Bird::drawMesh(vcl::camera_scene &camera, float time) {
    bird.set_light(lights[0]);
//    auto transform = updateTransform(time);
//    bear.transform(transform);
    bird.draw(camera, time);
//    lastTime = time;
    
    // Adjusting mesh rotation and angle
//    mesh.uniform.lights = lights;
//    mesh.uniform.transform.rotation = vcl::rotation_between_vector_mat3({0, 0, 1}, dp);
//    mesh.uniform.transform.translation = p;
    ndp = dp;

//    vcl::draw(mesh, camera);
}

vcl::vec3 Bird::getSpeed() {
    return dp;
}

void Bird::setFutureSpeed(vcl::vec3 speed) {
    ndp = speed;
}

void Bird::addFutureSpeed(vcl::vec3 speed) {
    ndp += speed;
}

vcl::vec3 Bird::getFutureSpeed() {
    return ndp;
}

vcl::vec3 Bird::getPosition() {
    return p;
}

void Bird::setPosition(vcl::vec3 pos) {
    p = pos;
}

void Bird::stepSpeed() {
    dp = ndp;
}

void Bird::stepPosition() {
    p += dp;
}
