//
// Created by igor on 23/04/2020.
//

#include "Object.h"

Object::Object(bool movable) : movable(movable) {}

void Object::setLight(std::shared_ptr<vcl::light_source> &l) {
   light = l;
}

bool Object::isMovable() {
    return movable;
}