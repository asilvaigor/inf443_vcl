//
// Created by igor on 23/04/2020.
//

#ifndef PGM_TERRAIN_H
#define PGM_TERRAIN_H

#include "vcl.hpp"
#include "objects/Object.h"
#include "utils/Texture.h"

/**
 * Ground object for the scene.
 */
class Terrain : public Object {
public:
    /**
     * Generates the terrain.
     * @param shaders
     */
    Terrain(Shaders &shaders);

    /**
     * Draws the terrain in opengl;
     * @param camera
     * @param sunMatrix Matrix for the light source transformation.
     */
    void draw(const vcl::camera_scene &camera, vcl::mat4 &sunMatrix) override;

private:
    vcl::mesh_drawable terrain;
    const Texture snowTexture;
};


#endif //PGM_TERRAIN_H
