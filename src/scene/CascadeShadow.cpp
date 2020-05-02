//
// Created by igor on 30/04/2020.
//

#include "objects/terrain/Terrain.h"
#include "CascadeShadow.h"

CascadeShadow::CascadeShadow(vcl::light_source &light, int mapResolution)
        : nCascades(3) {
    lastUpdated = -1;
    maps = std::make_shared<vcl::depth_maps>(2 * nCascades, mapResolution);
    for (int i = 0; i < 2 * nCascades; i++) {
        cameraLastUpdate.emplace_back(vcl::camera_scene());
        timeLastUpdate.push_back(0.0);
    }
    float zN;
    float zF = light.get_z_near();
    for (int i = 0; i < nCascades; i++) {
        zN = zF;
        zF = light.get_z_far() / std::max(1.0f, (5.0f * (float) (nCascades - i - 1)));
        lights.emplace_back(std::make_shared<vcl::light_source>(light.get_pos(), light.get_dir(), zN, zF, i + 1));
        lights.emplace_back(std::make_shared<vcl::light_source>(light.get_pos(), light.get_dir(), zN, zF, i + 1));
    }
}

void CascadeShadow::update(std::vector<std::shared_ptr<Object> > &movableObjects,
                           std::vector<std::shared_ptr<Object> > &stillObjects,
                           vcl::camera_scene &camera, std::shared_ptr<Shaders> &shaders,
                           int windowWidth, int windowHeight) {
    int startIdx = lastUpdated;
    lastUpdated = (lastUpdated + 1) % (2 * nCascades);
    shaders->overrideWithDepth(true);

    while (lastUpdated != startIdx) {
        bool mustUpdate = false;
        vcl::camera_scene &lastCamera = cameraLastUpdate[lastUpdated];
        mustUpdate |= std::fabs(lastCamera.perspective.image_aspect - camera.perspective.image_aspect) > 1e-6;
        mustUpdate |= lastCamera.camera_position().dist(camera.camera_position()) > 1.0f;
        mustUpdate |= lastCamera.camera_direction().angle(camera.camera_direction()) > 1e-2f;
        // TODO: Add condition for moving objects.
        if (mustUpdate) {
            lights[lastUpdated]->update(camera);

            if (lastUpdated % 2 == 0)
                render(stillObjects, camera);
            else render(movableObjects, camera);

            cameraLastUpdate[lastUpdated] = camera;
            break;
        }

        lastUpdated = (lastUpdated + 1) % (2 * nCascades);
    }

    shaders->overrideWithDepth(false);
    maps->unbind_all(windowWidth, windowHeight);
    lastUpdated = (lastUpdated + 1) % (2 * nCascades);
}

void CascadeShadow::render(std::vector<std::shared_ptr<Object> > &objects, vcl::camera_scene &camera) {
    maps->bind(lastUpdated);
    camera.calculate_frustum_planes();
    for (auto &obj : objects) {
        auto* t = dynamic_cast<Terrain*> (obj.get());
        if (t != nullptr) {
            if (lastUpdated / 2 == 0)
                t->setLight(lights[lastUpdated]);
            else if (lastUpdated / 2 == 1)
                t->setLight2(lights[lastUpdated]);
            else t->setLight3(lights[lastUpdated]);
            obj->draw(camera);
        } else if (obj->getBoundingBox().isInFrustum(camera, *lights[lastUpdated])) {
            obj->setLight(lights[lastUpdated]);
            obj->draw(camera);
        }
    }
}
