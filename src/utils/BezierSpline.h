//
// Created by igor on 16/04/20.
//

#ifndef PGM_BEZIERSPLINE_H
#define PGM_BEZIERSPLINE_H

#include "vcl.hpp"

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
     * @param r Radius at the current point.
     * @param lH Position of the left point that determines the curve's direction.
     * @param rH Position of the right point that determines the curve's direction.
     */
    void addPoint(vcl::vec3 pos, float r, vcl::vec3 lH, vcl::vec3 rH);

    /**
     * Converts the curve to a mesh.
     * @return mesh
     */
    vcl::mesh toMesh();

    /**
     * Calculates a point in the bezier spline, using its equation.
     * @param i Index of the starting point of the bezier spline which the point will be calculated.
     * @param offset Float between 0 and 1, proportion in the bezier curve to get the point.
     * @return Point position.
     */
    vcl::vec3 position(int i, float offset);

    /**
     * Calculates a tangent vector in the bezier spline, using its derivative.
     * @param i Index of the starting point of the bezier spline which the derivative will be calculated.
     * @param offset Float between 0 and 1, proportion in the bezier curve to get the tangent.
     * @return Tangent direction.
     */
    vcl::vec3 tangent(int i, float offset);

    int getNPoints() const;

private:
    int nPoints;
    std::vector<vcl::vec3> pts;
    std::vector<float> radius;
    std::vector<vcl::vec3> leftHandle;
    std::vector<vcl::vec3> rightHandle;
};


#endif //PGM_BEZIERSPLINE_H
