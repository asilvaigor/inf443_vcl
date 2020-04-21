//
// Created by igor on 15/04/20.
//

#include "TurtleGraphics.h"

TurtleGraphics::TurtleGraphics(vcl::vec3 p) {
    position = p;
    dir = {0, 0, 1};
    right = {0, -1, 0};
}

TurtleGraphics::TurtleGraphics(vcl::vec3 &p, vcl::vec3 &dir, vcl::vec3 &right) {
    this->position = p;
    this->dir = dir;
    this->right = right;
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

vcl::vec3 TurtleGraphics::getPosition() {
    return position;
}

vcl::vec3 TurtleGraphics::getDirection() {
    return dir;
}

vcl::vec3 TurtleGraphics::getRight() {
    return right;
}
