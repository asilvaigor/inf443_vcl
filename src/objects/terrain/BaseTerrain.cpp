//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#include "BaseTerrain.h"

BaseTerrain::BaseTerrain(float xSize, float ySize) : Object(false), xSize(xSize), ySize(ySize) {}

BaseTerrain::~BaseTerrain() {}

void BaseTerrain::setLight(std::vector<std::shared_ptr<vcl::light_source>> &lights, int currentLight) {
    this->lights = lights;
    this->currentLight = currentLight;
}

float BaseTerrain::getMaxTerrainHeight() {
    return 0.0f;
}

bool BaseTerrain::isObstructed(float, float) {
    return false;
}

float &BaseTerrain::getXSize() {
    return xSize;
}

float &BaseTerrain::getYSize() {
    return ySize;
}