//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_DOME_H
#define PGM_DOME_H


#include "objects/Object.h"
#include "utils/Texture.h"

class Dome : public Object {
public:
    explicit Dome(Shaders &shaders);

    void drawMesh(vcl::camera_scene &camera) override;

private:
    vcl::mesh mesh;
    vcl::mesh_drawable meshDrawable;
    Texture texture;
};


#endif //PGM_DOME_H
