//
// Created by igor on 15/04/20.
//

#include "TurtleGraphics.h"

TurtleGraphics::TurtleGraphics(vcl::vec3 p, float w) {
    position = p;
    width = w;
    dir = {0, 0, 1};
    right = {0, -1, 0};
}

void TurtleGraphics::turnRight(float angle) {
    vcl::vec3 axis = cross(dir, right).normalized();
    vcl::Quaternion rot(axis, angle);

    dir = rot.rotated(dir).normalized();
    right = rot.rotated(right).normalized();
}

void TurtleGraphics::turnLeft(float angle) {
    turnRight(-angle);
}

void TurtleGraphics::pitchUp(float angle) {
    vcl::Quaternion rot(right, angle);
    dir = rot.rotated(dir).normalized();
    std::cout << "dir: " << dir << std::endl;
}

void TurtleGraphics::pitchDown(float angle) {
    pitchUp(-angle);
}

void TurtleGraphics::rollCw(float angle) {
    vcl::Quaternion rot(dir, angle);
    right = rot.rotated(right).normalized();
}

void TurtleGraphics::rollCcw(float angle) {
    rollCw(-angle);
}

void TurtleGraphics::move(float distance) {
    position += dir * distance;
}

void TurtleGraphics::setWidth(float w) {
    width = w;
}

vcl::vec3 TurtleGraphics::getPosition() {
    return position;
}

float TurtleGraphics::getWidth() {
    return width;
}
