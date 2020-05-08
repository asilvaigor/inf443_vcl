//
// Created by igor on 30/04/2020.
//

#include "objects/terrain/FlatSurface.h"
#include "CascadeShadow.h"

CascadeShadow::CascadeShadow(vcl::light_source &light, int mapResolution)
        : nCascades(3) {
    lastUpdated = 0;
    maps = std::make_shared<vcl::depth_maps>(2 * nCascades, mapResolution);
    for (int i = 0; i < 2 * nCascades; i++) {
        lastCamera.emplace_back(vcl::camera_scene());
        lastSunAngle.emplace_back(0);
        lastTime.emplace_back(0.0);
    }
    float zN;
    float zF = light.get_z_near();
    for (int i = 0; i < nCascades; i++) {
        zN = zF;
        zF = light.get_z_far() / std::max(1.0f, (4.0f * (float) (nCascades - i - 1)));
        lights.emplace_back(std::make_shared<vcl::light_source>(light.get_pos(), light.get_dir(), zN, zF, i + 1));
        lights.emplace_back(std::make_shared<vcl::light_source>(light.get_pos(), light.get_dir(), zN, zF, i + 1));
    }
}

void CascadeShadow::update(std::vector<std::shared_ptr<Object> > &movableObjects,
                           std::vector<std::shared_ptr<Object> > &stillObjects,
                           std::shared_ptr<SceneGui> &gui, std::shared_ptr<Shaders> &shaders) {
    shaders->override("depth", true);

    int startIdx = lastUpdated;
    bool firstTime = true;
    while (firstTime || lastUpdated != startIdx) {
        firstTime = false;
        bool mustUpdate = false;

        vcl::camera_scene &camera = gui->getCamera();
        vcl::camera_scene &prevCamera = lastCamera[lastUpdated];
        mustUpdate |= std::fabs(prevCamera.perspective.image_aspect - camera.perspective.image_aspect) > 1e-6;
        mustUpdate |= prevCamera.camera_position().dist(camera.camera_position()) > 1.0f;
        mustUpdate |= prevCamera.camera_direction().angle(camera.camera_direction()) > 1e-2f;
        mustUpdate |= std::fabs(gui->getSunAngle() - lastSunAngle[lastUpdated]) > 1e-3;
        // TODO: Add condition for moving objects.

        // If camera has changed or objects have moved, update the objects
        if (mustUpdate) {
            float sunAngle = gui->getSunAngle();
            vcl::vec3 lightPos(500.0f * std::cos((float) (0.5 * M_PI) - sunAngle), 0.0f,
                               500.0f * std::sin((float) (0.5 * M_PI) - sunAngle));
            vcl::vec3 lightDir = -lightPos.normalized();
            lights[lastUpdated]->update(camera, lightPos, lightDir);

            if (lastUpdated % 2 == 0)
                render(stillObjects, camera);
            else render(movableObjects, camera);

            lastCamera[lastUpdated] = camera;
            lastSunAngle[lastUpdated] = sunAngle;
            break;
        }

        lastUpdated = (lastUpdated + 1) % (2 * nCascades);
    }

    shaders->override("depth", false);
    maps->unbind_all(gui->getWindowWidth(), gui->getWindowHeight());
}

void CascadeShadow::render(std::vector<std::shared_ptr<Object> > &objects, vcl::camera_scene &camera) {
    maps->bind(lastUpdated);
    camera.calculate_frustum_planes();

    for (auto &obj : objects) {
        // If it is the terrain, it will be in all cascades
        auto *t = dynamic_cast<FlatSurface *> (obj.get());
        if (t != nullptr) {
            t->setLight(lights[lastUpdated], lastUpdated / 2 + 1);
            t->draw(camera);
            // A normal object must be in the correct frustum to be rendered
        } else if (obj->getLight()->get_shadow_map_id() == lastUpdated / 2 + 1 ||
                obj->getBoundingSphere().isInLightRange(camera, *lights[lastUpdated])) {
            obj->setLight(lights[lastUpdated]);
            obj->draw(camera);
        }
    }
}
