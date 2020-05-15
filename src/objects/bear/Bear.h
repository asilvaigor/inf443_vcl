//
// Created by igor on 14/05/2020.
//

#ifndef PGM_BEAR_H
#define PGM_BEAR_H

#include "vcl.hpp"
#include "objects/Object.h"

/**
 * Bear object.
 */
class Bear : public Object {
public:
    /**
     * Generates the bear.
     * @param shaders
     * @param base
     */
    explicit Bear(Shaders &shaders, vcl::vec3 base = {0, 0, 0});

    /**
     * Draws the bear in opengl;
     * @param camera
     * @param time
     */
    void drawMesh(vcl::camera_scene &camera, float time) override;

private:
    vcl::fbx_model bear;
};


#endif //PGM_BEAR_H
