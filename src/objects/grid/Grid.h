//
// Created by igor on 25/04/2020.
//

#ifndef PGM_GRID_H
#define PGM_GRID_H

#include "vcl.hpp"
#include "objects/Object.h"
#include "utils/Texture.h"

/**
 * Grid object.
 */
class Grid : public Object {
public:
    /**
     * Generates the grid.
     * @param shaders
     */
    explicit Grid(Shaders &shaders);

    /**
     * Draws the grid in opengl;
     * @param camera
     * @param light
     */
    void drawMesh(vcl::camera_scene &camera) override;

private:
    vcl::mesh_drawable grid;
};


#endif //PGM_GRID_H
