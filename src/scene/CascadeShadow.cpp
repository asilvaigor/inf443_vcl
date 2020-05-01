//
// Created by igor on 30/04/2020.
//

#include "CascadeShadow.h"

CascadeShadow::CascadeShadow(vcl::light_source &light, int mapResolution)
        : nCascades(3) {
    lastUpdated = -1;
    for (int i = 0; i < 2 * nCascades; i++) {
        maps.push_back(vcl::depth_map(mapResolution));
        cameraLastUpdate.push_back(vcl::camera_scene());
        timeLastUpdate.push_back(0.0);
    }
    float zN;
    float zF = light.get_z_near();
    for (int i = 0; i < nCascades; i++) {
        zN = zF;
        zF = light.get_z_far() / std::max(1.0f, (10.0f * (nCascades - i - 1)));
        lights.push_back(std::make_shared<vcl::light_source>(light.get_pos(), light.get_dir(), zN, zF, i + 1));
    }
}

void CascadeShadow::startup(std::vector<std::shared_ptr<Object> > &movableObjects,
                            std::vector<std::shared_ptr<Object> > &stillObjects,
                            vcl::camera_scene &camera, std::shared_ptr<Shaders> &shaders,
                            unsigned int windowWidth, unsigned int windowHeight) {
    for (int i = 0; i < 2 * nCascades; i++)
        update(movableObjects, stillObjects, camera, shaders, windowWidth, windowHeight);
}

void CascadeShadow::update(std::vector<std::shared_ptr<Object> > &movableObjects,
                           std::vector<std::shared_ptr<Object> > &stillObjects,
                           vcl::camera_scene &camera, std::shared_ptr<Shaders> &shaders,
                           unsigned int windowWidth, unsigned int windowHeight) {
    int startIdx = lastUpdated;
    lastUpdated = (lastUpdated + 1) % (2 * nCascades);
    while (lastUpdated != startIdx) {
        if (lastUpdated % 2 == 0)
            lights[lastUpdated / 2]->update(camera);

        bool mustUpdate = false;
        vcl::camera_scene &lastCamera = cameraLastUpdate[lastUpdated];
        mustUpdate |= std::fabs(lastCamera.perspective.image_aspect - camera.perspective.image_aspect) > 1e-6;
        mustUpdate |= lastCamera.camera_position().dist(camera.camera_position()) > 1.0f;
        mustUpdate |= lastCamera.camera_direction().angle(camera.camera_direction()) > 1e-2f;
        if (mustUpdate) {
            if (lastUpdated % 2 == 0)
                render(maps[lastUpdated], stillObjects, camera, lights[lastUpdated / 2], shaders,
                        windowWidth, windowHeight);
            else render(maps[lastUpdated], movableObjects, camera, lights[lastUpdated / 2], shaders,
                    windowWidth, windowHeight);
            cameraLastUpdate[lastUpdated] = camera;
            break;
        }

        lastUpdated = (lastUpdated + 1) % (2 * nCascades);
    }
    lastUpdated = (lastUpdated + 1) % (2 * nCascades);
}

void CascadeShadow::render(vcl::depth_map &map, std::vector<std::shared_ptr<Object> > &objects,
                           vcl::camera_scene &camera, std::shared_ptr<vcl::light_source> &light,
                           std::shared_ptr<Shaders> &shaders, unsigned int windowWidth, unsigned int windowHeight) {
    map.bind();
    shaders->overrideWithDepth(true);

    for (auto &obj : objects) {
        obj->setLight(light);
        obj->draw(camera);
    }

    shaders->overrideWithDepth(false);
    map.unbind(windowWidth, windowHeight);
}
