//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#ifndef PGM_BIRD_H
#define PGM_BIRD_H


#include <src/objects/Object.h>

class Bird : public Object {
public:
    Bird(Shaders &shaders, vcl::vec3 pos, float scale, vcl::vec3 speed = {0, 0, 1});

    void drawMesh(vcl::camera_scene &camera) override;

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
    vcl::vec3 ndp;

    vcl::mesh_drawable mesh;
};


#endif //PGM_BIRD_H
