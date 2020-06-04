//
// Created by Aloysio Galvão Lopes on 2020-06-03.
//

#include "BearFilmingCompanion.h"

BearFilmingCompanion::BearFilmingCompanion(Shaders &shaders, std::shared_ptr<Object> &obj, bool debug)
        : ObjectFollowerCompanion(shaders, obj, debug) {}

void BearFilmingCompanion::update(float time) {
    vcl::vec3 circlePosition = {sin(time*timeFactor), cos(time*timeFactor), 0.0f};
    position = obj->getPosition()+vcl::vec3(0, 0, floatingMargin)+vcl::vec3()+circlePosition*floatingRadius;

    dp = obj->getPosition()-position;

    // Rotation using euler angles
    vcl::vec3 projDpXY = {dp.x, dp.y, 0};

    float alpha;
    if (vcl::cross(projDpXY, {0, 1, 0}).z > 0)
        alpha = -projDpXY.angle({0, 1, 0});
    else alpha = projDpXY.angle({0, 1, 0});
    vcl::mat3 aRotation = vcl::rotation_from_axis_angle_mat3({0, 0, 1}, alpha);

    vcl::vec3 bAxis = vcl::cross(dp, {0, 0, 1});
    float beta = -dp.angle({0, 0, 1})+M_PI_2;
    vcl::mat3 bRotation = vcl::rotation_from_axis_angle_mat3(bAxis, beta);

    orientation = bRotation*aRotation;

    updateChargesPositions();
}
