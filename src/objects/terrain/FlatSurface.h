//
// Created by igor on 23/04/2020.
//

#ifndef PGM_FLATSURFACE_H
#define PGM_FLATSURFACE_H

#include "objects/terrain/BaseTerrain.h"
#include "utils/Texture.h"

/**
 * Ground object for the scene.
 */
class FlatSurface : public BaseTerrain {
public:
    /**
     * Generates the terrain.
     * @param shaders
     */
    FlatSurface(Shaders &shaders);

    /**
     * Draws the surface in opengl;
     * @param camera
     * @param time
     */
    void drawMesh(vcl::camera_scene &camera , float time = 0.0f) override;

private:
    const Texture snowTexture;
};


#endif //PGM_FLATSURFACE_H
