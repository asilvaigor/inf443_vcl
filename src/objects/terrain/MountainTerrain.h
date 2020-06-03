//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#ifndef PGM_MOUNTAINTERRAIN_H
#define PGM_MOUNTAINTERRAIN_H

#include <src/shaders/Shaders.h>
#include <vcl/math/noise/PerlinNoise.h>
#include <vcl/math/helper_functions/helper_functions.hpp>
#include <src/objects/water/Water.h>
#include "BaseTerrain.h"
#include "TerrainFeature.h"
#include "objects/water/WaterLimits.h"
#include "utils/Texture.h"

/**
 * Generates procedurally a terrain with some
 * predefined relief
 */

class MountainTerrain : public BaseTerrain {
public:
    /**
     * Generates the relief mesh
     * @param shaders
     */
    explicit MountainTerrain(Shaders &shaders, float xSize, float ySize, WaterLimits &waterLimits);

    /**
     * Draws the surface in opengl;
     * @param camera
     */
    void drawMesh(vcl::camera_scene &camera) override;

    /**
     * Adds a mountain in the terrain
     * @param pos
     * @param sig
     * @param height
     */
    void addMountain(vcl::vec2 pos, float sig, float height);

    /**
     * Adds a gaussian curve to the terrain altitude map
     * @param pos
     * @param sig
     * @param height
     */
    void addHighLevelFeature(vcl::vec2 pos, float sig, float height);

    /**
     * Returns the height of the terrain at a given point.
     * @param x Must be within terrain boundaries.
     * @param y Must be within terrain boundaries.
     * @return z value of the terrain.
     */
    float getTerrainHeight(float x, float y) override;

    /**
     * Height of the peak of the mountain.
     * @return
     */
    float getMaxTerrainHeight() override;

    /**
     * Checks if a point is inside the lake area.
     * @param x Must be within terrain boundaries.
     * @param y Must be within terrain boundaries.
     * @return True if the point will fall inside the lake.
     */
    bool isObstructed(float x, float y) override;

    /**
     * Calculates the vector normal to the terrain's ground in a given spot.
     * @param x
     * @param y
     * @return Normal vector.
     */
    vcl::vec3 normal(float x, float y) override;

private:

    /**
     * Generates the mesh to be drawn
     */
    void evaluate_mesh();

    /**
     * Maps a surface point in u, v in (0, 1) to a 3D
     * point
     * @param u
     * @param v
     * @return 3D point
     */
    vcl::vec3 evaluate_terrain(float u, float v);

    /**
     * Terrain's elevation
     * @param u
     * @param v
     * @return Terrain's elevation
     */
    float evaluate_terrain_z(float u, float v);

    float altitudeErosionFbmNoise(double x, double y);

    // TODO change noise parameters names and pass them to class
    vcl::NoiseParameters parameters;
    vcl::PerlinNoise noiseGenerator;
    vcl::mesh terrainMesh;
    std::shared_ptr<Texture> terrainTexture;
    std::vector<TerrainFeature> features;

    float mountainHeight;

    // Lake parameters
    WaterLimits waterLimits;

    vcl::vec2 lakePos;
    float lakeSig;
};


#endif //PGM_MOUNTAINTERRAIN_H
