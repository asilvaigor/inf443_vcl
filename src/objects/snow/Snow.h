//
// Created by igor on 02/06/2020.
//

#ifndef PGM_SNOW_H
#define PGM_SNOW_H

#include "objects/Object.h"

class Snow : public Object {
public:
    explicit Snow(Shaders &shaders, int nParticles = 250);

    void drawMesh(vcl::camera_scene& camera) override;

    void update(float time) override;

private:
    std::vector<vcl::mesh_drawable> particles;
    std::vector<vcl::vec3> velocities, accelerations;
    vcl::camera_scene *camera;
    vcl::vec3 lastCameraPosition;
    float lastTime;
    int nParticles;
    float cameraDistance;
    float minFlakeSize;
    float flakeSizeVar;
    float dampingMassRatio;
    float accelerationNoise;
    std::vector<float> timeLastNoise;
    std::vector<vcl::vec2> noise;

    static vcl::rand_generator generator;
};


#endif //PGM_SNOW_H
