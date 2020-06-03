//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#ifndef PGM_BIRD_H
#define PGM_BIRD_H


#include <src/objects/Object.h>

class Bird : public Object {
public:
    Bird(Shaders &shaders, vcl::vec3 pos, float scale, vcl::vec3 speed = {1, 0, 0}, float turning = 0);

    void drawMesh(vcl::camera_scene &camera) override;

    void update(float time) override;

    vcl::vec3 getSpeed();

    vcl::vec3 getFutureSpeed();

    float getRotation();

    void addFutureSpeed(vcl::vec3 speed);

    void setFutureSpeed(vcl::vec3 speed);

    void setPosition(vcl::vec3 pos);

    void stepSpeed();

    void stepPosition();

private:
    float animationTime;
    vcl::vec3 dp;
    vcl::vec3 odp;
    vcl::vec3 ndp;
    float turining;

    vcl::fbx_model bird;

    // TODO move somewhere else
    const float turnFactor = 0.05f;
    const float maxTurnFactor = 100.0f;
    const float animationMaxTime = 1;
    const float animationSpeed = 0.03f;

    static std::vector<GLuint> textures;
};


#endif //PGM_BIRD_H
