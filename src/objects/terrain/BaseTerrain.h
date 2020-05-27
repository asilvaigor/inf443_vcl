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

    /**
     * Sets new lights to the terrain.
     * @param lights
     * @param currentLight
     */
    void setLight(std::vector<std::shared_ptr<vcl::light_source>> &lights, int currentLight);

protected:
    vcl::mesh_drawable terrain;
    int currentLight;
};


#endif //PGM_BASETERRAIN_H
