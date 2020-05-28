//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#ifndef PGM_MOUNTAINTERRAIN_H
#define PGM_MOUNTAINTERRAIN_H

#include <src/shaders/Shaders.h>
#include <vcl/math/noise/PerlinNoise.h>
#include <vcl/math/helper_functions/helper_functions.hpp>
#include "BaseTerrain.h"
#include "TerrainFeature.h"

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
    MountainTerrain(Shaders &shaders, float xSize, float ySize);

    /**
     * Draws the surface in opengl;
     * @param camera
     */
    void drawMesh(vcl::camera_scene &camera) override;

    /**
     * Sets a new light to the terrain.
     * @param light
     * @param idx
     */
    void setLight(std::shared_ptr<vcl::light_source> &light, int idx) override;

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

    //TODO add descriptions
    float evaluate_base_terrain_outline(float u, float v);

    float evaluate_terrain_z_no_erosion(float u, float v, vcl::NoiseParameters &parameters);

    float altitudeErosionFbmNoise(double x, double y);

    // TODO change noise parameters names and pass them to class
    vcl::NoiseParameters parameters;
    vcl::PerlinNoise noiseGenerator;
    vcl::mesh terrainMesh;
    std::vector<TerrainFeature> features;
    float xSize;
    float ySize;
};


#endif //PGM_MOUNTAINTERRAIN_H
