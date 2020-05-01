//
// Created by igor on 30/04/2020.
//

#ifndef PGM_CASCADESHADOW_H
#define PGM_CASCADESHADOW_H

#include <memory>
#include "vcl.hpp"
#include "objects/Object.h"

/**
 * Handles the calculation and rendering of shadows, using the cascade shadowing technique.
 * This technique consists of multiple depth maps, and the correct use of them for objects at different distances.
 * Code based on <https://ahbejarano.gitbook.io/lwjglgamedev/chapter26>.
 */
class CascadeShadow {
public:
    /**
     * Initializes its variables.
     * @param light
     * @param mapResolution
     */
    CascadeShadow(vcl::light_source &light, int mapResolution);

    /**
     * Updates the lights and renders shadows, if the camera has changed significantly. This method always ensured that
     * at most one depth map will be rendered per frame, placing pended updates in a stack.
     * TODO: Implement update if an object has moved.
     * @param movableObjects
     * @param stillObjects
     * @param camera
     * @param shaders
     * @param windowWidth
     * @param windowHeight
     */
    void update(std::vector<std::shared_ptr<Object>> &movableObjects,
                std::vector<std::shared_ptr<Object>> &stillObjects,
                vcl::camera_scene &camera, std::shared_ptr<Shaders> &shaders,
                int windowWidth, int windowHeight);

private:
    const int nCascades;
    std::shared_ptr<vcl::depth_maps> maps;
    std::vector<vcl::camera_scene> cameraLastUpdate;
    std::vector<double> timeLastUpdate;
    std::vector<std::shared_ptr<vcl::light_source>> lights;
    int lastUpdated;

    /**
     * Renders the shadows in the scene, using only the objects that are at the camera frustum.
     * @param objects
     * @param camera
     */
    void render(std::vector<std::shared_ptr<Object>> &objects, vcl::camera_scene &camera);
};


#endif //PGM_CASCADESHADOW_H
