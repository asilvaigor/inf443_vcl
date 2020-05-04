//
// Created by igor on 22/04/2020.
//

#include "Scene.h"
#include "utils/SingletonException.h"

bool Scene::exists = false;

Scene &Scene::getInstance(std::string windowTitle) {
    if (exists)
        throw SingletonException("Scene");
    exists = true;
    static Scene instance(windowTitle);
    return instance;
}

Scene::Scene(std::string &windowTitle) {
    gui = SceneGui::getInstance(windowTitle);
    shaders = Shaders::getInstance();

    // Creating rendering stuff
    whiteTexture = std::make_shared<Texture>();
    grid = std::make_shared<Grid>(*shaders);

    vcl::light_source light({100, 0, 100}, {-1, 0, -1},
            gui->getCamera().perspective.z_near, gui->getCamera().perspective.z_far);
    cascadeShadow = std::make_shared<CascadeShadow>(light, 3072);
}

Shaders &Scene::getShaders() {
    return *shaders;
}

void Scene::addObject(std::shared_ptr<Object> &object) {
    if (object->isMovable())
        movableObjects.push_back(object);
    else stillObjects.push_back(object);
}

void Scene::display() {
    while (gui->isRunning()) {
        gui->update();
        updateScene();
        gui->render();
    }
}

void Scene::updateScene() {
    cascadeShadow->update(movableObjects, stillObjects, gui, shaders);
    shaders->overrideWithWireframe(gui->showVertices());
    if (gui->showGrid())
        grid->draw(gui->getCamera());

    whiteTexture->bind();
    for (auto &obj : stillObjects) {
        if (obj->getBoundingSphere().isInCameraFrustum(gui->getCamera())) {
            obj->draw(gui->getCamera());
            whiteTexture->bind();
        }
    }
    for (auto &obj : movableObjects) {
        if (obj->getBoundingSphere().isInCameraFrustum(gui->getCamera())) {
            obj->draw(gui->getCamera());
            whiteTexture->bind();
        }
    }
}
