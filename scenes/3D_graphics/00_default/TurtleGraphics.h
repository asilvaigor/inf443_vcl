//
// Created by igor on 15/04/20.
//

#ifndef PGM_TURTLEGRAPHICS_H
#define PGM_TURTLEGRAPHICS_H

#include <vcl/vcl.hpp>

/**
 * Turtle graphics class, which is a tool to have a moving point with local rotation operations.
 * Local axis are: x forward, y left and z up.
 */
class TurtleGraphics {
public:
    /**
     * Can be given a starting point and a width. It will start looking up, with belly pointing in the direction of the
     * x axis.
     * @param p Starting point.
     * @param w Width of the drawing.
     */
    explicit TurtleGraphics(vcl::vec3 p = {0, 0, 0}, float w = 1.0);

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

    /**
     * Sets the drawing width.
     * @param w
     */
    void setWidth(float w);

    vcl::vec3 getPosition();

    float getWidth();

private:
    vcl::vec3 position;
    vcl::vec3 dir;
    vcl::vec3 right;
    float width;
};


#endif //PGM_TURTLEGRAPHICS_H
