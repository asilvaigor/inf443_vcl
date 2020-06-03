//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#include "utils/Texture.h"
#include "Bird.h"

std::vector<GLuint> Bird::textures;

Bird::Bird(Shaders &shaders, vcl::vec3 pos, float scale, vcl::vec3 speed, float turning) :
    Object(true), dp(speed), turining(turning),
    bird("../src/assets/models/bird.fbx", shaders["mesh"]) {
    position = pos;
    if (textures.empty()) {
        Texture feathers("bird");
        textures.push_back(feathers.getId());
    }
    bird.set_textures(textures);
    bird.set_animation("bird|fly");
    animationTime = 0;
}

void Bird::drawMesh(vcl::camera_scene &camera) {
    bird.set_light(lights[0]);

    // TODO add scaling to birds
    // TODO Add bounding sphere

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

    vcl::mat3 cRotation = vcl::rotation_from_axis_angle_mat3(dp, turining);

    orientation = cRotation*bRotation*aRotation;
    vcl::mat4 transform = {orientation, position};

    // Bird draw
    bird.transform(transform);
    bird.draw(camera, animationTime);
}

void Bird::update(float time) {
    vcl::vec3 projDpo = {odp.x, odp.y, 1};
    vcl::vec3 projNdp = {ndp.x, ndp.y, 1};
    float targetAngle = projDpo.angle(projNdp)*maxTurnFactor;
    if (targetAngle > M_PI_2)
        targetAngle = M_PI_2;
    else if (targetAngle < -M_PI_2)
        targetAngle = -M_PI_2;
    turining += turnFactor*(targetAngle-turining);

    // Animation part

    float animationNormalizedPosition = fmod(animationTime, animationMaxTime);
    float animationTargetPosition = animationNormalizedPosition+animationSpeed;
    float animationSpeedFactor = 1.0f;
    float inclination = dp.angle({0,0,1});
    if (inclination >= M_PI_2+M_PI_4){
        animationTargetPosition = 0.6;
    } else {
        animationSpeedFactor = (M_PI_2+M_PI_4-inclination)/(M_PI_2+M_PI_4)*0.6+0.4;
        animationSpeedFactor += (fabs(turining))/M_PI_2;
    }

    if (fabs(animationNormalizedPosition-animationTargetPosition) >= animationSpeed/2)
    animationTime += animationSpeed*animationSpeedFactor;
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

void Bird::setPosition(vcl::vec3 pos) {
    position = pos;
}

void Bird::stepSpeed() {
    odp = dp;
    dp = ndp;
}

void Bird::stepPosition() {
    position += dp;
}

float Bird::getRotation() {
    return turining;
}
