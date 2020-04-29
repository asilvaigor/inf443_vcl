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
    light = vcl::light_source({10, 0, 3});
    stillDepthMap = std::make_shared<vcl::depth_map>(1024);
    movableDepthMap = std::make_shared<vcl::depth_map>(1024);
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
    // Preprocessing the depth map for objects that do not move
    updateDepthMap(true);

    while (gui->isRunning()) {
        gui->update();
        updateScene();
        gui->render();
    }
}

void Scene::updateScene() {
    shaders->overrideWithWireframe(gui->showWireframe());
    updateDepthMap(false);

    whiteTexture->bind();
    for (auto &obj : stillObjects) {
        obj->draw(gui->getCamera(), light);
        whiteTexture->bind();
    }
    for (auto &obj : movableObjects) {
        obj->draw(gui->getCamera(), light);
        whiteTexture->bind();
    }
}

void Scene::updateDepthMap(bool still) {
    if (still) stillDepthMap->bind();
    else movableDepthMap->bind();

    shaders->overrideWithDepth(true);

    auto &objects = movableObjects;
    if (still) objects = stillObjects;
    for (auto &obj : objects) {
        obj->draw(gui->getCamera(), light);
        whiteTexture->bind();
    }

    shaders->overrideWithDepth(false);

    if (still) stillDepthMap->unbind(gui->getWindowWidth(), gui->getWindowHeight());
    else movableDepthMap->unbind(gui->getWindowWidth(), gui->getWindowHeight());
}
