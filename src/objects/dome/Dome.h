//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_DOME_H
#define PGM_DOME_H


#include <src/objects/Object.h>

class Dome : public Object {
public:
    explicit Dome(Shaders &shaders);

    void drawMesh(vcl::camera_scene &camera, float time = 0.0f) override;

private:
    vcl::mesh mesh;
    vcl::mesh_drawable meshDrawable;
};


#endif //PGM_DOME_H
