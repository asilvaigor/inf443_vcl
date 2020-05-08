//
// Created by igor on 23/04/2020.
//

#ifndef PGM_TERRAIN_H
#define PGM_TERRAIN_H

#include "objects/terrain/BaseTerrain.h"
#include "utils/Texture.h"

/**
 * Ground object for the scene.
 */
class Terrain : public BaseTerrain {
public:
    /**
     * Generates the terrain.
     * @param shaders
     */
    Terrain(Shaders &shaders);

    /**
     * Draws the surface in opengl;
     * @param camera
     */
    void draw(vcl::camera_scene &camera) override;

    /**
     * Sets a new light to the terrain.
     * @param light
     * @param idx
     */
    void setLight(std::shared_ptr<vcl::light_source> &light, int idx) override;

private:
    const Texture snowTexture;
};


#endif //PGM_TERRAIN_H
