//
// Created by igor on 23/04/2020.
//

#ifndef PGM_OBJECT_H
#define PGM_OBJECT_H

#include "vcl.hpp"
#include "shaders/Shaders.h"
#include "utils/BoundingBox.h"

/**
 * Abstract object class to be placed in a scene.
 */
class Object {
public:
    Object(bool movable);

    /**
     * Calls vcl::draw to draw the object in the scene.
     * @param camera
     */
    virtual void draw(const vcl::camera_scene &camera) = 0;

    /**
     * Sets the light object that the object will be rendered with.
     * @param light
     */
    void setLight(std::shared_ptr<vcl::light_source> &light);

    /**
     * If the object contains some kind of animation, and moves. This is required to update shadows.
     * @return True if the object moves.
     */
    bool isMovable() const;

    /**
     * Minimum box parallel to the axis that contains the whole object.
     * @return
     */
    BoundingBox &getBoundingBox();

protected:
    const bool movable;
    std::shared_ptr<vcl::light_source> light;
    BoundingBox boundingBox;
};


#endif //PGM_OBJECT_H
