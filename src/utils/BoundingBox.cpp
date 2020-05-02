//
// Created by igor on 01/05/2020.
//

#include "BoundingBox.h"

BoundingBox::BoundingBox() = default;

BoundingBox::BoundingBox(float _minX, float _maxX, float _minY, float _maxY, float _minZ, float _maxZ) {
    minX = _minX;
    maxX = _maxX;
    minY = _minY;
    maxY = _maxY;
    minZ = _minZ;
    maxZ = _maxZ;
}

void BoundingBox::update(const vcl::vec3 &point) {
    minX = std::min(minX, point.x);
    maxX = std::max(maxX, point.x);
    minY = std::min(minY, point.y);
    maxY = std::max(maxY, point.y);
    minZ = std::min(minZ, point.z);
    maxZ = std::max(maxZ, point.z);
}

int BoundingBox::isInFrustum(vcl::camera_scene &camera, vcl::light_source &light) {
    vcl::vec3 base((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, minZ);
    if (camera.is_inside_frustum(base)) {
        float d = std::fabs(vcl::dot(camera.camera_direction(), (base - camera.camera_position())));
        return light.get_z_near() < d && d < light.get_z_far();
    }
    return false;
}