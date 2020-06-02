//
// Created by igor on 23/04/2020.
//

#ifndef PGM_OBJECT_H
#define PGM_OBJECT_H

#include "vcl.hpp"
#include "shaders/Shaders.h"
#include "utils/BoundingBox.h"
#include "utils/BoundingSphere.h"
#include "utils/BillboardGenerator.h"

/**
 * Abstract object class to be placed in a scene.
 */
class Object {
public:
    explicit Object(bool movable, bool shadowless = false);

    /**
     * Destructor
     */
    virtual ~Object();

    /**
     * Calls vcl::draw to draw the object in the scene.
     * @param camera
     */
    void draw(vcl::camera_scene &camera, float time = 0.0f);

    /**
     * Draws the object's mesh in screen.
     * @param camera
     * @param time
     */
    virtual void drawMesh(vcl::camera_scene &camera, float time = 0.0f) = 0;

    /**
     * Sets the light object that the object will be rendered with.
     * @param light
     */
    void setLight(std::shared_ptr<vcl::light_source> &light);

    /**
     * Gets the light object that the object will be rendered with.
     * @param light
     */
    std::shared_ptr<vcl::light_source> &getLight();

    /**
     * If the object contains some kind of animation, and moves. This is required to update shadows.
     * @return True if the object moves.
     */
    bool isMovable() const;

    /**
     * If shadow should be rendered with this object.
     * @return True if the object must be included in the depth map.
     */
    bool hasShadow() const;

    /**
     * Sphere that covers the whole object.
     * @return
     */
    BoundingSphere &getBoundingSphere();

    /**
     * Minimum box that covers the whole object.
     * @return
     */
    BoundingBox &getBoundingBox();

    vcl::vec3 &getPosition();

    vcl::mat3 &getOrientation();

    BillboardGenerator &getBillboard();

protected:
    const bool movable;
    const bool shadowless;
    std::vector<std::shared_ptr<vcl::light_source>> lights;
    BoundingSphere boundingSphere;
    BoundingBox boundingBox;
    BillboardGenerator billboard;
    vcl::vec3 position;
    vcl::mat3 orientation;
};


#endif //PGM_OBJECT_H
