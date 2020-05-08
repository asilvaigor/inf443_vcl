//
// Created by igor on 04/05/2020.
//

#include "BoundingBox.h"

BoundingBox::BoundingBox() {
    minX = std::numeric_limits<float>::max();
    maxX = std::numeric_limits<float>::min();
    minY = std::numeric_limits<float>::max();
    maxY = std::numeric_limits<float>::min();
    minZ = std::numeric_limits<float>::max();
    maxZ = std::numeric_limits<float>::min();
    empty = true;
};

BoundingBox::BoundingBox(float _minX, float _maxX, float _minY, float _maxY, float _minZ, float _maxZ) {
    minX = _minX;
    maxX = _maxX;
    minY = _minY;
    maxY = _maxY;
    minZ = _minZ;
    maxZ = _maxZ;
    empty = false;
}

void BoundingBox::update(const vcl::vec3 &point) {
    if (empty) {
        minX = maxX = point.x;
        minY = maxY = point.y;
        minZ = maxZ = point.z;
        empty = false;
    } else {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
        minZ = std::min(minZ, point.z);
        maxZ = std::max(maxZ, point.z);
    }
}

std::vector<vcl::vec3> BoundingBox::getCorners() {
    std::vector<vcl::vec3> corners;
    corners.reserve(8);
    for (int i = 0; i < 8; i++) {
        vcl::vec3 pt;
        pt.x = i / 4 == 0 ? minX : maxX;
        pt.y = (i / 2) % 2 == 0 ? minY : maxY;
        pt.z = i % 2 == 0 ? minZ : maxZ;
        corners.push_back(pt);
    }
    return corners;
}

float BoundingBox::relativeSize(vcl::camera_scene &camera) {
    if (empty) return std::numeric_limits<float>::max();

    float xMin = std::numeric_limits<float>::max(), yMin = std::numeric_limits<float>::max(),
            xMax = std::numeric_limits<float>::min(), yMax = std::numeric_limits<float>::min();
    auto cameraMatrix = camera.perspective.matrix() * camera.view_matrix();
    auto corners = getCorners();

    for (auto &pt : corners) {
        vcl::vec4 p = cameraMatrix * vcl::vec4(pt, 1.0f);
        xMin = std::min(xMin, p.x / p.w);
        yMin = std::min(yMin, p.y / p.w);
        xMax = std::max(xMax, p.x / p.w);
        yMax = std::max(yMax, p.y / p.w);
    }

    return std::max(0.5 * (xMax - xMin), 0.5 * (yMax - yMin));
}
