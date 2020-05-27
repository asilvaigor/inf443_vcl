//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#include "BaseTerrain.h"

BaseTerrain::BaseTerrain() : Object(false) {}

void BaseTerrain::setLight(std::vector<std::shared_ptr<vcl::light_source>> &lights, int currentLight) {
    this->lights = lights;
    this->currentLight = currentLight;
}