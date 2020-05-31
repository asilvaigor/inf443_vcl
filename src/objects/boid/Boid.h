//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#ifndef PGM_BOID_H
#define PGM_BOID_H


#include <src/objects/Object.h>
#include "Bird.h"

class Boid : public Object {
public:
    Boid(Shaders &shaders, int birdCount, float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

    void drawMesh(vcl::camera_scene &camera) override;

    void updateBirds();

    void birdFlyCenter(Bird* bird);

    void birdAvoidOthers(Bird* bird);

    void birdMatchOthers(Bird* bird);

    void birdBound(Bird* bird);

    void birdLimitSpeed(Bird* bird);

    std::vector<std::shared_ptr<Object>> &getObjects();

private:
    std::vector<std::shared_ptr<Object>> birds;
    float maxX, maxY, maxZ;
    float minX, minY, minZ;
    vcl::rand_generator generator;

    std::chrono::time_point<std::chrono::system_clock> timer;

    // TODO do not hardcode these variables
    const float centeringFactor = 0.000001f;
    const float visualRange = 20.0f;

    const float minDistance = 5.0f;
    const float avoidFactor = 0.001f;

    const float matchingFactor = 0.001f;

    const float turnFactor = 0.01f;

    const float speedMaxLimit = 1.0f;
    const float speedMinLimit = 0.5f;
};


#endif //PGM_BOID_H
