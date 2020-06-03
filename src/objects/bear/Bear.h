//
// Created by igor on 14/05/2020.
//

#ifndef PGM_BEAR_H
#define PGM_BEAR_H

#include "vcl.hpp"
#include "objects/Object.h"
#include "objects/terrain/BaseTerrain.h"
#include "objects/forest/Forest.h"
#include "BearCompanion.h"

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
    explicit Bear(Shaders &shaders, std::shared_ptr<BaseTerrain> &terrain, std::shared_ptr<BearCompanion> &companion,
                  std::shared_ptr<vcl::vec3> &pos, std::shared_ptr<Forest> forest = nullptr);

    /**
     * Draws the bear in opengl, without colliding with the terrain
     * @param camera
     */
    void drawMesh(vcl::camera_scene &camera) override;

    /**
     * Updates the animation.
     * @param time
     */
    void update(float time) override;

private:
    vcl::fbx_model bear;
    std::shared_ptr<BaseTerrain> terrain;
    std::shared_ptr<BearCompanion> companion;
    std::shared_ptr<Forest> forest;
    std::shared_ptr<vcl::vec3> pos;
    vcl::vec3 direction;
    float boundingSphereRadius;
    float deltaZ;
    float speed, speedVar;
    float stepPhi, stepPeriod;
    float animationTime;
    float lastTime;

    const float k = 0.3;

    // TODO change this
    std::vector<vcl::vec3>poses;

    static std::vector<GLuint> textures;

    vcl::mat4 updateTransform(float &time);

    void updateDirection();
};


#endif //PGM_BEAR_H
