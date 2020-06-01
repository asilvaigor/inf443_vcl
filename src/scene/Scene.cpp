//
// Created by igor on 22/04/2020.
//

#include <src/objects/boid/Boid.h>
#include "Scene.h"
#include "objects/forest/Forest.h"
#include "utils/SingletonException.h"

bool Scene::exists = false;
bool Scene::deterministic = true;

Scene &Scene::getInstance(std::string windowTitle) {
    if (exists)
        throw SingletonException("Scene");
    exists = true;
    static Scene instance(windowTitle);
    return instance;
}

Scene::Scene(std::string &windowTitle) {
    gui = SceneGui::getInstance(windowTitle);

    std::cout << "Initializing scene... ";

    shaders = Shaders::getInstance();

    // Creating rendering stuff
    whiteTexture = std::make_shared<Texture>(255, 255, 255);
    grid = std::make_shared<Grid>(*shaders);

    vcl::light_source light({100, 0, 100}, {-1, 0, -1},
            gui->getCamera().perspective.z_near, gui->getCamera().perspective.z_far);
    cascadeShadow = std::make_shared<CascadeShadow>(light, 2048);

    std::cout << "Finished" << std::endl;
}

Shaders &Scene::getShaders() {
    return *shaders;
}

std::shared_ptr<SceneGui> Scene::getGui() {
    return gui;
}

void Scene::addObject(std::shared_ptr<Object> &object) {
    if (object->isMovable())
        movableObjects.push_back(object);
    else stillObjects.push_back(object);
}

void Scene::display() {
    gui->open();
    while (gui->isRunning()) {
        gui->update();
        updateScene();
        gui->render();
    }
}

void Scene::updateScene() {
    float time = glfwGetTime();

    cascadeShadow->update(movableObjects, stillObjects, gui, shaders, time);
    shaders->override("wireframe", gui->showVertices());
    if (gui->showGrid())
        grid->draw(gui->getCamera());

    whiteTexture->bind();
    for (auto &obj : stillObjects) {
        // Forest should be decomposed in its objects
        auto *f = dynamic_cast<Forest *>(obj.get());
        auto *b = dynamic_cast<Boid *>(obj.get());
        if (f != nullptr)
            for (auto &o : f->getObjects())
                o->draw(gui->getCamera());
        else if (b != nullptr){
            b->updateBirds();
            for (auto &o : b->getObjects())
                o->draw(gui->getCamera());
        }
        else obj->draw(gui->getCamera());
        whiteTexture->bind();
    }
    for (auto &obj : movableObjects) {
        obj->draw(gui->getCamera(), time);
        whiteTexture->bind();
    }
}
