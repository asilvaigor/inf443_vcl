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

float WaterLimits::getWaterLevel() {
    return z2;
}

bool WaterLimits::isInside(vcl::vec2 pos) {
    return (pos.x >= x1 && pos.x <= x2) &&
           (pos.y >= y1 && pos.y <= y2);
}

bool WaterLimits::isInside(vcl::vec3 pos) {
    return isInside({pos.x, pos.y}) && (pos.z >= z1 && pos.z <= z2);
}

float WaterLimits::getX1() {
    return x1;
}

float WaterLimits::getX2() {
    return x2;
}

float WaterLimits::getY1() {
    return y1;
}

float WaterLimits::getY2() {
    return y2;
}

