//
// Created by igor on 23/04/2020.
//

#ifndef PGM_OBJECT_H
#define PGM_OBJECT_H

#include "vcl.hpp"
#include "shaders/Shaders.h"

/**
 * Abstract object class to be placed in a scene.
 */
class Object {
public:
    Object(bool movable);

    /**
     * Calls vcl::draw to draw the object in the scene.
     * @param camera
     * @param light
     */
    virtual void draw(const vcl::camera_scene &camera, vcl::light_source &light) = 0;

    /**
     * If the object contains some kind of animation, and moves. This is required to update shadows.
     * @return True if the object moves.
     */
    bool isMovable();

private:
    const bool movable;
};


#endif //PGM_OBJECT_H
