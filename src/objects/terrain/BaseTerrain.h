//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#ifndef PGM_BASETERRAIN_H
#define PGM_BASETERRAIN_H

#include "vcl.hpp"
#include "objects/Object.h"

/**
 * Ground object for the scene.
 */
class BaseTerrain : public Object{
public:
    /**
     * Generates the terrain.
     * @param shaders
     */
    BaseTerrain();

    virtual ~BaseTerrain();

    /**
     * Sets a new light to the terrain.
     * @param light
     * @param idx
     */
    virtual void setLight(std::shared_ptr<vcl::light_source> &light, int idx) = 0;
protected:
    vcl::mesh_drawable terrain;
    // Terrain has extra lights
    std::shared_ptr<vcl::light_source> light2;
    std::shared_ptr<vcl::light_source> light3;
};


#endif //PGM_BASETERRAIN_H
