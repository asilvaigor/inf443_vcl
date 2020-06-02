//
// Created by igor on 14/05/2020.
//

#ifndef PGM_BEAR_H
#define PGM_BEAR_H

#include "vcl.hpp"
#include "objects/Object.h"
#include "objects/terrain/BaseTerrain.h"

/**
 * Bear object.
 */
class Bear : public Object {
public:
    /**
     * Generates the bear.
     * @param shaders
     * @param terrain
     * @param pos
     */
    explicit Bear(Shaders &shaders, std::shared_ptr<BaseTerrain> &terrain, vcl::vec3 pos = {0, 0, 0});

    /**
     * Draws the bear in opengl, without colliding with the terrain
     * @param camera
     * @param time
     */
    void drawMesh(vcl::camera_scene &camera, float time) override;

private:
    vcl::fbx_model bear;
    std::shared_ptr<BaseTerrain> &terrain;
    vcl::vec3 direction;
    float boundingSphereRadius;
    float deltaZ;
    float speed, speedVar;
    float stepPhi, stepPeriod;
    float animationTime;
    float lastTime;

    static std::vector<GLuint> textures;

    vcl::mat4 updateTransform(float &time);
};


#endif //PGM_BEAR_H
