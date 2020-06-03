//
// Created by Aloysio Galvão Lopes on 2020-06-03.
//

#include <src/objects/boid/Bird.h>
#include "ObjectFollowerCompanion.h"

ObjectFollowerCompanion::ObjectFollowerCompanion(Shaders &shaders, std::shared_ptr<Object>& obj, bool debug) :
    DipoleCompanion(shaders, debug), obj(obj) {}

void ObjectFollowerCompanion::update(float time) {
    position = obj->getPosition();
    orientation = obj->getOrientation();
    dp = orientation*vcl::vec3(0, 1, 0);
    Bird* bird = dynamic_cast<Bird* >(obj.get());
    if (bird != nullptr)
        rotation = bird->getRotation();

    updateChargesPositions();
}