//
// Created by igor on 30/04/2020.
//

#ifndef PGM_CASCADESHADOW_H
#define PGM_CASCADESHADOW_H

#include <memory>
#include "vcl.hpp"
#include "objects/Object.h"

class CascadeShadow {
public:
    CascadeShadow(vcl::light_source &light, int mapResolution);

    void startup(std::vector<std::shared_ptr<Object>> &movableObjects,
                 std::vector<std::shared_ptr<Object>> &stillObjects,
                 vcl::camera_scene &camera, std::shared_ptr<Shaders> &shaders,
                 unsigned int windowWidth, unsigned int windowHeight);

    void update(std::vector<std::shared_ptr<Object>> &movableObjects,
                std::vector<std::shared_ptr<Object>> &stillObjects,
                vcl::camera_scene &camera, std::shared_ptr<Shaders> &shaders,
                unsigned int windowWidth, unsigned int windowHeight);

private:
    const int nCascades;
    std::vector<vcl::depth_map> maps;
    std::vector<vcl::camera_scene> cameraLastUpdate;
    std::vector<double> timeLastUpdate;
    std::vector<std::shared_ptr<vcl::light_source>> lights;
    int lastUpdated;

    void render(vcl::depth_map &map, std::vector<std::shared_ptr<Object>> &objects, vcl::camera_scene &camera,
                std::shared_ptr<vcl::light_source> &light, std::shared_ptr<Shaders> &shaders, unsigned int windowWidth,
                unsigned int windowHeight);
};


#endif //PGM_CASCADESHADOW_H
