//
// Created by igor on 23/04/2020.
//

#include "Object.h"

Object::Object(bool movable) : movable(movable) {}

bool Object::isMovable() {
    return movable;
}