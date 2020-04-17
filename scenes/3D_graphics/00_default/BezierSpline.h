//
// Created by igor on 16/04/20.
//

#ifndef PGM_BEZIERSPLINE_H
#define PGM_BEZIERSPLINE_H

#include "vcl/vcl.hpp"

/**
 * Generates a mesh which consists of circles in which their centers follow a Bezier curve, and their radius are
 * interpolated linearly, forming a snake-like figure.
 */
class BezierSpline {
public:
    BezierSpline();

    /**
     * Adds a point to be interpolated in the bezier curve.
     * @param pos Position.
     * @param radius Radius at the current point.
     * @param leftHandle Position of the left point that determines the curve's direction.
     * @param rightHandle Position of the right point that determines the curve's direction.
     */
    void addPoint(vcl::vec3 pos, float radius, vcl::vec3 leftHandle, vcl::vec3 rightHandle);

    /**
     * Converts the curve to a mesh.
     * @return mesh
     */
    vcl::mesh toMesh();

private:
    int nPoints;
    std::vector<vcl::vec3> pts;
    std::vector<float> radius;
    std::vector<vcl::vec3> leftHandle;
    std::vector<vcl::vec3> rightHandle;
};


#endif //PGM_BEZIERSPLINE_H
