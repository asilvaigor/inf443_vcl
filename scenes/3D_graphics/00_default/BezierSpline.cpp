//
// Created by igor on 16/04/20.
//

#include "BezierSpline.h"

BezierSpline::BezierSpline() {
    nPoints = 0;
};

void BezierSpline::addPoint(vcl::vec3 pos, float radius, vcl::vec3 leftHandle, vcl::vec3 rightHandle) {
    nPoints++;
    pts.push_back(pos);
    this->radius.push_back(radius);
    this->leftHandle.push_back(leftHandle);
    this->rightHandle.push_back(rightHandle);
}

vcl::mesh BezierSpline::toMesh() {
    vcl::mesh mesh;
    const float circleResolution = 64;
    const float lengthResolution = 10;

    // Calculating number of points in each circle and number of circles in total.
    int uMax = 0, vMax = 0;
    for (int i = 0; i < nPoints; i++) {
        uMax = std::max(uMax, (int) ceil(circleResolution * radius[i]));
        if (i < nPoints - 1)
            vMax += (int) ceil(lengthResolution * pts[i].dist(pts[i + 1]));
    }

    // Allocating memory
    mesh.position.reserve((unsigned long) uMax * vMax);
    mesh.normal.reserve((unsigned long) uMax * vMax);
    mesh.texture_uv.reserve((unsigned long) uMax * vMax);
    mesh.connectivity.reserve((unsigned long) (uMax + 2) * (vMax - 1) * 2);

    // Going through each point in the spline
    vcl::vec3 lastCenter;
    int v = 0;
    for (int i = 0; i < nPoints - 1; i++) {
        int nCircles = (int) ceil(lengthResolution * pts[i].dist(pts[i + 1]));
        // Going through each circle
        for (int j = 0; j < nCircles; j++) {
            float a = 1.0f * j / (nCircles - 1);

            // Bezier interpolation for center of circles
            vcl::vec3 center = pow(1 - a, 3) * pts[i] + 3 * a * pow(1 - a, 2) * rightHandle[i] +
                               3 * pow(a, 2) * (1 - a) * leftHandle[i + 1] + pow(a, 3) * pts[i + 1];

            // Linear interpolation for radius
            float b = center.dist(pts[i]) / pts[i].dist(pts[i+1]);
            float r = radius[i] * (1.0f - b) + radius[i + 1] * b;

            // Direction of the curve
            vcl::vec3 dir = (center - lastCenter).normalized();
            if (j == 0) {
                float nextA = 1.0f / (nCircles - 1);
                vcl::vec3 nextCenter = pow(1 - nextA, 3) * pts[i] + 3 * nextA * pow(1 - nextA, 2) * rightHandle[i] +
                                       3 * pow(nextA, 2) * (1 - nextA) * leftHandle[i + 1] + pow(nextA, 3) * pts[i + 1];
                dir = (nextCenter - center).normalized();
            }

            // Placing points in mesh as a circle orthogonal to dir
            vcl::vec3 ort1 = vcl::cross(dir, {1, 0, 0}).normalized();
            vcl::vec3 ort2 = vcl::cross(dir, ort1).normalized();
            for (int u = 0; u < uMax; u++) {
                auto angle = (float) (2 * M_PI * u / uMax);
                vcl::vec3 normal = std::cos(angle) * ort1 + std::sin(angle) * ort2;
                mesh.position.push_back(center + r * normal);
                mesh.normal.push_back(normal);
                mesh.texture_uv.push_back({(float) u, (float) v});
            }

            // Adjusting connectivity
            if (i != 0 || j != 0) {
                for (int k1 = 0, k2 = 0; k1 < uMax + 2 && k2 < uMax + 2; k1++, k2++) {
                    auto i0 = (unsigned int) (mesh.position.size() - uMax);
                    auto j0 = (unsigned int) (mesh.position.size() - 2 * uMax);

                    auto i1 = i0 + (unsigned int) k1 % uMax;
                    auto i2 = i0 +(unsigned int) (k1 + 1) % uMax;
                    auto j1 = j0 + (unsigned int) k2 % uMax;
                    auto j2 = j0 + (unsigned int) (k2 + 1) % uMax;
                    mesh.connectivity.push_back({i1, j1, i2});
                    mesh.connectivity.push_back({j1, i2, j2});
                }
            }

            lastCenter = center;
            v++;
        }
    }

    return mesh;
}
