#include <utility>

//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#include <src/scene/Scene.h>
#include "Boid.h"

Boid::Boid(Shaders &shaders, int birdCount, float minX, float maxX, float minY, float maxY, float minZ, float maxZ,
           std::shared_ptr<MountainTerrain> terrain)
        : Object(true),  minX(minX), maxX(maxX), maxY(maxY), minY(minY), minZ(minZ), maxZ(maxZ),
        generator(Scene::deterministic), terrain(std::move(terrain)){


    // TODO stop hardcoding the scale

    for (int i = 0; i < birdCount; ++i){
        vcl::vec3 pos = {generator.rand(minX, maxX), generator.rand(minY, maxY), generator.rand(minZ, maxZ)};
        float scale = generator.rand(1.0f, 2.0f);
        vcl::vec3 speed = {generator.rand(), generator.rand(), generator.rand(-0.2, 0.2)};

        birds.push_back(std::make_shared<Bird>(shaders, pos, scale, speed));
    }

    // Setting current time
    curTime = 0;
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
    curTime = time;
}

void Boid::birdFlyCenter(Bird* bird) {
    vcl::vec3 center = {0.0f, 0.0f, 0.0f};
    auto numBirds = (float) birds.size();

    for (auto& obj : birds){
        auto otherBird = dynamic_cast<Bird *>(obj.get());
        if (vcl::norm(bird->getPosition()-otherBird->getPosition()) < visualRange)
            center += otherBird->getPosition();
    }

    center /= numBirds;
    bird->addFutureSpeed((center - bird->getPosition()) * centeringFactor);
    bird->addFutureSpeed({0.0f, 0.0f, -bird->getFutureSpeed().z*0.01f});
}

void Boid::birdLimitSpeed(Bird *bird) {
    float speed = norm(bird->getFutureSpeed());

    if (speed > speedMaxLimit){
        bird->setFutureSpeed(bird->getFutureSpeed() / speed * speedMaxLimit);
    } else if (speed < speedMinLimit){
        bird->setFutureSpeed(bird->getFutureSpeed() / speed * speedMinLimit);
    }
}

void Boid::birdAvoidOthers(Bird *bird) {
    vcl::vec3 move = {0.0f, 0.0f, 0.0f};

    for (auto &obj : birds) {
        auto otherBird = dynamic_cast<Bird *>(obj.get());
        vcl::vec3 dist = bird->getPosition() - otherBird->getPosition();
        if (otherBird != bird && dist.norm() < minDistance)
            move += dist;
    }

    bird->addFutureSpeed(move * avoidFactor);
}

void Boid::birdMatchOthers(Bird *bird){
    vcl::vec3 avgDp = {0.0f, 0.0f, 0.0f};
    auto numBirds = (float) birds.size();

    for (auto &obj : birds) {
        auto otherBird = dynamic_cast<Bird *>(obj.get());
        if (vcl::norm(bird->getPosition()-otherBird->getPosition()) < visualRange)
            avgDp += otherBird->getSpeed();
    }

    avgDp /= numBirds;

    bird->addFutureSpeed((avgDp-bird->getSpeed()) * matchingFactor);
}

void Boid::birdBound(Bird *bird) {
    if (bird->getPosition().x <= minX)
        bird->addFutureSpeed({turnFactor, 0.0f, 0.0f});
    if (bird->getPosition().x >= maxX)
        bird->addFutureSpeed({-turnFactor, 0.0f, 0.0f});
    if (bird->getPosition().y <= minY)
        bird->addFutureSpeed({0.0f, turnFactor, 0.0f});
    if (bird->getPosition().y >= maxY)
        bird->addFutureSpeed({0.0f, -turnFactor, 0.0f});
    if (bird->getPosition().z <= minZ)
        bird->addFutureSpeed({0.0f, 0.0f, turnFactor});

    float vertDist = maxZ - bird->getPosition().z;
    vertDist = vertDist*vertDist;

    bird->addFutureSpeed({0.0f, 0.0f, -turnFactor/vertDist});
    if (terrain != nullptr &&
    bird->getPosition().z <= terrainMargin+terrain->getTerrainHeight(bird->getPosition().x, bird->getPosition().y))
//    vertDist = bird->getPosition().z-terrain->getTerrainHeight(bird->getPosition().x, bird->getPosition().y);
//    vertDist = vertDist*vertDist;
    bird->addFutureSpeed({0.0f, 0.0f, turnFactor});
}

std::shared_ptr<Object> & Boid::getBird(int idx) {
    return birds[idx];
}
