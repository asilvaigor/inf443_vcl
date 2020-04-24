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
    /**
     * Calls vcl::draw to draw the object in the scene.
     * @param camera
     * @param sunMatrix Matrix for the light source transformation.
     */
    virtual void draw(const vcl::camera_scene &camera, vcl::mat4 &sunMatrix) = 0;
};


#endif //PGM_OBJECT_H
