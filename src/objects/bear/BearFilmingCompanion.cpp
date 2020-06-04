//
// Created by Aloysio Galvão Lopes on 2020-06-03.
//

#include "BearFilmingCompanion.h"

BearFilmingCompanion::BearFilmingCompanion(Shaders &shaders, std::shared_ptr<Object> &obj, bool debug)
        : ObjectFollowerCompanion(shaders, obj, debug) {}

void BearFilmingCompanion::update(float time) {
    vcl::vec3 circlePosition = {sin(time*Constants::TIME_FACTOR), cos(time*Constants::TIME_FACTOR), 0.0f};
    position = obj->getPosition()+vcl::vec3(0, 0, Constants::FLOATING_MARGIN)+
            vcl::vec3()+circlePosition*Constants::FLOATING_RADIUS;

    dp = obj->getPosition()-position;

    orientation = vcl::rotation_euler(dp, 0.0f);

    updateChargesPositions();
}
