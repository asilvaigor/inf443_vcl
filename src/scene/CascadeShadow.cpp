//
// Created by igor on 30/04/2020.
//

#include "objects/terrain/BaseTerrain.h"
#include "objects/forest/Forest.h"
#include "CascadeShadow.h"

CascadeShadow::CascadeShadow(vcl::light_source &light, int mapResolution)
        : nCascades(3) {
    lastUpdated = 0;
    maps = std::make_shared<vcl::depth_maps>(2 * nCascades, mapResolution);
    for (int i = 0; i < 2 * nCascades; i++) {
        lastCamera.emplace_back(vcl::camera_scene());
        lastSunAngle.emplace_back(0.0f);
        lastTime.emplace_back(0.0f);
        nMovableObjects.emplace_back(0);
    }
    float zN;
    float zF = light.get_z_near();
    for (int i = 0; i < nCascades; i++) {
        zN = zF;
        zF = light.get_z_far() / std::max(1.0f, (4.0f * (float) (nCascades - i - 1)));
        lights.emplace_back(std::make_shared<vcl::light_source>(light.get_pos(), light.get_dir(), zN, zF, i));
        lights.emplace_back(std::make_shared<vcl::light_source>(light.get_pos(), light.get_dir(), zN, zF, i));
    }
}

void CascadeShadow::update(std::vector<std::shared_ptr<Object> > &movableObjects,
                           std::vector<std::shared_ptr<Object> > &stillObjects,
                           std::shared_ptr<SceneGui> &gui, std::shared_ptr<Shaders> &shaders,
                           float time) {
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
        mustUpdate |= std::fabs(gui->getSunAngle() - lastSunAngle[lastUpdated]) > 1e-3f;
        float timeThreshold = nMovableObjects[lastUpdated] > 0 ? 1e-3f : 1.0f;
        mustUpdate |= (lastUpdated % 2 == 1) && (time - lastTime[lastUpdated] > timeThreshold);

        // If camera has changed or objects have moved, update the objects
        if (mustUpdate) {
            float sunAngle = gui->getSunAngle();
            vcl::vec3 lightPos(500.0f * std::cos((float) (0.5 * M_PI) - sunAngle), 0.0f,
                               500.0f * std::sin((float) (0.5 * M_PI) - sunAngle));
            vcl::vec3 lightDir = -lightPos.normalized();
            lights[lastUpdated]->update(camera, lightPos, lightDir);
            lastCamera[lastUpdated] = camera;
            lastSunAngle[lastUpdated] = sunAngle;
            lastTime[lastUpdated] = time;

            if (lastUpdated % 2 == 0)
                render(stillObjects, camera);
            else render(movableObjects, camera);
            lastUpdated = (lastUpdated + 1) % (2 * nCascades);
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
    nMovableObjects[lastUpdated] = 0;

    for (auto &obj : objects) {
        // If it is the terrain, it will be in all cascades
        auto *t = dynamic_cast<BaseTerrain *> (obj.get());
        auto *f = dynamic_cast<Forest *>(obj.get());
        if (t != nullptr) {
            t->setLight(lights, lastUpdated);
            t->draw(camera);
        } else if (f != nullptr) {
            // A forest will be decomposed in its objects
            for (auto &o : f->getObjects())
                renderObject(o, camera);
        } else renderObject(obj, camera);
    }
}

void CascadeShadow::renderObject(std::shared_ptr<Object> &obj, vcl::camera_scene &camera) {
    if (obj->getLight()->get_shadow_map_id() == lastUpdated ||
           obj->getBoundingSphere().isInLightRange(camera, *lights[lastUpdated])) {
        // A normal object must be in the correct frustum to be rendered
        obj->setLight(lights[lastUpdated]);
        if (obj->isMovable()) {
            obj->draw(camera, lastTime[lastUpdated]);
            nMovableObjects[lastUpdated]++;
        } else obj->draw(camera);
    }
}
