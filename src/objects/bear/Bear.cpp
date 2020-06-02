//
// Created by igor on 15/05/2020.
//

#include "utils/Texture.h"
#include "Bear.h"
#include "BearCompanion.h"

std::vector<GLuint> Bear::textures;

Bear::Bear(Shaders &shaders, std::shared_ptr<BaseTerrain> &terrain, std::shared_ptr<BearCompanion> &companion,
           std::shared_ptr<vcl::vec3> &pos) :
        Object(true), bear("../src/assets/models/bear.fbx", shaders["mesh"]), terrain(terrain),
        companion(companion), pos(pos){
    if (textures.empty()) {
        Texture fur("bear_fur");
        Texture nose(0, 0, 0);
        Texture eyes("bear_eyes");
        textures.push_back(fur.getId());
        textures.push_back(nose.getId());
        textures.push_back(eyes.getId());
    }

    bear.set_textures(textures);
    bear.set_animation("bear|walk");
    boundingSphereRadius = 1.2f;
    boundingSphere = BoundingSphere(*pos, boundingSphereRadius);
    position = *pos;
    direction = {0, 1, 0};
    deltaZ = 1.05f;
    speed = 1.5f;
    speedVar = 0.75f;
    stepPhi = -0.14f;
    stepPeriod = 0.5f;
    animationTime = 0.0f;
    lastTime = 0.0f;

    //TODO
}

void Bear::drawMesh(vcl::camera_scene &camera, float time) {
    bear.set_light(lights[0]);
    auto transform = updateTransform(time);
    bear.transform(transform);
    animationTime = bear.draw(camera, time);
    lastTime = time;
}

vcl::mat4 Bear::updateTransform(float &time) {
    vcl::vec2 field = companion->getFieldAt({position.x, position.y});
    direction = {field.x, field.y, 0};
    direction = direction.normalized();

    // Calculating floor rotation
    auto floorNormal = terrain->normal(position.x, position.y);

    // Calculating rotations
    // Alpha rotation
    float alpha;
    if (vcl::cross(direction, {0, -1, 0}).z > 0)
        alpha = -direction.angle({0, -1, 0});
    else alpha = direction.angle({0, -1, 0});
    vcl::mat3 aRotation = vcl::rotation_from_axis_angle_mat3({0, 0, 1}, alpha);

    // Beta rotation
    vcl::vec3 bAxis = vcl::cross({0, 0, 1}, direction);
    float anglePerp = -floorNormal.angle(bAxis)+M_PI_2;
    vcl::vec3 perp = vcl::cross(floorNormal, bAxis);
    vcl::vec3 normProj = vcl::rotation_from_axis_angle_mat3(-perp, anglePerp)*floorNormal;

    float angleBeta = -normProj.angle({0, 0, 1});
    if (vcl::dot(bAxis, vcl::cross({0, 0, 1}, normProj)) > 0)
        angleBeta = normProj.angle({0, 0, 1});

    vcl::mat3 bRotation = vcl::rotation_from_axis_angle_mat3(bAxis, angleBeta);

    orientation = bRotation*aRotation;

    // Adjusting position with animation change of height
    vcl::vec3 positionAdjusted = position + floorNormal * deltaZ;
    boundingSphere = BoundingSphere(positionAdjusted, boundingSphereRadius);

    // Updating position with velocity
    auto speedDir = bRotation*direction;
    float speedCur = speed + speedVar * std::sin(((animationTime + stepPhi) / stepPeriod) * 2.0f * (float) M_PI);
    position += speedDir * speedCur * (time - lastTime);
    position.z = terrain->getTerrainHeight(positionAdjusted.x, positionAdjusted.y);
    *pos = position;

    return {orientation, positionAdjusted};
}