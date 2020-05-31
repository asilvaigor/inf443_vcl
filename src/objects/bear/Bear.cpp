//
// Created by igor on 15/05/2020.
//

#include "Bear.h"

Bear::Bear(Shaders &shaders, std::shared_ptr<BaseTerrain> &terrain, vcl::vec3 pos) :
        Object(true), bear("../src/assets/models/bear.fbx", shaders["mesh"]), terrain(terrain) {
    bear.set_animation("bear|run");
    boundingSphereRadius = 1.2f;
    boundingSphere = BoundingSphere(pos, boundingSphereRadius);
    position = pos;
    direction = {0, -1, 0};
    deltaZ = 0.95f;
    deltaZVar = 0.1f;
    deltaZPhi = 0.0f;
    speed = 2.0f;
    speedVar = 0.5f;
    speedPhi = 0.0f;
    animationTime = 0.0f;
    lastTime = 0.0f;
}

void Bear::drawMesh(vcl::camera_scene &camera, float time) {
    bear.set_light(lights[0]);
    auto transform = updateTransform(time);
    bear.transform(transform);
    animationTime = bear.draw(camera, time);
    lastTime = time;
}

vcl::mat4 Bear::updateTransform(float &time) {
    // Calculating floor rotation
    auto floorNormal = terrain->normal(position.x, position.y);
    float angx = atan2(-floorNormal.y, floorNormal.z);
    float sinx = std::sin(angx);
    float cosx = std::cos(angx);
    float angy = atan2(floorNormal.x, floorNormal.z);
    float siny = std::sin(angy);
    float cosy = std::cos(angy);
    vcl::mat3 floor_rot(cosy, 0, -siny,
                        -sinx * siny, cosx, -sinx * cosy,
                        cosx * siny, sinx, cosx * cosy);

    // Adjusting position with animation change of height
    float deltaZCur = deltaZ + deltaZVar * std::sin((animationTime + deltaZPhi) * 2.0f * (float) M_PI);
    vcl::vec3 positionAdjusted = position + floorNormal * deltaZCur;
    boundingSphere = BoundingSphere(positionAdjusted, boundingSphereRadius);

    // Updating position with velocity
    auto speedDir = floor_rot * direction;
    float speedCur = speed + speedVar * std::sin((animationTime + speedPhi) * 2.0f * (float) M_PI);
    position += speedDir * speedCur * (time - lastTime);
    position.z = terrain->getTerrainHeight(positionAdjusted.x, positionAdjusted.y);

    return {floor_rot, positionAdjusted};
}