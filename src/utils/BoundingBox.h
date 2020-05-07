//
// Created by igor on 04/05/2020.
//

#ifndef PGM_BOUNDINGBOX_H
#define PGM_BOUNDINGBOX_H

#include "vcl.hpp"

/**
 * Minimum box that covers an object.
 */
class BoundingBox {
public:
    /**
     * The box will be infinite.
     */
    BoundingBox();

    /**
     * Starts the box with the given boundaries.
     * @param minX
     * @param maxX
     * @param minY
     * @param maxY
     * @param minZ
     * @param maxZ
     */
    BoundingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);

    /**
     * Updates the box to include this point inside it.
     * @param point
     */
    void update(const vcl::vec3 &point);

    /**
     * Calculates the corner points of the box.
     * @param camera
     * @return Vector of size 8 with the corners.
     */
    std::vector<vcl::vec3> getCorners();

    /**
     * Calculates the relative size of the maximum dimension of the box in relationship to the screen's diagonal.
     * @param camera
     * @return Float bigger than 0 indicating how big will the box be in the screen.
     */
    float relativeSize(vcl::camera_scene &camera);

private:
    bool empty;
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;
};


#endif //PGM_BOUNDINGBOX_H
