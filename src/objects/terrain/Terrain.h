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
     */
    void draw(const vcl::camera_scene &camera) override;

    void setLight2(std::shared_ptr<vcl::light_source> &light);

    void setLight3(std::shared_ptr<vcl::light_source> &light);

private:
    vcl::mesh_drawable terrain;
    const Texture snowTexture;
    // Terrain has extra lights
    std::shared_ptr<vcl::light_source> light2;
    std::shared_ptr<vcl::light_source> light3;
};


#endif //PGM_TERRAIN_H