//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#include "WaterLimits.h"

WaterLimits::WaterLimits(float x1, float x2, float y1, float y2, float z, float waterLevel) :
    x1(x1), x2(x2), y1(y1), y2(y2), z1(z), z2(waterLevel), waterLevel(waterLevel)
{
    vcl::vec3 p0 = vcl::vec3(x1, y1, z1);
    mesh = vcl::mesh_primitive_parallelepiped(p0,
                                              vcl::vec3(x2, y1, z1)-p0,
                                              vcl::vec3(x1, y2, z1)-p0,
                                              vcl::vec3(x1, y1, z2)-p0);
}

vcl::mesh_drawable &WaterLimits::getMesh() {
    return mesh;
}
