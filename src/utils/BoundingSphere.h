//
// Created by igor on 01/05/2020.
//

#ifndef PGM_BOUNDINGSPHERE_H
#define PGM_BOUNDINGSPHERE_H

#include "vcl/vcl.hpp"

/**
 * Stores a bounding sphere for a mesh.
 * Uses Ritter's algorithm to calculate the sphere, which is O(n) and generally gives a sphere around 5-20% larger than
 * the optimal.
 * Jack Ritter. “An Efficient Bounding Sphere”. In Graphics Gems, Academic Press, 1990, pp. 301–303.
 */
class BoundingSphere {
public:
    BoundingSphere();

    BoundingSphere(vcl::vec3 pt, float radius);

    BoundingSphere(vcl::mesh &mesh);

    BoundingSphere(std::vector<vcl::mesh> &meshes);

    BoundingSphere(std::vector<vcl::mesh_skinned> &meshes);

    BoundingSphere(std::vector<vcl::vec3 *> &points);

    /**
     * Calculates if the sphere is in the camera frustum.
     * @param camera
     * @return True if the sphere is in the camera frustum.
     */
    int isInCameraFrustum(vcl::camera_scene &camera);

    /**
     * Calculates if the sphere is in the z range covered by a light source.
     * @param camera
     * @param light Light in the shadow cascade.
     * @return True if the object's shadow should be rendered.
     */
    int isInLightRange(vcl::camera_scene &camera, vcl::light_source &light);

private:
    vcl::vec3 pt;
    float radius;
    int n;

    /**
     * Uses Ritter's algorithm to calculate a bounding sphere.
     * @param points
     */
    void calculate(std::vector<vcl::vec3 *> &points);
};


#endif //PGM_BOUNDINGSPHERE_H
