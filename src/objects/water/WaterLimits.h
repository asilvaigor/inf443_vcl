//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#ifndef PGM_WATERLIMITS_H
#define PGM_WATERLIMITS_H

#include "vcl.hpp"

class WaterLimits  {
public:
    WaterLimits(float x1, float x2, float y1, float y2, float z, float waterLevel);

    vcl::mesh_drawable& getMesh();

private:
    float x1;
    float x2;
    float y1;
    float y2;
    float z1;
    float z2;

    float waterLevel;

    vcl::mesh_drawable mesh;
};


#endif //PGM_WATERLIMITS_H
