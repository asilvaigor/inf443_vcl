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

    float getWaterLevel();

    bool isInside(vcl::vec3 pos);

    bool isInside(vcl::vec2 pos);
    
    float getX1();
    
    float getX2();
    
    float getY1();
    
    float getY2();

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
