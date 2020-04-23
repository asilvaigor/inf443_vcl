//
// Created by igor on 16/04/20.
//

#include "BezierSpline.h"

BezierSpline::BezierSpline() {
    nPoints = 0;
}

void BezierSpline::addPoint(vcl::vec3 pos, float r, vcl::vec3 lH, vcl::vec3 rH) {
    nPoints++;
    pts.push_back(pos);
    this->radius.push_back(r);
    this->leftHandle.push_back(lH);
    this->rightHandle.push_back(rH);
}

vcl::mesh BezierSpline::toMesh() {
    vcl::mesh mesh;

    if (nPoints < 2)
        return mesh;

    const float circleResolution = 64;
    const float lengthResolution = 8;
    const int minNCircles = 2;
    const int minCircleNPoints = 3;

    // Calculating number of points in each circle and number of circles in total.
    int uMax = 0, vMax = 0;
    for (int i = 0; i < nPoints - 1; i++) {
        uMax = std::max(uMax, std::max(minCircleNPoints, (int) ceil(circleResolution * radius[i])));
        if (i < nPoints - 1)
            vMax += std::max(minNCircles, (int) ceil(lengthResolution * pts[i].dist(pts[i + 1])));
    }

    // If it finishes in a point, will optimize the number of points
    bool finishesInPoint = radius.back() < FLT_EPSILON;
    if (finishesInPoint)
        vMax--;

    // Allocating memory
    mesh.position.reserve((unsigned long) uMax * vMax + finishesInPoint);
    mesh.normal.reserve((unsigned long) uMax * vMax + finishesInPoint);
    mesh.texture_uv.reserve((unsigned long) uMax * vMax + finishesInPoint);
    mesh.color.reserve((unsigned long) uMax * vMax + finishesInPoint);
    mesh.connectivity.reserve((unsigned long) uMax * (vMax - 1) * 2 + finishesInPoint * uMax);

    // Going through each point in the spline
    vcl::vec3 lastCenter;
    int v = 0;
    for (int i = 0; i < nPoints - 1; i++) {
        int nCircles = std::max(minNCircles, (int) ceil(lengthResolution * pts[i].dist(pts[i + 1])));
        // Going through each circle
        for (int j = 0; j < nCircles; j++) {
            float a = (float) j / (float) (nCircles - 1);

            // Bezier interpolation for center of circles
            vcl::vec3 center = position(i, a);

            // Linear interpolation for radius
            float b = center.dist(pts[i]) / (center.dist(pts[i]) + center.dist(pts[i + 1]));
            float r = radius[i] * (1.0f - b) + radius[i + 1] * b;

            // Direction of the curve
            vcl::vec3 dir = (center - lastCenter).normalized();
            if (j == 0) {
                float nextA = 1.0f / (float) (nCircles - 1);
                vcl::vec3 nextCenter = position(i, nextA);
                dir = (nextCenter - center).normalized();
            }

            // If it is not a point
            if (r > FLT_EPSILON) {
                // Placing points in mesh as a circle orthogonal to dir
                vcl::vec3 ort1 = vcl::cross(dir, {1, 0, 0}).normalized();
                vcl::vec3 ort2 = vcl::cross(dir, ort1).normalized();
                for (int u = 0; u < uMax; u++) {
                    auto angle = (float) (2 * M_PI * u / uMax);
                    vcl::vec3 normal = std::cos(angle) * ort1 + std::sin(angle) * ort2;
                    mesh.position.push_back(center + r * normal);
                    mesh.normal.push_back(normal);
                    mesh.texture_uv.push_back({(float) u / (float) uMax,
                                               ((float) v / (float) vMax) * pts[0].dist(pts.back()) / 0.5f});
                }
            } else {
                // Only adding one point and one triangle
                mesh.position.push_back(center);
                mesh.normal.push_back(dir);
                mesh.texture_uv.push_back({0,
                                           ((float) v / (float) vMax) * pts[0].dist(pts.back()) / 0.5f});
            }

            // Adjusting connectivity
            if (i != 0 || j != 0) {
                if (!finishesInPoint || (i != nPoints - 2 || j != nCircles - 1)) {
                    for (int k1 = 0, k2 = 0; k1 < uMax + 1 && k2 < uMax + 1; k1++, k2++) {
                        auto i0 = (unsigned int) (mesh.position.size() - uMax);
                        auto j0 = (unsigned int) (mesh.position.size() - 2 * uMax);

                        auto i1 = i0 + (unsigned int) k1 % uMax;
                        auto i2 = i0 + (unsigned int) (k1 + 1) % uMax;
                        auto j1 = j0 + (unsigned int) k2 % uMax;
                        auto j2 = j0 + (unsigned int) (k2 + 1) % uMax;
                        mesh.connectivity.push_back({i1, j1, i2});
                        mesh.connectivity.push_back({j1, i2, j2});
                    }
                } else {
                    // If it ends in a point, adding only one triangle
                    for (int k1 = 0; k1 < uMax + 1; k1++) {
                        auto i0 = (unsigned int) (mesh.position.size() - uMax - 1);
                        auto j0 = (unsigned int) (mesh.position.size() - 1);
                        auto i1 = i0 + (unsigned int) k1 % uMax;
                        auto i2 = i0 + (unsigned int) (k1 + 1) % uMax;
                        mesh.connectivity.push_back({i1, j0, i2});
                    }
                }
            }

            lastCenter = center;
            v++;
        }
    }

    return mesh;
}

vcl::vec3 BezierSpline::position(int i, float offset) {
    vcl::vec3 rightH = rightHandle[i];
    vcl::vec3 leftH = leftHandle[i + 1];
    return pow((1.0f - offset), 3.0f) * pts[i] + 3.0f * pow((1.0f - offset), 2.0f) * offset * rightH +
           3.0f * (1.0f - offset) * pow(offset, 2.0f) * leftH + pow(offset, 3.0f) * pts[i + 1];
}

vcl::vec3 BezierSpline::tangent(int i, float offset) {
    vcl::vec3 rightH = rightHandle[i];
    vcl::vec3 leftH = leftHandle[i + 1];
    vcl::vec3 tangent = 3.0f * pow((1.0f - offset), 2.0f) * (rightH - pts[i]) +
                        6.0f * (1.0f - offset) * offset * (leftH - rightH) +
                        3.0f * pow(offset, 2.0f) * (pts[i + 1] - leftH);
    return tangent.normalized();
}

int BezierSpline::getNPoints() const {
    return nPoints;
}
