//
// Created by igor on 01/05/2020.
//

#ifndef PGM_BOUNDINGBOX_H
#define PGM_BOUNDINGBOX_H

#include <memory>
#include "vcl/vcl.hpp"

/**
 * Stores a box parallel to the axis.
 */
class BoundingBox {
public:
    BoundingBox();

    BoundingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

    /**
     * Updates the box to include this point inside it.
     * @param point
     */
    void update(const vcl::vec3 &point);

    /**
     * Calculates if the box is in the camera frustum.
     * @param camera
     * @return True if the object is in the camera frustum.
     */
    int isInCameraFrustum(vcl::camera_scene &camera);

    /**
     * Calculates if the box is in the light frustum.
     * @param camera
     * @param light Light in the shadow cascade.
     * @return True if the object's shadow should be rendered.
     */
    int isInLightFrustum(vcl::camera_scene &camera, vcl::light_source &light);

    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;
};


#endif //PGM_BOUNDINGBOX_H
