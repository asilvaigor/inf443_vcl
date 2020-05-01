//
// Created by igor on 25/04/2020.
//

#ifndef PGM_BOX_H
#define PGM_BOX_H

#include "vcl.hpp"
#include "objects/Object.h"
#include "utils/Texture.h"

/**
 * Box object.
 */
class Box : public Object {
public:
    /**
     * Generates the box.
     * @param shaders
     */
    explicit Box(Shaders &shaders);

    /**
     * Draws the box in opengl;
     * @param camera
     */
    void draw(const vcl::camera_scene &camera) override;

private:
    vcl::mesh_drawable box;
};


#endif //PGM_BOX_H
