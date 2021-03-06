//
// Created by igor on 30/04/2020.
//

#ifndef PGM_CASCADESHADOW_H
#define PGM_CASCADESHADOW_H

#include <memory>
#include "vcl.hpp"
#include "SceneGui.h"
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
     * @param movableObjects
     * @param stillObjects
     * @param gui Contains info on camera, light and windows.
     * @param shaders
     * @param time
     */
    void update(std::vector<std::shared_ptr<Object>> &movableObjects,
                std::vector<std::shared_ptr<Object>> &stillObjects,
                std::shared_ptr<SceneGui> &gui, std::shared_ptr<Shaders> &shaders,
                float time);

    /**
     * Returns the combination of the z distances for each cascade division, as well as the camera total.
     * @return
     */
    std::vector<std::pair<float, float>> &getZNearFarList();

private:
    const int nCascades;
    std::shared_ptr<vcl::depth_maps> maps;
    std::vector<std::pair<float, float>> zNearFarList;
    std::vector<vcl::camera_scene> lastCamera;
    std::vector<float> lastSunAngle;
    std::vector<float> lastTime;
    std::vector<std::shared_ptr<vcl::light_source>> lights;
    std::vector<int> nMovableObjects;
    int lastUpdated;

    /**
     * Renders the shadows in the scene, using only the objects that are at the camera frustum.
     * @param objects
     * @param camera
     */
    void render(std::vector<std::shared_ptr<Object>> &objects, vcl::camera_scene &camera);

    /**
     * Renders a single object, if it is in the light range.
     * @param object
     * @param camera
     */
    void renderObject(std::shared_ptr<Object> &object, vcl::camera_scene &camera);
};


#endif //PGM_CASCADESHADOW_H
