//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#ifndef PGM_BIRD_H
#define PGM_BIRD_H


#include <src/objects/Object.h>

class Bird : public Object {
public:
    Bird(Shaders &shaders, vcl::vec3 pos, float scale, vcl::vec3 speed = {1, 0, 0}, float turning = 50);

    void drawMesh(vcl::camera_scene &camera, float time = 0.0f) override;

    vcl::vec3 getSpeed();

    vcl::vec3 getFutureSpeed();

    void addFutureSpeed(vcl::vec3 speed);

    void setFutureSpeed(vcl::vec3 speed);

    vcl::vec3 getPosition();

    void setPosition(vcl::vec3 pos);

    void stepSpeed();

    void stepPosition();

private:
    vcl::vec3 p;
    vcl::vec3 dp;
    vcl::vec3 odp;
    vcl::vec3 ndp;
    float turining;

    vcl::fbx_model bird;

    static std::vector<GLuint> textures;
};


#endif //PGM_BIRD_H
