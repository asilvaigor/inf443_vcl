#include <utility>

//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#include <src/scene/Scene.h>
#include "Boid.h"

Boid::Boid(Shaders &shaders, int birdCount, float minX, float maxX, float minY, float maxY, float minZ, float maxZ,
           std::shared_ptr<BaseTerrain> terrain)
        : Object(true),  minX(minX), maxX(maxX), maxY(maxY), minY(minY), minZ(minZ), maxZ(maxZ),
        generator(Scene::deterministic), terrain(std::move(terrain)){

    for (int i = 0; i < birdCount; ++i){
        vcl::vec3 pos = {generator.rand(minX, maxX), generator.rand(minY, maxY), generator.rand(minZ, maxZ)};
        float scale = generator.rand(Constants::BIRD_MIN_SCALE, Constants::BIRD_MAX_SCALE);
        vcl::vec3 speed = {generator.rand(), generator.rand(), generator.rand(-0.2, 0.2)};

        birds.push_back(std::make_shared<Bird>(shaders, pos, scale, speed));
    }
}

void Boid::drawMesh(vcl::camera_scene &camera) {
    throw std::invalid_argument("Boid drawMesh should not be called because of bounding spheres. "
                                "Draw each of the objects separately.");
}

std::vector<std::shared_ptr<Object>> &Boid::getObjects() {
    return birds;
}

void Boid::update(float time) {
    for (auto& obj : birds){
        auto bird = dynamic_cast<Bird *>(obj.get());

        birdFlyCenter(bird);
        birdAvoidOthers(bird);
        birdMatchOthers(bird);
        birdBound(bird);
        birdLimitSpeed(bird);
    }
    for (auto& obj : birds){
        auto bird = dynamic_cast<Bird *>(obj.get());
        bird->stepSpeed();
        bird->stepPosition();
        bird->update(time);
    }
}

void Boid::birdFlyCenter(Bird* bird) {
    vcl::vec3 center = {0.0f, 0.0f, 0.0f};
    auto numBirds = (float) birds.size();

    for (auto& obj : birds){
        auto otherBird = dynamic_cast<Bird *>(obj.get());
        if (vcl::norm(bird->getPosition()-otherBird->getPosition()) < Constants::VISUAL_RANGE)
            center += otherBird->getPosition();
    }

    center /= numBirds;
    bird->addFutureSpeed((center - bird->getPosition()) * Constants::CENTERING_FACTOR);
    bird->addFutureSpeed({0.0f, 0.0f, -bird->getFutureSpeed().z*0.01f});
}

void Boid::birdLimitSpeed(Bird *bird) {
    float speed = norm(bird->getFutureSpeed());

    if (speed > Constants::BIRD_MAX_SPEED_LIMIT){
        bird->setFutureSpeed(bird->getFutureSpeed() / speed * Constants::BIRD_MAX_SPEED_LIMIT);
    } else if (speed < Constants::BIRD_MIN_SPEED_LIMIT){
        bird->setFutureSpeed(bird->getFutureSpeed() / speed * Constants::BIRD_MIN_SPEED_LIMIT);
    }
}

void Boid::birdAvoidOthers(Bird *bird) {
    vcl::vec3 move = {0.0f, 0.0f, 0.0f};

    for (auto &obj : birds) {
        auto otherBird = dynamic_cast<Bird *>(obj.get());
        vcl::vec3 dist = bird->getPosition() - otherBird->getPosition();
        if (otherBird != bird && dist.norm() < Constants::MIN_DISTANCE)
            move += dist;
    }

    bird->addFutureSpeed(move * Constants::AVOID_FACTOR);
}

void Boid::birdMatchOthers(Bird *bird){
    vcl::vec3 avgDp = {0.0f, 0.0f, 0.0f};
    auto numBirds = (float) birds.size();

    for (auto &obj : birds) {
        auto otherBird = dynamic_cast<Bird *>(obj.get());
        if (vcl::norm(bird->getPosition()-otherBird->getPosition()) < Constants::VISUAL_RANGE)
            avgDp += otherBird->getSpeed();
    }

    avgDp /= numBirds;

    bird->addFutureSpeed((avgDp-bird->getSpeed()) * Constants::MATCHING_FACTOR);
}

void Boid::birdBound(Bird *bird) {
    float dist;
    if (bird->getPosition().x <= minX){
        dist = minX-bird->getPosition().x;
        bird->addFutureSpeed({dist*Constants::TURN_FACTOR, 0.0f, 0.0f});
    }
    else if (bird->getPosition().x >= maxX){
        dist = bird->getPosition().x-maxX;
        bird->addFutureSpeed({-dist*Constants::TURN_FACTOR, 0.0f, 0.0f});
    }
    if (bird->getPosition().y <= minY){
        dist = minY-bird->getPosition().y;
        bird->addFutureSpeed({0.0f, dist*Constants::TURN_FACTOR, 0.0f});
    }
    else if (bird->getPosition().y >= maxY){
        dist = bird->getPosition().y-maxY;
        bird->addFutureSpeed({0.0f, -dist*Constants::TURN_FACTOR, 0.0f});
    }
    if (bird->getPosition().z <= minZ){
        dist = minZ-bird->getPosition().z;
        bird->addFutureSpeed({0.0f, 0.0f, dist*Constants::TURN_FACTOR});
    }

    float vertDist = maxZ - bird->getPosition().z;
    vertDist = vertDist*vertDist;

    bird->addFutureSpeed({0.0f, 0.0f, -Constants::TURN_FACTOR/vertDist});
    if (terrain != nullptr &&
    bird->getPosition().z <= Constants::TERRAIN_MARGIN+
    terrain->getTerrainHeight(bird->getPosition().x, bird->getPosition().y)){
        dist = Constants::TERRAIN_MARGIN+terrain->getTerrainHeight(bird->getPosition().x, bird->getPosition().y)
                - bird->getPosition().z;
        bird->addFutureSpeed({0.0f, 0.0f, dist*Constants::TURN_FACTOR});
    }
}

std::shared_ptr<Object> & Boid::getBird(int idx) {
    return birds[idx];
}
