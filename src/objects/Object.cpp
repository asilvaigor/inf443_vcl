//
// Created by igor on 23/04/2020.
//

#include "Object.h"

Object::Object(bool movable) : movable(movable) {
    light = std::make_shared<vcl::light_source>();
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

BoundingBox &Object::getBoundingBox() {
    return boundingBox;
}