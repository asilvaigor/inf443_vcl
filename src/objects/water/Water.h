//
// Created by Aloysio Galvão Lopes on 2020-05-29.
//

#ifndef PGM_WATER_H
#define PGM_WATER_H


#include "vcl.hpp"
#include "objects/Object.h"
#include <chrono>

class Water : public Object {
public:
    /**
     * Generates the water mesh
     * @param shaders
     */
    Water(Shaders& shaders, float xSize, float ySize);

    /**
     * Draws mesh in opengl
     * @param camera
     */
    void drawMesh(vcl::camera_scene &camera) override;

private:
    vcl::mesh_drawable waterMeshDrawable;
    vcl::mesh waterMesh;
    float xSize, ySize;
    size_t uDimensionSize, vDimensionSize;
    std::chrono::time_point<std::chrono::system_clock> timer, timeBegin;
    std::vector<std::vector<vcl::vec3>> waterPositions;
    std::vector<std::vector<float>> waterUpdatedVerticalPositions;
    std::vector<std::vector<float>> waterVerticalSpeeds;

    vcl::vec3 oscillator;
    vcl::mesh_drawable oscilator_mesh;

    void update_mesh();
    void initialize_mesh();

    void update_oscillators();
    void update_heights();
};


#endif //PGM_WATER_H
