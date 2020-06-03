//
// Created by Aloysio Galvão Lopes on 2020-06-03.
//

#include "ObjectFollowerCompanion.h"

ObjectFollowerCompanion::ObjectFollowerCompanion(Shaders &shaders, std::shared_ptr<Object>& obj, bool debug) :
    DipoleCompanion(shaders, debug), obj(obj) {}

void ObjectFollowerCompanion::update(float time) {
    position = obj->getPosition();
    orientation = obj->getOrientation();
}