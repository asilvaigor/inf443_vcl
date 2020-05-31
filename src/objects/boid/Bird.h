//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#ifndef PGM_BIRD_H
#define PGM_BIRD_H


#include <src/objects/Object.h>

class Bird : public Object {
public:
    Bird(Shaders &shaders, vcl::vec3 pos, float scale, vcl::vec3 speed = {1, 0, 0});

    void drawMesh(vcl::camera_scene &camera);

    vcl::vec3 getSpeed();

    void setSpeed(vcl::vec3 speed);

    vcl::vec3 getPosition();

    void setPosition(vcl::vec3 pos);
private:
    vcl::vec3 p;
    vcl::vec3 dp;

    vcl::mesh_drawable mesh;
};


#endif //PGM_BIRD_H
