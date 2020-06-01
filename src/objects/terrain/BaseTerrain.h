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
     * @param xSize
     * @param ySize
     */
    BaseTerrain(float xSize, float ySize);

    virtual ~BaseTerrain();

    /**
     * Sets new lights to the terrain.
     * @param lights
     * @param currentLight
     */
    void setLight(std::vector<std::shared_ptr<vcl::light_source>> &lights, int currentLight);

    /**
     * Returns the height of the terrain at a given point.
     * @param x Must be within terrain boundaries.
     * @param y Must be within terrain boundaries.
     * @return z value of the terrain.
     */
    virtual float getTerrainHeight(float x, float y) = 0;

    /**
     * Height of the peak of the terrain.
     * @return
     */
    virtual float getMaxTerrainHeight();

    /**
     * Calculates the vector normal to the terrain's ground in a given spot.
     * @param x
     * @param y
     * @return Normal vector.
     */
    virtual vcl::vec3 normal(float x, float y) = 0;

    virtual bool isObstructed(float x, float y);

    /**
     * Length of a side of the terrain.
     * @return
     */
    float &getXSize();

    /**
     * Length of a side of the terrain.
     * @return
     */
    float &getYSize();

protected:
    vcl::mesh_drawable terrain;
    int currentLight;
    float xSize;
    float ySize;
};


#endif //PGM_BASETERRAIN_H
