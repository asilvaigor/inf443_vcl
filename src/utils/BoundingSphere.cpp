//
// Created by igor on 01/05/2020.
//

#include "BoundingSphere.h"

BoundingSphere::BoundingSphere() {
    radius = std::numeric_limits<float>::max();
}

BoundingSphere::BoundingSphere(vcl::vec3 p, float r) {
    pt = p;
    radius = r;
}

BoundingSphere::BoundingSphere(vcl::mesh &mesh) {
    std::vector<vcl::vec3 *> points;
    points.reserve(mesh.position.size());
    for (auto &p : mesh.position)
        points.push_back(&p);

    calculate(points);
}

BoundingSphere::BoundingSphere(std::vector<vcl::mesh> &meshes) {
    int nVertex = 0;
    for (auto &m : meshes)
        nVertex += m.position.size();

    std::vector<vcl::vec3 *> points;
    points.reserve(nVertex);
    for (auto &m : meshes)
        for (auto &p : m.position)
            points.push_back(&p);

    calculate(points);
}

BoundingSphere::BoundingSphere(std::vector<vcl::vec3 *> &points) {
    calculate(points);
}

const float &BoundingSphere::getRadius() {
    return radius;
}

const vcl::vec3 &BoundingSphere::getCenter() {
    return pt;
}

int BoundingSphere::isInCameraFrustum(vcl::camera_scene &camera) {
    for (int i = 0; i < 6; i++) {
        auto &plane = camera.get_frustum_planes()[i];
        // Distance center to plane
        float d = dot(vcl::vec4(pt, 1.0f), plane) /
                  std::sqrt(plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2]);
        if (d < -radius)
            return false;
    }

    return true;
}

int BoundingSphere::isInLightRange(vcl::camera_scene &camera, vcl::light_source &light) {
    vcl::vec3 cameraPos = camera.get_position();
    vcl::vec3 cameraDir = camera.get_direction();
    vcl::vec3 p1 = pt + radius * cameraDir;
    vcl::vec3 p2 = pt - radius * cameraDir;

    // Taking the distance to the point's projection in the camera_direction vector
    float d1 = std::fabs(vcl::dot(cameraDir, (p1 - cameraPos)));
    float d2 = std::fabs(vcl::dot(cameraDir, (p2 - cameraPos)));

    return (light.get_z_near() < d1 && d1 < light.get_z_far()) ||
           (light.get_z_near() < d2 && d2 < light.get_z_far());
}

void BoundingSphere::calculate(std::vector<vcl::vec3 *> &points) {
    pt = {0, 0, 0};
    radius = 0.0f;

    if (points.empty())
        return;

    // Getting a random point
    vcl::vec3 *x = points[0];

    // Point furthest from x
    vcl::vec3 *y = x;
    for (auto *p : points)
        if (x->dist(*p) > x->dist(*y))
            y = p;

    // Point furthest from y
    vcl::vec3 *z = y;
    for (auto *p : points)
        if (y->dist(*p) > y->dist(*z))
            z = p;

    // Placing the first sphere between y and z
    pt = 0.5f * (*y + *z);
    radius = 0.5f * y->dist(*z);

    // Iterating: if a point is not covered, create another sphere covering the current one and the point
    for (auto *p : points)
        if (pt.dist(*p) > radius + EPSILON) {
            vcl::vec3 dir = (pt - *p).normalized();
            vcl::vec3 p2 = pt + radius * dir;
            pt = 0.5f * (p2 + *p);
            radius = 0.5f * p2.dist(*p);
        }
}