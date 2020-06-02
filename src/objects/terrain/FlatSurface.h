//
// Created by igor on 23/04/2020.
//

#ifndef PGM_FLATSURFACE_H
#define PGM_FLATSURFACE_H

#include "objects/terrain/BaseTerrain.h"
#include "utils/Texture.h"

/**
 * Flat ground object for the scene.
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
     */
    void drawMesh(vcl::camera_scene &camera) override;

    /**
     * Returns the height of the terrain at a given point.
     * @param x Must be within terrain boundaries.
     * @param y Must be within terrain boundaries.
     * @return z value of the terrain.
     */
    float getTerrainHeight(float x, float y) override;

    /**
     * Calculates the vector normal to the terrain's ground in a given spot.
     * @param x
     * @param y
     * @return Normal vector.
     */
    vcl::vec3 normal(float x, float y);

private:
    const Texture snowTexture;
};


#endif //PGM_FLATSURFACE_H
