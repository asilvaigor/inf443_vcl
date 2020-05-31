//
// Created by Aloysio Galvão Lopes on 2020-05-29.
//

#ifndef PGM_WATER_H
#define PGM_WATER_H


#include "vcl.hpp"
#include "objects/Object.h"
#include "WaterOscillator.h"
#include "WaterLimits.h"
#include <chrono>

class Water : public Object {
public:
    /**
     * Generates the water mesh
     * @param shaders
     */
    Water(Shaders &shaders, WaterLimits& waterLimits, std::vector<WaterOscillator> &oscillators);

    /**
     * Draws mesh in opengl
     * @param camera
     */
    void drawMesh(vcl::camera_scene &camera) override;

private:
    // Mesh related variables
    vcl::mesh_drawable waterMeshDrawable;
    vcl::mesh waterMesh;
    WaterLimits &waterLimits;
    float xSize, ySize;
    size_t uDimensionSize, vDimensionSize;

    // Animation related variables
    std::chrono::time_point<std::chrono::system_clock> timer;
    std::vector<std::vector<vcl::vec3>> waterPositions;
    std::vector<std::vector<float>> waterVerticalSpeeds;
    std::vector<WaterOscillator>& oscillators;

    void update_mesh();
    void initialize_mesh();

    void update_heights();
};


#endif //PGM_WATER_H
