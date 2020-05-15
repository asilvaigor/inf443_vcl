//
// Created by igor on 23/04/2020.
//

#include "Object.h"
#include "utils/Constants.h"

Object::Object(bool movable) : movable(movable) {
    light = std::make_shared<vcl::light_source>(vcl::vec3(500, 0, 500), vcl::vec3(-1, 0, -1));
}

void Object::draw(vcl::camera_scene &camera, float time) {
    if (boundingSphere.isInCameraFrustum(camera)) {
        if (boundingBox.relativeSize(camera) > Constants::BILLBOARD_RATIO_THRESHOLD || billboard.empty())
            drawMesh(camera, time);
        else billboard.draw(camera, light);
    }
}

void Object::setLight(std::shared_ptr<vcl::light_source> &l) {
    light = l;
}

std::shared_ptr<vcl::light_source> & Object::getLight() {
    return light;
}

bool Object::isMovable() const {
    return movable;
}

BoundingSphere &Object::getBoundingSphere() {
    return boundingSphere;
}

BoundingBox &Object::getBoundingBox() {
    return boundingBox;
}