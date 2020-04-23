//
// Created by igor on 15/04/20.
//

#ifndef PGM_TURTLEGRAPHICS_H
#define PGM_TURTLEGRAPHICS_H

#include "vcl.hpp"

/**
 * Turtle graphics class, which is a tool to have a moving point with local rotation operations.
 * Local axis are: x forward, y left and z up.
 */
class TurtleGraphics {
public:
    /**
     * Can be given a starting point. It will start looking in the direction of the z axis.
     * @param p Starting point.
     */
    explicit TurtleGraphics(vcl::vec3 p = {0, 0, 0});

    /**
     * Customize the orientation too.
     * @param p Starting point.
     * @param dir Vector pointing to the front of the turtle.
     * @param right Vector pointing to the right of the turtle.
     */
    TurtleGraphics(vcl::vec3 &p, vcl::vec3 &dir, vcl::vec3 &right);

    /**
     * Rotates the turtle right (yaw) in its own perspective.
     * @param angle in radians.
     */
    void turnRight(float angle);

    void turnLeft(float angle);

    /**
     * Rotates the turtle in its pitch.
     * @param angle in radians.
     */
    void pitchUp(float angle);

    void pitchDown(float angle);

    /**
     * Rotates the turtle in its roll.
     * @param angle in radians.
     */
    void rollCw(float angle);

    void rollCcw(float angle);

    /**
     * The turtle walks in the direction it is pointed.
     * @param distance
     */
    void move(float distance);

    vcl::vec3 getPosition();

    vcl::vec3 getDirection();

    vcl::vec3 getRight();

private:
    vcl::vec3 position;
    vcl::vec3 dir;
    vcl::vec3 right;
};


#endif //PGM_TURTLEGRAPHICS_H
